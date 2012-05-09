#define _MULTI_THREADED

#include <nativeblack/nativeblack.h>
#include "nativeblue.h"
#include "pthread_rwlock.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <alloca.h>
#include <ctype.h>

int    nablue_main(int argc, char **argv);
void*  main_entry(void *parm);
int    __wrap_write(int, void *, size_t);


void BrowserSysCallWrite( const char* dev, const char* buffer, size_t size );

enum
{
  kMainStatusStarting,
  kMainStatusRunning,
  kMainStatusComplete,
  kMainStatusClosed,
  kMainStatusError,
  kMainStatusUnknown
};

struct MainThreadStatus 
{
  int m_Status;
  int m_Result;
};

typedef struct MainThreadStatus MainThreadStatus ;

pthread_t         g_MainThread;
MainThreadStatus  g_MainThreadStatus;


// --------------------------------------------------------------------------
// Shared command buffer for commands that need to run on the NaCl thread
//   #todo reduce locking
// --------------------------------------------------------------------------

typedef struct NaBlueCommand      NaBlueCommand;
typedef struct NaBlueWriteCommand NaBlueWriteCommand;

enum
{
  kNaBlueCommandWrite = 1,
};

struct NaBlueCommand
{
  int32_t m_Command;
  size_t  m_Size;
};

struct NaBlueWriteCommand
{
  int32_t m_Fd;
  size_t  m_Count;
};

void FlushCommands(void);

// --------------------------------------------------------------------------
// Scratch allocation used by browser command buffer
//   - These functions can be overriden by application if desired
//   - Buffer will be divided in half and double buffered
//   - If NaBlueRealloc fails (fixed memory conditions), the command will not
//     be processed.
// --------------------------------------------------------------------------

size_t NaBlueDefaultScratchSize( void )
{
  return 64 * 1024;
}

// Note any replacement should guarantee alignment of at least 32 bits.
char* NaBlueAlloc( size_t size )
{
  return (char*)malloc( size );
}

void NaBlueFree( char* buf )
{
  free( buf );
}

char* NaBlueRealloc( char* old_buf, size_t new_size )
{
  return (char*)realloc( old_buf, new_size );  
}

// This function is provided for the application to override in very space-sentitive
// situations where rounding up by large amounts wouldn't be acceptable.
size_t NaBlueReallocSizeRoundUp( size_t size )
{
  return next_pow2_u64( (uint64_t) size );
}

// --------------------------------------------------------------------------
// Command buffer scratch
// --------------------------------------------------------------------------

static char*             s_NaBlueCommandScratch[2]       = { NULL, NULL };
static size_t            s_NaBlueCommandScratchOffset[2] = { 0,    0    };
static size_t            s_NaBlueCommandScratchSize[2]   = { 0,    0    };
static int               s_NaBlueCommandIndex            = 0;

// --------------------------------------------------------------------------
// s_NaBlueCommandAllocLock: 
//   - Since other (unknown) threads can be allocating from the same pool, they
//     must lock each other out. 
//   - #todo per-thread pool.
//   - However, the FlushCommands thread does not need to aquire this lock, it just
//     needs to make sure the threads can't be writing when it swaps the buffer.
//   - See #note-01 below.
// --------------------------------------------------------------------------

static pthread_mutex_t   s_NaBlueCommandAllocLock        = PTHREAD_MUTEX_INITIALIZER;

// --------------------------------------------------------------------------
// s_NaBlueCommandWriteLock may need some clarification: #note-01
//   - Some poeple might say this "an abuse" of the rwlock. I say it was just mis-named.
//   - Alternatively, you could think of it as a rwlock on g_NaBlueCommandIndex
//   - No writer thread needs to lock any other writer thread out. Since they're already
//     constrained by the allocation to be working in separate, unrelated memory.
//   - However, the FlushCommands thread can't swap buffers until all the writes are 
//     ready.
//   - So:
//     1) the reader (NaBlueCommandSwapBuffers) must aquire a write lock to make sure they
//        are complete.
//     2) the writers (threads generating commands) each aquire a read lock while writing.
//     3) This will ensure that all writes are complete before swap.
//   - #todo g_NaBlueCommandWriteLock can be reduced to a single-writer/single-reader
//           "lock" per writing thread. 
//           - But SwapBuffers may still need to spin for lack of work while waiting for
//             the last write from the last thread (potentially)
// --------------------------------------------------------------------------

