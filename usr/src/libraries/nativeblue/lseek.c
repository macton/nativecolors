#include <errno.h>
#include "nativeblue_private.h"

int __wrap_lseek(int fd, int offset, int whence) 
{
  NaBlueFileDescription* file = NaBlueGetFileDescription( fd );

  if ( file == NULL )
  {
    errno = EBADF;
    return ((off_t)-1);
  }

  switch ( whence )
  {
    case SEEK_SET:
    {
      file->pos = offset;
      return (off_t)( file->pos );
    }
    break;

    case SEEK_CUR:
    {
      file->pos += offset;
      return (off_t)( file->pos );
    }
    break;

    case SEEK_END:
    {
      file->pos = file->fileInfo.size + offset;
      return (off_t)( file->pos );
    }
    break;

    default:
    {
      errno = EINVAL;  
      return ((off_t)-1);
    }
    break;
  }
}
