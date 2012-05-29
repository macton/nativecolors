#include <string.h>
#include <stdio.h>
#include <alloca.h>
#include <ctype.h>
#include <nativeblack.h>
#include "nativeblue_private.h"

// Q: Why queue up all the PPAPI calls installed of CallOnMainThread?
//   - See: "CallonMainThread kills performance" https://groups.google.com/d/msg/native-client-discuss/IhdJ-fAre8g/8pOPxzo5_DcJ
//     "Hi, so CallOnMainThread() invoked off the main thread in its current implementation involves multiple RPCs.  It might be ok to invoke 
//      a few times per frame at 60hz.  As the OP found, wrapping OpenGL calls (of which there might be thousands or tens of thousands of 
//      calls per frame) with CallOnMainThread() will have a significant impact on performance.  It wasn't intended to be used this way.  
//      We're working on ways to make PPAPI & OpenGL usable off the main thread in NaCl, but I can't put a timeline on when that will land."

static NaBlueCommand* CommandGetNext( size_t offset );
static void           CommandSwapBuffers( void );

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

static char*             s_CommandScratch[2]       = { NULL, NULL };
static size_t            s_CommandScratchOffset[2] = { 0,    0    };
static size_t            s_CommandScratchSize[2]   = { 0,    0    };
static int               s_CommandIndex            = 0;

// --------------------------------------------------------------------------
// s_CommandAllocLock: 
//   - Since other (unknown) threads can be allocating from the same pool, they
//     must lock each other out. 
//   - #todo per-thread pool.
//   - However, the FlushCommands thread does not need to aquire this lock, it just
//     needs to make sure the threads can't be writing when it swaps the buffer.
//   - See #note-01 below.
// --------------------------------------------------------------------------

static pthread_mutex_t   s_CommandAllocLock        = PTHREAD_MUTEX_INITIALIZER;

// --------------------------------------------------------------------------
// s_CommandWriteLock may need some clarification: #note-01
//   - Some poeple might say this "an abuse" of the rwlock. I say it was just mis-named.
//   - Alternatively, you could think of it as a rwlock on s_CommandIndex
//   - No writer thread needs to lock any other writer thread out. Since they're already
//     constrained by the allocation to be working in separate, unrelated memory.
//   - However, the FlushCommands thread can't swap buffers until all the writes are 
//     ready.
//   - So:
//     1) the reader (NaBlueCommandSwapBuffers) must aquire a write lock to make sure they
//        are complete.
//     2) the writers (threads generating commands) each aquire a read lock while writing.
//     3) This will ensure that all writes are complete before swap.
//   - #todo s_CommandWriteLock can be reduced to a single-writer/single-reader
//           "lock" per writing thread. 
//           - But SwapBuffers may still need to spin for lack of work while waiting for
//             the last write from the last thread (potentially)
// --------------------------------------------------------------------------

static pthread_rwlock_t  s_CommandWriteLock      = PTHREAD_RWLOCK_INITIALIZER;

int NaBlueCommandStartup( void )
{
  size_t scratch_size = NaBlueDefaultScratchSize();

  s_CommandScratch[0]     = NaBlueAlloc( scratch_size );
  s_CommandScratchSize[0] = scratch_size;

  if ( s_CommandScratch[0] == NULL )
  {
    return (0);
  }

  s_CommandScratch[1]     = NaBlueAlloc( scratch_size );
  s_CommandScratchSize[1] = scratch_size;

  if ( s_CommandScratch[1] == NULL )
  {
    NaBlueFree( s_CommandScratch[0] );
    return (0);
  }

  s_CommandIndex = 0;

  return (1);
}

void NaBlueCommandShutdown( void )
{
  NaBlueFree( s_CommandScratch[0] );
  NaBlueFree( s_CommandScratch[1] );

  s_CommandScratchSize[0]   = 0;
  s_CommandScratchSize[1]   = 0;
  s_CommandScratchOffset[0] = 0;
  s_CommandScratchOffset[1] = 0;
}

// Note: You need to lock s_CommandLock before calling this. 
//       And don't unlock until the memory has been filled and is ready.
NaBlueCommand* NaBlueCommandAllocate( int cmd, size_t size )
{
  pthread_mutex_lock(&s_CommandAllocLock);

  size_t alloc_size = sizeof( NaBlueCommand ) + size;

  int    scratch_index     = s_CommandIndex;
  size_t scratch_size      = s_CommandScratchSize[ scratch_index ];
  size_t scratch_used      = s_CommandScratchOffset[ scratch_index ];
  size_t scratch_available = scratch_size - scratch_used;

  if ( alloc_size > scratch_available )
  {
    // One of two things will happen in this case:
    //   (1) Realloc the buffer (raise high-water mark) and continue
    //   (2) Realloc fails (fixed memory condition); command silently dropped. Assume application is signalled by having overridden NaBlueRealloc.

    size_t scratch_needed    = scratch_used + alloc_size;
    size_t scratch_requested = NaBlueReallocSizeRoundUp( scratch_needed );
    char*  scratch_current   = s_CommandScratch[ scratch_index ];
    char*  scratch_new       = NaBlueRealloc( scratch_current, scratch_requested );

    if ( !scratch_new )
    {
      pthread_mutex_unlock(&s_CommandAllocLock);
      return NULL; 
    }

    s_CommandScratch[ scratch_index ]     = scratch_new;
    s_CommandScratchSize[ scratch_index ] = scratch_requested;

    scratch_size      = scratch_requested;
    scratch_available = scratch_size - scratch_used;
  }

  char*           scratch = s_CommandScratch[ scratch_index ] + scratch_used;
  NaBlueCommand*  command = (NaBlueCommand*)scratch;
  
  command->m_Command = cmd;
  command->m_Size    = size;
  
  s_CommandScratchOffset[ scratch_index ] += alloc_size;

  pthread_mutex_unlock(&s_CommandAllocLock);

  return ( command );
}

