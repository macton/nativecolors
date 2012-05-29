#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include "nativeblue_private.h"

static void FileFlushComplete( void* user_data, int32_t result );

int __wrap_fsync(const char *path, int oflag, ... )
{
  int fd = NaBlueAllocFd();

  if ( fd < 0 )
  {
    return (-1);
  }

  NaBlueFileDescription* file  = NaBlueGetFileDescription( fd );

  //
  // Add command to queue
  //
  file->state = kNaBlueFileStateFlushing; // Fix the state before before enter command queue

  NaBlueCommandWriteLock();

  size_t         packet_size = sizeof( NaBlueFlushCommand );
  NaBlueCommand* command     = NaBlueCommandAllocate( kNaBlueCommandFlush, packet_size  );

  if ( command )
  {
    NaBlueFlushCommand* fsync_command = (NaBlueFlushCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
    fsync_command->fd = fd;
  }

  NaBlueCommandWriteUnlock();

  // Wait for FileFlushComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    close(fd);
    return (0);
  }

  close(fd);
  return (-1);
}

void FileFlushComplete( void* user_data, int32_t result )
{
  NaBlueFileDescription* file = (NaBlueFileDescription*)user_data;

  if ( result == 0 )
  {
    __sync_synchronize();
    file->state = kNaBlueFileStateReady;
  }
  else
  {
    file->status = result;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}

// Called from NaCl main thread. Called from command queue.
void NaBlueFileFlush( int fd )
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  PP_CompletionCallback fsync_callback = { .func = FileFlushComplete, .user_data = (void*)file, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t               fsync_status   = NaBlackFileIOFlush( file->fileIO, fsync_callback );

  if ( fsync_status != PP_OK_COMPLETIONPENDING )
  {
    file->status = fsync_status;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}
