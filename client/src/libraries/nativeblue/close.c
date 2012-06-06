#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <errno.h>
#include "nativeblue_private.h"

int __wrap_close( int fd )
{
  NaBlueFileDescription* file  = NaBlueGetFileDescription( fd );

  if ( file == NULL )
  {
    return (EBADF);
  }

  //
  // Add command to queue
  //

  NaBlueCommandWriteLock();

  size_t         packet_size = sizeof( NaBlueCloseCommand );
  NaBlueCommand* command     = NaBlueCommandAllocate( kNaBlueCommandClose, packet_size  );

  if ( command )
  {
    NaBlueCloseCommand* close_command = (NaBlueCloseCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
    close_command->fd = fd;
  }

  NaBlueCommandWriteUnlock();

  // - No handle to keep in case of error
  // - No notification in case of error during deferred close
  // #todo something about that.

  return (0);
}

// Called from NaCl main thread. Called from command queue.
void NaBlueFileClose( int fd )
{
  // void(* PPB_FileIO_1_0::Close)(PP_Resource file_io)
  // ny pending callbacks will still run, reporting PP_ERROR_ABORTED if pending IO was interrupted. 
  // It is not valid to call Open() again after a call to this method. Note: If the FileIO object is destroyed, 
  // and it is still open, then it will be implicitly closed, so you are not required to call Close().

  NaBlueFreeFd( fd );
}