static pthread_rwlock_t  s_NaBlueCommandWriteLock      = PTHREAD_RWLOCK_INITIALIZER;

int NaBlueCommandStartup( void )
{
  size_t scratch_size = NaBlueDefaultScratchSize();

  s_NaBlueCommandScratch[0]     = NaBlueAlloc( scratch_size );
  s_NaBlueCommandScratchSize[0] = scratch_size;

  if ( s_NaBlueCommandScratch[0] == NULL )
  {
    return (0);
  }

  s_NaBlueCommandScratch[1]     = NaBlueAlloc( scratch_size );
  s_NaBlueCommandScratchSize[1] = scratch_size;

  if ( s_NaBlueCommandScratch[1] == NULL )
  {
    NaBlueFree( s_NaBlueCommandScratch[0] );
    return (0);
  }

  s_NaBlueCommandIndex = 0;

  return (1);
}

void NaBlueCommandShutdown( void )
{
  NaBlueFree( s_NaBlueCommandScratch[0] );
  NaBlueFree( s_NaBlueCommandScratch[1] );

  s_NaBlueCommandScratchSize[0]   = 0;
  s_NaBlueCommandScratchSize[1]   = 0;
  s_NaBlueCommandScratchOffset[0] = 0;
  s_NaBlueCommandScratchOffset[1] = 0;
}

// Note: You need to lock s_NaBlueCommandLock before calling this. 
//       And don't unlock until the memory has been filled and is ready.
NaBlueCommand* NaBlueCommandAllocate( int cmd, size_t size )
{
  pthread_mutex_lock(&s_NaBlueCommandAllocLock);

  size_t alloc_size = sizeof( NaBlueCommand ) + size;

  int    scratch_index     = s_NaBlueCommandIndex;
  size_t scratch_size      = s_NaBlueCommandScratchSize[ scratch_index ];
  size_t scratch_used      = s_NaBlueCommandScratchOffset[ scratch_index ];
  size_t scratch_available = scratch_size - scratch_used;

  if ( alloc_size > scratch_available )
  {
    // One of two things will happen in this case:
    //   (1) Realloc the buffer (raise high-water mark) and continue
    //   (2) Realloc fails (fixed memory condition); command silently dropped. Assume application is signalled by having overridden NaBlueRealloc.

    size_t scratch_needed    = scratch_used + alloc_size;
    size_t scratch_requested = NaBlueReallocSizeRoundUp( scratch_needed );
    char*  scratch_current   = s_NaBlueCommandScratch[ scratch_index ];
    char*  scratch_new       = NaBlueRealloc( scratch_current, scratch_requested );

    if ( !scratch_new )
    {
      pthread_mutex_unlock(&s_NaBlueCommandAllocLock);
      return NULL; 
    }

    s_NaBlueCommandScratch[ scratch_index ]     = scratch_new;
    s_NaBlueCommandScratchSize[ scratch_index ] = scratch_requested;

    scratch_size      = scratch_requested;
    scratch_available = scratch_size - scratch_used;
  }

  char*           scratch = s_NaBlueCommandScratch[ scratch_index ] + scratch_used;
  NaBlueCommand*  command = (NaBlueCommand*)scratch;
  
  command->m_Command = cmd;
  command->m_Size    = size;
  
  s_NaBlueCommandScratchOffset[ scratch_index ] += alloc_size;

  pthread_mutex_unlock(&s_NaBlueCommandAllocLock);

  return ( command );
}

void NaBlueCommandSwapBuffers( void )
{
  // Aquire a *write* lock to ensure that all threads are finished writing. See #note-01
  pthread_rwlock_wrlock( &s_NaBlueCommandWriteLock );

  // Note s_NaBlueCommandAllocLock doesn't need to be aquired since it's guaranteed to only 
  // be referring to the buffer that's currently being written to by the writing threads.
  //   - At this point there are none, guaranteed by the above.

  // #todo s_NaBlueCommandWriteLock could be, in principal, double buffered:
  //       - The read lock could be aquired as soon as the index is flipped
  //       - So make sure there's a fence that guaratees that happens after the offset clear.
  //       - The writers could be holding the lock from the previous buffer in that case
  //         (before this index swap), so that would have to be resolved.

  s_NaBlueCommandScratchOffset[ s_NaBlueCommandIndex^1 ] = 0;

  // ...would need a fence here... see above

  s_NaBlueCommandIndex ^= 1;

  // Alow writing threads to continue...
  pthread_rwlock_unlock( &s_NaBlueCommandWriteLock );
}

