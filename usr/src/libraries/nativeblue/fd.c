#include <string.h>
#include "nativeblue_private.h"

// See also: http://en.wikipedia.org/wiki/File_descriptor

static NaBlueFileDescription s_FileDescriptions[ kNaBlueMaxOpenFileDescriptionCount ];
static pthread_mutex_t       s_FileDescriptionAllocLock = PTHREAD_MUTEX_INITIALIZER;

static void NaBlueForceAllocFd( int fd );

// Called from NaCl main thread
void NaBlueFdStartup( void )
{
  memset( s_FileDescriptions, 0, sizeof(NaBlueFileDescription) * kNaBlueMaxOpenFileDescriptionCount );
  
  // Force Alloc

  NaBlueForceAllocFd( fileno(stdin)  ); // 0 Standard Input  (stdin)
  NaBlueForceAllocFd( fileno(stdout) ); // 1 Standard Output (stdout)
  NaBlueForceAllocFd( fileno(stderr) ); // 2 Standard Error  (stderr)
}

// Called only from NaCl main thread
void NaBlueFdShutdown( void )
{
  for (int i=0;i<kNaBlueMaxOpenFileDescriptionCount;i++)
  {
    NaBlueFreeFd(i);
  }
}

void NaBlueForceAllocFd( int fd )
{
  if (( fd < 0 ) || ( fd >= kNaBlueMaxOpenFileDescriptionCount ))
  {
    return;
  }
  
  s_FileDescriptions[fd].state = kNaBlueFileStateAllocated;
}

// Called from the/any calling thread before the command is enqueued.
int NaBlueAllocFd( void )
{
  int fd = -1;
  pthread_mutex_lock( &s_FileDescriptionAllocLock );
  for (int i=0;i<kNaBlueMaxOpenFileDescriptionCount;i++)
  {
    if ( s_FileDescriptions[i].state == kNaBlueFileStateFree )
    {
      s_FileDescriptions[i].state = kNaBlueFileStateAllocated;
      fd = i;
      break;
    }
  }
  pthread_mutex_unlock( &s_FileDescriptionAllocLock );
  return (fd);
}

// Called only from NaCl main thread
void NaBlueFreeFd( int fd )
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );
  if ( file )
  {
    if ( file->state > kNaBlueFileStateAllocated )
    {
      // Calling release on failed allocation will cause module to crash 
      NaBlackCoreReleaseResource( file->fileIO );
      NaBlackCoreReleaseResource( file->fileRef );
    }

    __sync_synchronize();
    file->state = kNaBlueFileStateFree;
  }
}

NaBlueFileDescription* NaBlueGetFileDescription( int fd )
{
  if (( fd < 0 ) || ( fd >= kNaBlueMaxOpenFileDescriptionCount ))
  {
    return NULL;
  }
  return &s_FileDescriptions[fd];
}

int NaBlueWaitFileReady( int fd )
{
  NaBlueFileDescription* file       = NaBlueGetFileDescription( fd );
  int32_t*               file_state = &file->state;

  for (;;)
  {
    int32_t file_ready   = __sync_fetch_and_add( file_state, 0 ); 
    if ( file_ready == kNaBlueFileStateReady )
    {
      return (1);
    }
    if ( file_ready == kNaBlueFileStateError )
    {
      return (0);
    }
    NaBlueSleepLong();
  }
}
