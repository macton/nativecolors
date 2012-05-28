#include <string.h>
#include <alloca.h>
#include "nativeblue_private.h"

static ssize_t BrowserQueueWrite( const char* dev, void* buf, size_t count );
static ssize_t LocalQueueWrite( int fd, void* buf, size_t count );
static void    FileWriteComplete( void* user_data, int32_t result );

ssize_t __wrap_write(int fd, void* buf, size_t count)
{
  int            is_stdout       = ( fd == fileno(stdout) );
  int            is_stderr       = ( fd == fileno(stderr) );
  int            is_browser_file = is_stdout || is_stderr;

  if ( is_browser_file )
  {
    char*  dev = ( is_stdout ) ? "/dev/stdout" : "/dev/stderr";
    return BrowserQueueWrite( dev, buf, count );
  }

  return LocalQueueWrite( fd, buf, count );
}

ssize_t BrowserQueueWrite( const char* dev, void* buf, size_t count )
{
  NaBlueCommandWriteLock();

  size_t         packet_size     = sizeof( NaBlueBrowserWriteCommand ) + count;
  NaBlueCommand* command         = NaBlueCommandAllocate( kNaBlueCommandBrowserWrite, packet_size  );
  ssize_t        result          = -1;

  if ( command )
  {
    NaBlueBrowserWriteCommand* write_command = (NaBlueBrowserWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
    char*                      write_buffer  = ( (char*) write_command ) + sizeof( NaBlueBrowserWriteCommand );

    strncpy( write_command->dev, dev, kNaBlueMaxPath );
    write_command->dev[kNaBlueMaxPath] = 0;

    write_command->count = count;
    memcpy( write_buffer, buf, count );

    // No blocking. Just lie and tell the calling function that's it all okay.
    result = count;
  }

  NaBlueCommandWriteUnlock();
  return ( result );
}

void NaBlueBrowserWrite( const char* dev, const char* buffer, size_t size )
{
  int   encoded_len = strnurlencode( NULL, 0, buffer, size );
  char* encoded_str = (char*)alloca( encoded_len+1 );

  strnurlencode( encoded_str, encoded_len+1, buffer, size );
  
  NaBlackMessagingPostPrintf( "{ \"func\": \"write\", \"dev\": \"%s\", \"data\": \"%s\", \"len\": %d }", dev, encoded_str, encoded_len );
}

ssize_t LocalQueueWrite( int fd, void* buf, size_t count )
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  file->state = kNaBlueFileStateWriting; // Fix the state before before enter command queue

  NaBlueCommandWriteLock();

  size_t         packet_size     = sizeof( NaBlueWriteCommand ) + count;
  NaBlueCommand* command         = NaBlueCommandAllocate( kNaBlueCommandWrite, packet_size  );

  if ( command == NULL ) 
  {
    NaBlueCommandWriteUnlock();
    return (-1);
  }

  NaBlueWriteCommand* write_command = (NaBlueWriteCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
  char*               write_buffer  = ( (char*) write_command ) + sizeof( NaBlueWriteCommand );

  write_command->fd    = fd;
  write_command->count = count;
  memcpy( write_buffer, buf, count );

  NaBlueCommandWriteUnlock();

  // Wait for NaBlueFileWriteComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    return (count);  
  }
 
  return ( -1 );
}

void NaBlueFileWrite( int fd, const char* buffer, size_t size )
{
  NaBlueFileDescription* file              = NaBlueGetFileDescription( fd );
  PP_Resource            file_io           = file->fileIO;

  PP_CompletionCallback  write_callback    = { .func = FileWriteComplete, .user_data = (void*)file, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t                write_status      = NaBlackFileIOWrite( file_io, file->pos, buffer, size, write_callback );

  file->targetPos = file->pos + (int64_t)size;
  file->buf.write = buffer;

  if ( write_status != PP_OK_COMPLETIONPENDING )
  {
    file->status = write_status;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}

void FileWriteComplete( void* user_data, int32_t result )
{
  NaBlueFileDescription* file = (NaBlueFileDescription*)user_data;

  if ( result == 0 )
  {
    // Under what conditions would this happen?
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

  file->pos += result;

  if ( file->pos < file->targetPos )
  {
    NaBlueFileWrite( file->fd, file->buf.write + result, file->targetPos - file->pos );
    return;
  }

  // Complete;.
  __sync_synchronize();
  file->state = kNaBlueFileStateReady;
}