NaBlueCommand* NaBlueCommandGetNext( size_t offset )
{
  int back_index = s_NaBlueCommandIndex ^ 1;
  int back_used  = s_NaBlueCommandScratchOffset[ back_index ];

  if ( offset < back_used )
  {
    return (NaBlueCommand*)( s_NaBlueCommandScratch[ back_index ] + offset );
  }
  return NULL;
}

// --------------------------------------------------------------------------
// Required entry point functions -------------------------------------------
// --------------------------------------------------------------------------

PP_Bool NaBlackHandleInputEvent( PP_Resource input_event )
{
  return PP_TRUE;
}

PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] )
{
  NaBlueCommandStartup();

  g_MainThreadStatus.m_Status = kMainStatusStarting;
  int main_start_result       = pthread_create(&g_MainThread, NULL, main_entry, NULL);
  if ( main_start_result != 0 )
  {
    g_MainThreadStatus.m_Status = kMainStatusError;
    fprintf(stderr,"ERROR: main thread not started.\n");
  }

  return PP_TRUE;
}

void NaBlackInstanceDestroy()
{
  // assumes you can't get a destroy call while you're inside a create call
  if ( ( g_MainThreadStatus.m_Status == kMainStatusStarting ) ||
       ( g_MainThreadStatus.m_Status == kMainStatusRunning  ) ||
       ( g_MainThreadStatus.m_Status == kMainStatusComplete ) )
  {
    (void)pthread_join( g_MainThread, NULL );
  }

  NaBlueCommandShutdown();
}

void NaBlackDidChangeView( PP_Resource view )
{
  // printf("Instance_DidChangeView\n");
}

void  NaBlackDidChangeFocus( PP_Bool has_focus )
{
  // printf("Instance_DidChangeFocus\n");
}

PP_Bool NaBlackHandleDocumentLoad( PP_Resource url_loader )
{
  return PP_TRUE;
}

void NaBlackHandleMessage( PP_Var message )
{
}

void NaBlackMouseLockLost()
{
}

void NaBlackRenderFrame()
{
  // check if main thread still running
  if ( g_MainThreadStatus.m_Status == kMainStatusComplete )
  {
    // pthread_join( g_MainThread, NULL );
    g_MainThreadStatus.m_Status = kMainStatusClosed;
  }

  FlushCommands();
}

// --------------------------------------------------------------------------
// Thread entry point to call application main(2)
// --------------------------------------------------------------------------

void* main_entry(void* parm)
{
  (void)parm;

  g_MainThreadStatus.m_Status = kMainStatusRunning;
  g_MainThreadStatus.m_Result = nablue_main( 0, NULL );
  g_MainThreadStatus.m_Status = kMainStatusComplete;

  return NULL;
}

// --------------------------------------------------------------------------
// Wrapped POSIX calls. 
// - Called from application threads
// - Queue commands to be processed from the main thread, when possible.
// - Priority is given to functionality that provides easier porting 
// - Someone needs to design a new OS standard that doesn't have all these
//   goddamn return values which makes it impossible to design a real 
//   concurrent model.
// --------------------------------------------------------------------------