void CommandSwapBuffers( void )
{
  // Aquire a *write* lock to ensure that all threads are finished writing. See #note-01
  pthread_rwlock_wrlock( &s_CommandWriteLock );

  // Note s_CommandAllocLock doesn't need to be aquired since it's guaranteed to only 
  // be referring to the buffer that's currently being written to by the writing threads.
  //   - At this point there are none, guaranteed by the above.

  // #todo s_CommandWriteLock could be, in principal, double buffered:
  //       - The read lock could be aquired as soon as the index is flipped
  //       - So make sure there's a fence that guaratees that happens after the offset clear.
  //       - The writers could be holding the lock from the previous buffer in that case
  //         (before this index swap), so that would have to be resolved.

  s_CommandScratchOffset[ s_CommandIndex^1 ] = 0;

  // ...would need a fence here... see above

  s_CommandIndex ^= 1;

  // Alow writing threads to continue...
  pthread_rwlock_unlock( &s_CommandWriteLock );
}

void NaBlueCommandWriteLock( void )
{
  pthread_rwlock_rdlock( &s_CommandWriteLock );
}

void NaBlueCommandWriteUnlock( void )
{
  pthread_rwlock_unlock( &s_CommandWriteLock );
}

NaBlueCommand* CommandGetNext( size_t offset )
{
  int back_index = s_CommandIndex ^ 1;
  int back_used  = s_CommandScratchOffset[ back_index ];

  if ( offset < back_used )
  {
    return (NaBlueCommand*)( s_CommandScratch[ back_index ] + offset );
  }
  return NULL;
}

// #todo Just generate the code that does the appropriate thing directly in the command buffer.
// Reference: http://people.csail.mit.edu/jansel/papers/2011pldi-nacljit-slides.pdf
//            http://code.google.com/p/minnacl/source/browse/tests/dynamic_code_loading/dynamic_modify_test.c

void NaBlueFlushCommands(void)
{
  int32_t  local_file_system_state = NaBlueGetLocalFileSystemState();
  int32_t  temp_file_system_state  = NaBlueGetTempFileSystemState();

  // Wait for file system to finish initializing
  if ( local_file_system_state == kNaBlueFileSystemStateInitializing )
  {
    return;
  }

  // Wait for file system to finish initializing
  if ( temp_file_system_state == kNaBlueFileSystemStateInitializing )
  {
    return;
  }

  size_t         command_offset = 0;
  NaBlueCommand* command;
  int            command_count = 0;

  while ( (command = CommandGetNext( command_offset )) )
  {
    command_offset += sizeof( NaBlueCommand ) + command->m_Size;
    command_count++;

    switch ( command->m_Command )
    {
      // --------------------------------------------------------------------------
      // Wrapped POSIX calls. 
      // - Called from application threads
      // - Queue commands to be processed from the main thread, when possible.
      // - Priority is given to functionality that provides easier porting 
      // --------------------------------------------------------------------------

      case kNaBlueCommandBrowserWrite:
      {
        NaBlueBrowserWriteCommand* write_command = (NaBlueBrowserWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        char*                      write_buffer  = ( (char*) write_command ) + sizeof( NaBlueBrowserWriteCommand );
        NaBlueBrowserWrite( write_command->dev, write_buffer, write_command->count );
      }
      break;

      case kNaBlueCommandWrite:
      {
        NaBlueWriteCommand* write_command = (NaBlueWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        char*               write_buffer  = ( (char*) write_command ) + sizeof( NaBlueWriteCommand );
        NaBlueFileWrite( write_command->fd, write_buffer, write_command->count );
      }
      break;

      case kNaBlueCommandRead:
      {
        NaBlueReadCommand* read_command = (NaBlueReadCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        NaBlueFileRead( read_command->fd, read_command->buf, read_command->count );
      }
      break;

      case kNaBlueCommandOpen:
      {
        NaBlueOpenCommand* open_command = (NaBlueOpenCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        NaBlueFileOpen( open_command->fd );
      }
      break;

      case kNaBlueCommandMkDir:
      {
        NaBlueMkDirCommand* mkdir_command = (NaBlueMkDirCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        NaBlueFileMkDir( mkdir_command->fd );
      }
      break;

      case kNaBlueCommandClose:
      {
        NaBlueCloseCommand* close_command = (NaBlueCloseCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        NaBlueFileClose( close_command->fd );
      }
      break;

      case kNaBlueCommandFlush:
      {
        NaBlueFlushCommand* flush_command = (NaBlueFlushCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
        NaBlueFileFlush( flush_command->fd );
      }
      break;
    }
  }

  CommandSwapBuffers();
}

