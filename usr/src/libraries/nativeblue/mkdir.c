#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include "nativeblue_private.h"

static void FileMkDirComplete( void* user_data, int32_t result );

int mkdir(const char *pathname, mode_t mode)
{
  int fd = NaBlueAllocFd();

  if ( fd < 0 )
  {
    return (-1);
  }

  NaBlueFileDescription* file  = NaBlueGetFileDescription( fd );

  if ( pathname[0] == '/' )
  {
    strncpy( file->path, pathname, kNaBlueMaxPath );
    file->path[ kNaBlueMaxPath ] = 0;
  }
  else
  {
    const char* cwd     = NaBlueGetCwd();
    int         cwd_len = strlen( cwd );

    strncpy( file->path, cwd, kNaBlueMaxPath ); 
    strncpy( file->path + cwd_len, pathname, kNaBlueMaxPath - cwd_len );
    file->path[ kNaBlueMaxPath ] = 0;
  }

  file->flags = 0;
  file->pos   = 0;
  file->fd    = fd;

  //
  // Add command to queue
  //
  file->state = kNaBlueFileStateMakingDirectory; // Fix the state before before enter command queue

  NaBlueCommandWriteLock();

  size_t         packet_size = sizeof( NaBlueMkDirCommand );
  NaBlueCommand* command     = NaBlueCommandAllocate( kNaBlueCommandMkDir, packet_size  );

  if ( command )
  {
    NaBlueMkDirCommand* mkdir_command = (NaBlueMkDirCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
    mkdir_command->fd = fd;
  }

  NaBlueCommandWriteUnlock();

  // Wait for FileMkDirComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    close(fd);
    return (0);  
  }

  close(fd);
  return ( -1 );
}

void FileMkDirComplete( void* user_data, int32_t result )
{
  NaBlueFileDescription* file = (NaBlueFileDescription*)user_data;

  if ( result == 0 )
  {
    __sync_synchronize();
    file->state  = kNaBlueFileStateReady;
  }
  else
  {
    file->status = result;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}

// Called from NaCl main thread. Called from command queue.
void NaBlueFileMkDir( int fd )
{
  PP_Resource            local_file_system = NaBlueGetLocalFileSystem();
  NaBlueFileDescription* file              = NaBlueGetFileDescription( fd );
  PP_Resource            file_ref;
  PP_Resource            file_io;

  file_ref = NaBlackFileRefCreate( local_file_system, file->path );
  if (!NaBlackFileRefIsFileRef( file_ref ))
  {
    // Probably not a good path. e.g. "hello.txt" not "/hello.txt"
    file->status = PP_ERROR_BADRESOURCE;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
    return;
  }

  // This FileIO is only created to make close non-exceptional.
  // If this case comes up more often, might need a special flag to handle releasing more granularly.
  file_io = NaBlackFileIOCreate(); 

  file->fileIO    = file_io;
  file->fileRef   = file_ref;

  PP_CompletionCallback mkdir_callback = { .func = FileMkDirComplete, .user_data = (void*)file, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t               mkdir_status   = NaBlackFileRefMakeDirectory( file->fileRef, PP_TRUE, mkdir_callback );

  if ( mkdir_status != PP_OK_COMPLETIONPENDING )
  {
    file->status = mkdir_status;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}