int __wrap_write(int fd, void* buf, size_t count)
{
  // Aquire lock to let SwapBuffers know we're still writing. See #note-01
  // The only time this should ever block is if we're actually in the middle of SwapBuffers()
  pthread_rwlock_rdlock( &s_NaBlueCommandWriteLock );

  size_t         packet_size = sizeof( NaBlueWriteCommand ) + count;
  NaBlueCommand* command     = NaBlueCommandAllocate( kNaBlueCommandWrite, packet_size  );

  if ( !command )
  {
    pthread_rwlock_unlock( &s_NaBlueCommandWriteLock );
    return (0);
  }

  NaBlueWriteCommand* write_command = (NaBlueWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
  char*               write_buffer  = ( (char*) write_command ) + sizeof( NaBlueWriteCommand );

  write_command->m_Fd    = fd;
  write_command->m_Count = count;

  memcpy( write_buffer, buf, count );

  pthread_rwlock_unlock( &s_NaBlueCommandWriteLock );

  return ( count );
}

void FlushCommands(void)
{
  size_t         command_offset = 0;
  NaBlueCommand* command;
  int            command_count = 0;

  while ( (command = NaBlueCommandGetNext( command_offset )) )
  {
    command_offset += sizeof( NaBlueCommand ) + command->m_Size;
    command_count++;

    switch ( command->m_Command )
    {
      case kNaBlueCommandWrite:
      {
        NaBlueWriteCommand* write_command = (NaBlueWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        char*               write_buffer  = ( (char*) write_command ) + sizeof( NaBlueWriteCommand );

        if ( write_command->m_Fd == fileno(stdout) )
        {
          BrowserSysCallWrite( "/dev/stdout", write_buffer, write_command->m_Count );
        } 
        else if ( write_command->m_Fd == fileno(stderr) )
        {
          BrowserSysCallWrite( "/dev/stderr", write_buffer, write_command->m_Count );
        }
        else /* fd should have come from the browser via wrapped open(), so it's aware of it */
        {
          int  dev_len = sprintf( NULL, "/dev/fd/%d", write_command->m_Fd );
          char dev[ dev_len+1 ]; 
    
          sprintf( dev, "/dev/fd/%d", write_command->m_Fd );
          
          BrowserSysCallWrite( dev, write_buffer, write_command->m_Count );
        }
      }
      break;
    }
  }

  NaBlueCommandSwapBuffers();
}

// --------------------------------------------------------------------------
// Selected syscalls re-routed to the browser
//   - Yes, it definitely could be thought of as a crazy-backward OS kernel.
//   - These calls can only be called from the NaCl thread.
//   - Internal use only!
// --------------------------------------------------------------------------

void BrowserSysCallWrite( const char* dev, const char* buffer, size_t size )
{
  int  encoded_len = strnurlencode( NULL, buffer, size );
  char message[ encoded_len+1 ];

  strnurlencode( message, buffer, encoded_len+1 );
  message[ encoded_len ] = 0;

  NaBlackMessagingPostPrintf( "{ \"func\": \"write\", \"dev\": \"%s\", \"data\": \"%s\", \"len\": %d }", dev, message, encoded_len );
}

// --------------------------------------------------------------------------
// Utility functions 
//   - Additional functions used both internally and provided to application
// --------------------------------------------------------------------------

/* Converts a hex character to its integer value */
static char from_hex(char ch) {
  return isdigit((int)ch) ? ch - '0' : tolower((int)ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
static char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

/* Returns a url-encoded version of str */
int strnurlencode( char* dest, const char* str, size_t max_len ) 
{
  const char* pstr = str;
  char        buf[ max_len * 3 + 1 ];
  char*       pbuf = buf;
  int         len;

  while ((*pstr) && ( (uintptr_t)(pstr) - (uintptr_t)(str) < (max_len-1) ))
  {
    if (isalnum((int)*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
    {
      *pbuf++ = *pstr;
    }
    else 
    {
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    }
    pstr++;
  }
  *pbuf = '\0';

  len = (uintptr_t)(pbuf)-(uintptr_t)buf;

  if ( dest )
  {
    strncpy( dest, buf, (len<max_len)?len:max_len );
  }

  return len;
}

/* Returns a url-decoded version of str */
int strnurldecode( char* dest, const char* str, size_t max_len ) 
{
  const char* pstr = str;
  char        buf[ max_len + 1 ];
  char*       pbuf = buf;
  int         len;

  while ((*pstr) && ( (uintptr_t)(pstr) - (uintptr_t)(str) < (max_len-1) ))
  {
    if (*pstr == '%') 
    {
      if (pstr[1] && pstr[2]) 
      {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } 
    else 
    {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';

  len = (uintptr_t)(pbuf)-(uintptr_t)buf;

  if ( dest )
  {
    strncpy( dest, buf, (len<max_len)?len:max_len );
  }

  return len;
}

uint32_t next_pow2_u32(uint32_t x)
{
  x -= 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  
  return x + 1;
}

uint64_t next_pow2_u64(uint64_t x)
{
  x -= 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);
  
  return x + 1;
}
