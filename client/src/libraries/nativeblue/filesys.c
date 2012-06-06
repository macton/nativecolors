#include <nativeblack.h>
#include <string.h>
#include "nativeblue_private.h"

// See also: File System Explorer https://chrome.google.com/webstore/detail/nhnjmpbdkieehidddbaeajffijockaea

static PP_Resource     s_TempFileSystem;
static PP_Resource     s_LocalFileSystem;
static int             s_TempFileSystemState;
static int             s_LocalFileSystemState;

static void FileSystemOpen( void* user_data, int32_t result )
{
  *(int*)user_data = kNaBlueFileSystemStateReady;
}

// Only call from NaCl main thread
int NaBlueFileSystemStartup(void)
{
  NaBlueFdStartup();
  NaBlueSetCwd("/");

  s_TempFileSystem       = NaBlackFileSystemCreate( PP_FILESYSTEMTYPE_LOCALTEMPORARY );
  s_LocalFileSystem      = NaBlackFileSystemCreate( PP_FILESYSTEMTYPE_LOCALPERSISTENT );
  s_TempFileSystemState  = kNaBlueFileSystemStateInitializing;
  s_LocalFileSystemState = kNaBlueFileSystemStateInitializing;

  PP_CompletionCallback temp_open_callback  = { .func = FileSystemOpen, .user_data = (void*)&s_TempFileSystemState,  .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  PP_CompletionCallback local_open_callback = { .func = FileSystemOpen, .user_data = (void*)&s_LocalFileSystemState, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t               temp_open_status    = NaBlackFileSystemOpen( s_TempFileSystem,  kTempFileSystemSize,  temp_open_callback );
  int32_t               local_open_status   = NaBlackFileSystemOpen( s_LocalFileSystem, kLocalFileSystemSize, local_open_callback );

  if ( temp_open_status != PP_OK_COMPLETIONPENDING )
  {
    s_TempFileSystemState  = kNaBlueFileSystemStateError;
    return (0);
  }

  if ( local_open_status != PP_OK_COMPLETIONPENDING )
  {
    s_LocalFileSystemState  = kNaBlueFileSystemStateError;
    return (0);
  }

  return (1);
}

// Only call from NaCl main thread
void NaBlueFileSystemShutdown(void)
{
  NaBlueFdShutdown();
}

PP_Resource NaBlueGetLocalFileSystem( void )
{
  return (s_LocalFileSystem);
}

PP_Resource NaBlueGetTempFileSystem( void )
{
  return (s_TempFileSystem);
}

int32_t NaBlueGetLocalFileSystemState( void )
{
  return (s_LocalFileSystemState);
}

int32_t NaBlueGetTempFileSystemState( void )
{
  return (s_TempFileSystemState);
}

