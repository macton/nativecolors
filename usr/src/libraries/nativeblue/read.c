#include <string.h>
#include <alloca.h>
#include "nativeblue_private.h"

static ssize_t BrowserQueueRead( const char* dev, void* buf, size_t count );
static ssize_t LocalQueueRead( int fd, void* buf, size_t count );
static void    FileReadComplete( void* user_data, int32_t result );

// Don't bother locking fd. There's no sensible use of multiple threads using fd without a higher level
// locking mechanism anyway.

ssize_t __wrap_read(int fd, void* buf, size_t count)
{
  int is_stdin = ( fd == fileno(stdin) );

  if ( is_stdin )
  {
    return BrowserQueueRead( "/dev/stdin", buf, count );
  }

  return LocalQueueRead( fd, buf, count );
}

ssize_t BrowserQueueRead( const char* dev, void* buf, size_t count )
{
  // #todo get stdin
  return (0);
}

ssize_t LocalQueueRead( int fd, void* buf, size_t count )
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  file->rw_count = 0;
  file->state    = kNaBlueFileStateReading; // Fix the state before before enter command queue

  NaBlueCommandWriteLock();

  size_t         packet_size     = sizeof( NaBlueReadCommand ) + count;
  NaBlueCommand* command         = NaBlueCommandAllocate( kNaBlueCommandRead, packet_size  );

  if ( command == NULL ) 
  {
    NaBlueCommandWriteUnlock();
    return (-1);
  }

  NaBlueReadCommand* read_command = (NaBlueReadCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );

  read_command->fd    = fd;
  read_command->count = count;
  read_command->buf   = (char*)buf;

  NaBlueCommandWriteUnlock();

  // Wait for NaBlueFileReadComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    return (file->rw_count);
  }
 
  return ( -1 );
}

void NaBlueFileRead( int fd, char* buffer, size_t size )
{
  NaBlueFileDescription* file              = NaBlueGetFileDescription( fd );
  PP_Resource            file_io           = file->fileIO;

  PP_CompletionCallback  read_callback    = { .func = FileReadComplete, .user_data = (void*)file, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t                read_status      = NaBlackFileIORead( file_io, file->pos, buffer, size, read_callback );

  file->targetPos = file->pos + (int64_t)size;
  file->buf.read  = buffer;

  if ( read_status != PP_OK_COMPLETIONPENDING )
  {
    file->status = read_status;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}

void FileReadComplete( void* user_data, int32_t result )
{
  NaBlueFileDescription* file = (NaBlueFileDescription*)user_data;

  if ( result == 0 )
  {
    // EOF
    __sync_synchronize();
    file->state = kNaBlueFileStateReady;
    return; 
  }

  if ( result < 0 )
  {
    // Under what conditions would this happen?
    file->status = result;
    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
    return; 
  }

  file->pos      += result;
  file->rw_count += result;

  if ( file->pos < file->targetPos )
  {
    NaBlueFileRead( file->fd, file->buf.read + result, file->targetPos - file->pos );
    return;
  }

  // Complete;.
  __sync_synchronize();
  file->state = kNaBlueFileStateReady;
}
