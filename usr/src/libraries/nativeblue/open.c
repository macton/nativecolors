#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include "nativeblue_private.h"

static void FileOpenComplete( void* user_data, int32_t result );
static void FileQueryComplete( void* user_data, int32_t result );

int __wrap_open(const char *path, int oflag, ... )
{
  int fd = NaBlueAllocFd();

  if ( fd < 0 )
  {
    return (-1);
  }

  NaBlueFileDescription* file  = NaBlueGetFileDescription( fd );

  if ( path[0] == '/' )
  {
    strncpy( file->path, path, kNaBlueMaxPath );
    file->path[ kNaBlueMaxPath ] = 0;
  }
  else
  {
    const char* cwd     = NaBlueGetCwd();
    int         cwd_len = strlen( cwd );

    strncpy( file->path, cwd, kNaBlueMaxPath ); 
    strncpy( file->path + cwd_len, path, kNaBlueMaxPath - cwd_len );
    file->path[ kNaBlueMaxPath ] = 0;
  }

  file->flags = 0;
  file->pos   = 0;
  file->fd    = fd;

  // PP_FILEOPENFLAG_READ 	
  // Requests read access to a file.

  // O_RDONLY
  // Open for reading only.

  if ( oflag & O_RDONLY )
  {
    file->flags |= PP_FILEOPENFLAG_READ;
  }

  // PP_FILEOPENFLAG_WRITE 	
  // Requests write access to a file.
  // May be combined with PP_FILEOPENFLAG_READ to request read and write access.

  // O_WRONLY
  // Open for writing only.
  // O_RDWR
  // Open for reading and writing. The result is undefined if this flag is applied to a FIFO.

  if ( oflag & O_WRONLY )
  {
    file->flags |= PP_FILEOPENFLAG_WRITE;
  }

  if ( oflag & O_RDWR )
  {
    file->flags |= PP_FILEOPENFLAG_WRITE | PP_FILEOPENFLAG_READ;
  }

  // PP_FILEOPENFLAG_CREATE 	
  // Requests that the file be created if it does not exist.
  // If the file already exists, then this flag is ignored unless PP_FILEOPENFLAG_EXCLUSIVE was also specified, in which case FileIO::Open() will fail.

  // O_CREAT
  // If the file exists, this flag has no effect except as noted under O_EXCL below. Otherwise, the file shall be created; the user ID of the file shall
  // be set to the effective user ID of the process; the group ID of the file shall be set to the group ID of the file's parent directory or to the 
  // effective group ID of the process; and the access permission bits (see <sys/stat.h>) of the file mode shall be set to the value of the third argument 
  // taken as type mode_t modified as follows: a bitwise AND is performed on the file-mode bits and the corresponding bits in the complement of the 
  // process' file mode creation mask. Thus, all bits in the file mode whose corresponding bit in the file mode creation mask is set are cleared. 
  // When bits other than the file permission bits are set, the effect is unspecified. The third argument does not affect whether the file is open for 
  // reading, writing, or for both. Implementations shall provide a way to initialize the file's group ID to the group ID of the parent directory. 
  // Implementations may, but need not, provide an implementation-defined way to initialize the file's group ID to the effective group ID of the calling process.

  if ( oflag & O_CREAT )
  {
    file->flags |= PP_FILEOPENFLAG_CREATE;
  }

  // PP_FILEOPENFLAG_TRUNCATE 	
  // Requests that the file be truncated to length 0 if it exists and is a regular file.
  // PP_FILEOPENFLAG_WRITE must also be specified.

  // O_TRUNC
  // If the file exists and is a regular file, and the file is successfully opened O_RDWR or O_WRONLY, its length shall be truncated to 0, and the 
  // mode and owner shall be unchanged. It shall have no effect on FIFO special files or terminal device files. Its effect on other file types is 
  // implementation-defined. The result of using O_TRUNC with O_RDONLY is undefined.

  if ( oflag & O_TRUNC )
  {
    file->flags |= PP_FILEOPENFLAG_TRUNCATE;
  }

  // PP_FILEOPENFLAG_EXCLUSIVE 	
  // Requests that the file is created when this flag is combined with PP_FILEOPENFLAG_CREATE.
  // If this flag is specified, and the file already exists, then the FileIO::Open() call will fail.

  // O_EXCL
  // If O_CREAT and O_EXCL are set, open() shall fail if the file exists. The check for the existence of the file and the creation of the file if it does 
  // not exist shall be atomic with respect to other threads executing open() naming the same filename in the same directory with O_EXCL and O_CREAT set. 
  // If O_EXCL and O_CREAT are set, and path names a symbolic link, open() shall fail and set errno to [EEXIST], regardless of the contents of the symbolic 
  // link. If O_EXCL is set and O_CREAT is not set, the result is undefined.

  if ( oflag & O_EXCL )
  {
    file->flags |= PP_FILEOPENFLAG_EXCLUSIVE;
  }

  //
  // Add command to queue
  //
  file->state = kNaBlueFileStateOpening; // Fix the state before before enter command queue

  NaBlueCommandWriteLock();

  size_t         packet_size = sizeof( NaBlueOpenCommand );
  NaBlueCommand* command     = NaBlueCommandAllocate( kNaBlueCommandOpen, packet_size  );

  if ( command )
  {
    NaBlueOpenCommand* open_command = (NaBlueOpenCommand*) ( ( (char*)command ) + sizeof( NaBlueCommand ) );
    open_command->fd = fd;
  }

  NaBlueCommandWriteUnlock();

  // Wait for FileOpenComplete()
  if ( NaBlueWaitFileReady( fd ) )
  {
    return (fd);  
  }

  NaBlueFreeFd(fd);
  return ( -1 );
}

void FileQueryComplete( void* user_data, int32_t result )
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

void FileOpenComplete( void* user_data, int32_t result )
{
  NaBlueFileDescription* file = (NaBlueFileDescription*)user_data;

  if ( result == 0 )
  {
    PP_CompletionCallback query_callback = { .func = FileQueryComplete, .user_data = (void*)file, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
    int32_t               query_status   = NaBlackFileIOQuery( file->fileIO, &file->fileInfo, query_callback );
  
    if ( query_status != PP_OK_COMPLETIONPENDING )
    {
      file->status = query_status;
  
      __sync_synchronize();
      file->state  = kNaBlueFileStateError;
    }
  }
  else
  {
    file->status = result;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}

// Called from NaCl main thread. Called from command queue.
void NaBlueFileOpen( int fd )
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

  file_io = NaBlackFileIOCreate();
  if (!NaBlackFileIOIsFileIO( file_io ))
  {
    // Don't know what would cause this one.
    NaBlackCoreReleaseResource( file_ref );
    file->status = PP_ERROR_BADRESOURCE;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
    return;
  }

  file->fileRef   = file_ref;
  file->fileIO    = file_io;

  PP_CompletionCallback open_callback = { .func = FileOpenComplete, .user_data = (void*)file, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t               open_status   = NaBlackFileIOOpen( file->fileIO, file->fileRef, file->flags, open_callback );

  if ( open_status != PP_OK_COMPLETIONPENDING )
  {
    file->status = open_status;

    __sync_synchronize();
    file->state  = kNaBlueFileStateError;
  }
}
