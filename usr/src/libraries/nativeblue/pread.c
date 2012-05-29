#include "nativeblue_private.h"

ssize_t __wrap_pread(int fd, void *buf, size_t count, off_t offset)
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  file->rw_count = 0;
  file->state    = kNaBlueFileStateReading; // Fix the state before before enter command queue
  file->savedPos = file->pos;
  file->pos      = offset;

  NaBlueCommandWriteLock();

  size_t         packet_size     = sizeof( NaBlueReadCommand ) + count;
  NaBlueCommand* command         = NaBlueCommandAllocate( kNaBlueCommandRead, packet_size  );

  if ( command == NULL ) 
  {
    NaBlueCommandWriteUnlock();
    return (-1);
  }

  NaBlueReadCommand* read_command = (NaBlueReadCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );

  read_command->fd     = fd;
  read_command->count  = count;
  read_command->buf    = (char*)buf;

  NaBlueCommandWriteUnlock();

  // Wait for FileReadComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    file->pos = file->savedPos; // Reset pos. The file offset is not changed.
    return (file->rw_count);
  }
 
  return ( -1 );
}
