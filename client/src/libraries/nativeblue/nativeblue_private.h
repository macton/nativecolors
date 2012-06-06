#pragma once

#include <nativeblack.h>
#include "nativeblue.h"
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>

enum
{
  kNaBlueMaxOpenFileDescriptionCount = 512,
  kNaBlueMaxPath                     = 512,
  kTempFileSystemSize                = 1UL * 1024 * 1024 * 1024,
  kLocalFileSystemSize               = 8UL * 1024 * 1024 * 1024
};

// --------------------------------------------------------------------------
// Shared command buffer for commands that need to run on the NaCl thread
// --------------------------------------------------------------------------

typedef struct NaBlueCommand             NaBlueCommand;
typedef struct NaBlueWriteCommand        NaBlueWriteCommand;
typedef struct NaBlueReadCommand         NaBlueReadCommand;
typedef struct NaBlueBrowserWriteCommand NaBlueBrowserWriteCommand;
typedef struct NaBlueOpenCommand         NaBlueOpenCommand;
typedef struct NaBlueMkDirCommand        NaBlueMkDirCommand;
typedef struct NaBlueCloseCommand        NaBlueCloseCommand;
typedef struct NaBlueFlushCommand        NaBlueFlushCommand;

enum
{
  kNaBlueCommandNull,
  kNaBlueCommandBrowserWrite,
  kNaBlueCommandWrite,
  kNaBlueCommandRead,
  kNaBlueCommandOpen,
  kNaBlueCommandMkDir,
  kNaBlueCommandClose,
  kNaBlueCommandFlush
};

struct NaBlueCommand
{
  int32_t m_Command;
  size_t  m_Size;
};

struct NaBlueBrowserWriteCommand
{
  char    dev[ kNaBlueMaxPath+1 ];
  size_t  count;
};

struct NaBlueWriteCommand
{
  int32_t fd;
  size_t  count;
};

struct NaBlueReadCommand
{
  int32_t fd;
  size_t  count;
  char*   buf;
};

struct NaBlueOpenCommand
{
  int     fd;
};

struct NaBlueMkDirCommand
{
  int     fd;
};

struct NaBlueCloseCommand
{
  int     fd;
};

struct NaBlueFlushCommand
{
  int     fd;
};

// Note: You need to call NaBlueCommandWriteLock before calling this. 
//       And don't unlock until the memory has been filled and is ready.

NaBlueCommand* NaBlueCommandAllocate( int cmd, size_t size );
void           NaBlueCommandWriteLock( void );
void           NaBlueCommandWriteUnlock( void );
int            NaBlueOpenCommandHandler( NaBlueCommand* command );

// --------------------------------------------------------------------------
// File System API wrapper
// --------------------------------------------------------------------------

typedef struct NaBlueFileDescription NaBlueFileDescription;

struct NaBlueFileDescription
{
  PP_Resource        fileRef;  
  PP_Resource        fileIO;
  PP_FileInfo        fileInfo;

  int64_t            pos;
  int64_t            savedPos;
  int64_t            targetPos;
  int64_t            rw_count;
  int32_t            state;
  int32_t            status;
  char               path[ kNaBlueMaxPath+1 ];
  int32_t            flags;
  int                fd;

  union
  {
    const char* write;
    char*       read;
  } buf;
};

enum
{
  kNaBlueFileStateFree,
  /* -- Anything after this point assumes fd is being used -- */
  kNaBlueFileStateError,
  kNaBlueFileStateAllocated,
  /* -- Anything after this point assumes resources (fileio/fileref) have been allocated -- */
  kNaBlueFileStateOpening,
  kNaBlueFileStateMakingDirectory,
  kNaBlueFileStateFlushing,
  kNaBlueFileStateReading,
  kNaBlueFileStateWriting,
  kNaBlueFileStateSeeking,
  kNaBlueFileStateReady
};

enum
{
  kNaBlueFileSystemStateInitializing,
  kNaBlueFileSystemStateReady,
  kNaBlueFileSystemStateError
};

void                   NaBlueFlushCommands(void);
int                    NaBlueCommandStartup( void );
void                   NaBlueCommandShutdown( void );
int                    NaBlueFileSystemStartup( void );
void                   NaBlueFileSystemShutdown( void );
void                   NaBlueFdStartup( void );
void                   NaBlueFdShutdown( void );
int                    NaBlueAllocFd( void );
void                   NaBlueFreeFd( int fd );
NaBlueFileDescription* NaBlueGetFileDescription( int fd );
int                    NaBlueWaitFileReady( int fd );
PP_Resource            NaBlueGetLocalFileSystem( void );
PP_Resource            NaBlueGetTempFileSystem( void );
int32_t                NaBlueGetLocalFileSystemState( void );
int32_t                NaBlueGetTempFileSystemState( void );
void                   NaBlueFileOpen( int fd );
void                   NaBlueFileMkDir( int fd );
void                   NaBlueBrowserWrite( const char* dev, const char* buffer, size_t size );
void                   NaBlueFileWrite( int fd, const char* buffer, size_t size );
void                   NaBlueFileRead( int fd, char* buffer, size_t size );
void                   NaBlueFileClose( int fd );
void                   NaBlueFileFlush( int fd );
void                   NaBlueSetCwd( const char* path );
const char*            NaBlueGetCwd( void );
void                   NaBlueTTYResize( int width, int height );
int                    NaBlueWaitTTYConnected( int fd );
int                    NaBlueTTYWidth( int fd );
int                    NaBlueTTYHeight( int fd );


