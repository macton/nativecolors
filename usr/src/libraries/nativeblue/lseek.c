#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "nativeblue_private.h"

off_t __wrap_lseek(int fd, off_t offset, int whence) 
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  off_t result = -1;

  if ( file == NULL )
  {
    errno = EBADF;
    result =  ((off_t)-1);
  }

  switch ( whence )
  {
    case SEEK_SET:
    {
      file->pos = offset;
      result =  (off_t)( file->pos );
    }
    break;

    case SEEK_CUR:
    {
      file->pos += offset;
      result =  (off_t)( file->pos );
    }
    break;

    case SEEK_END:
    {
      file->pos = file->fileInfo.size + offset;
      result =  (off_t)( file->pos );
    }
    break;

    default:
    {
      errno = EINVAL;  
      result =  ((off_t)-1);
    }
    break;
  }

  return (result);
}
