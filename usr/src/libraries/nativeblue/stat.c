#include "nativeblue_private.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

int __wrap_stat(const char* path, struct stat* buf)
{
  int fd = open(path,O_RDONLY);
  if ( fd < 0 )
  {
    errno = ENOENT;
    return (-1);
  } 

  NaBlueFileDescription* file  = NaBlueGetFileDescription( fd );

  memset( buf, 0, sizeof( struct stat ) );
  buf->st_size  = file->fileInfo.size;
  buf->st_atime = (time_t)file->fileInfo.last_access_time;
  buf->st_mtime = (time_t)file->fileInfo.last_modified_time;

  // Note: ctime isn't exactly creation_time, but there isn't (yet) the equivalent of inode info so, close enough.
  // - PP_Time PP_FileInfo::creation_time
  //   This value represents the creation time of the file.
  // - The field st_ctime is changed by writing or by setting inode information (i.e., owner, group, link count, mode, etc.).

  buf->st_ctime = (time_t)file->fileInfo.creation_time;

  close(fd);
  return (0);
}
