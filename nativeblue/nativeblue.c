#define _MULTI_THREADED

#include <nativeblack/nativeblack.h>
#include "nativeblue.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
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

void FlushWrite(void);

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
static pthread_mutex_t   s_NaBlueCommandLock             = PTHREAD_MUTEX_INITIALIZER;

static int s_DebugWriteCount = 0;

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

NaBlueCommand* NaBlueCommandAllocate( int cmd, size_t size )
{
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

  return ( command );
}

void NaBlueCommandSwapBuffers( void )
{
  pthread_mutex_lock(&s_NaBlueCommandLock);

  s_NaBlueCommandIndex ^= 1;
  s_NaBlueCommandScratchOffset[ s_NaBlueCommandIndex ] = 0;

  pthread_mutex_unlock(&s_NaBlueCommandLock);
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

  FlushWrite();
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
  pthread_mutex_lock(&s_NaBlueCommandLock);

  size_t         packet_size = sizeof( NaBlueWriteCommand ) + count;
  NaBlueCommand* command     = NaBlueCommandAllocate( kNaBlueCommandWrite, packet_size );

  if ( !command )
  {
    pthread_mutex_unlock(&s_NaBlueCommandLock);
    return (0);
  }

  NaBlueWriteCommand* write_command = (NaBlueWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
  char*               write_buffer  = ( (char*) write_command ) + sizeof( NaBlueWriteCommand );

  write_command->m_Fd    = fd;
  write_command->m_Count = count;

  memcpy( write_buffer, buf, count );

  s_DebugWriteCount++;
  pthread_mutex_unlock(&s_NaBlueCommandLock);

  return ( count );
}

void FlushWrite(void)
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
