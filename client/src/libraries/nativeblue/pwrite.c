#include <string.h>
#include "nativeblue_private.h"

ssize_t __wrap_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  file->rw_count = 0;
  file->state    = kNaBlueFileStateWriting; // Fix the state before before enter command queue
  file->savedPos = file->pos;
  file->pos      = offset;

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

  // Wait for FileWriteComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    file->pos = file->savedPos;
    return (file->rw_count);
  }
 
  return ( -1 );
}
