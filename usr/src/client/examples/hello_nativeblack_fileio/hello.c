#include <nativeblack.h>
#include <string.h>

// File pipeline
// 1. FileSystemCreate (Temp or Persistent)
// 2. FileSystemOpen
// 3. FileSystemOpen result
// 4. FileRefCreate; FileIOCrate
// 5. FileIOOpen
// 6. FileIOOpen result
// 7. FileIOWrite
// 8. FileIOWrite result
// 9. [ Repeat FileIOWrite if not complete ]
// 10. FileIOClose


PP_Resource g_TempFileSystem;
PP_Resource g_LocalFileSystem;

// fd =
PP_Resource g_HelloFileRef;
PP_Resource g_HelloFileIO;
char        g_HelloBuffer[] = "Hello, World!";
int32_t     g_HelloPos;
int32_t     g_HelloFileSize;

enum
{
  kTempFileSystemSize  = ( 512 * 1024 * 1024ULL ),
  kLocalFileSystemSize = ( 8 * 1024 * 1024 * 1024ULL )
};

void TempFileSystemOpen( void* user_data, int32_t result )
{
  NaBlackMessagingPostPrintf( "TempFileSystemOpen result=%d\n",result);
}

void HelloFileWrite( void* user_data, int32_t result )
{
  NaBlackMessagingPostPrintf( "HelloFileWrite result=%d\n",result);
  
  if ( result == 0 )
  {
    NaBlackMessagingPostPrintf( "HelloFileWrite Complete.\n");

    NaBlackFileIOClose( g_HelloFileIO );
    NaBlackMessagingPostPrintf( "HelloFileWrite Close.\n");
    return; 
  }
  
  if ( result < 0 )
  {
    NaBlackMessagingPostPrintf( "HelloFileWrite Error\n");
    return; 
  }
  
  g_HelloPos += result;

  if ( g_HelloPos < g_HelloFileSize )
  {
    NaBlackMessagingPostPrintf( "HelloFileWrite Continue\n");

    PP_CompletionCallback hello_write_callback = { .func = HelloFileWrite, .user_data = NULL, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
    int32_t               hello_write_status   = NaBlackFileIOWrite( g_HelloFileIO, 0, g_HelloBuffer, g_HelloFileSize-g_HelloPos, hello_write_callback );

    NaBlackMessagingPostPrintf( "NaBlackFileIOWrite hello status=%d\n",hello_write_status);
    return;
  }
  
  NaBlackMessagingPostPrintf( "HelloFileWrite Complete\n");
  NaBlackFileIOClose( g_HelloFileIO );
  NaBlackMessagingPostPrintf( "HelloFileWrite Close.\n");
}

void HelloFileOpen( void* user_data, int32_t result )
{
  NaBlackMessagingPostPrintf( "HelloFileOpen result=%d\n",result);

  if ( result == 0 )
  {
    g_HelloPos      = 0;
    g_HelloFileSize = strlen(g_HelloBuffer)+1;

    PP_CompletionCallback hello_write_callback = { .func = HelloFileWrite, .user_data = NULL, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
    int32_t               hello_write_status   = NaBlackFileIOWrite( g_HelloFileIO, 0, g_HelloBuffer, g_HelloFileSize, hello_write_callback );

    NaBlackMessagingPostPrintf( "NaBlackFileIOWrite hello status=%d\n",hello_write_status);
  }
}

void LocalFileSystemOpen( void* user_data, int32_t result )
{
  NaBlackMessagingPostPrintf( "LocalFileSystemOpen result=%d\n",result);

  if ( result == 0 )
  {

    g_HelloFileRef = NaBlackFileRefCreate( g_LocalFileSystem, "/hello.txt" );
    g_HelloFileIO  = NaBlackFileIOCreate();

    int32_t               hello_open_flags    = PP_FILEOPENFLAG_CREATE | PP_FILEOPENFLAG_TRUNCATE | PP_FILEOPENFLAG_READ | PP_FILEOPENFLAG_WRITE;
    PP_CompletionCallback hello_open_callback = { .func = HelloFileOpen, .user_data = NULL, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
    int32_t               hello_open_status   = NaBlackFileIOOpen( g_HelloFileIO, g_HelloFileRef, hello_open_flags, hello_open_callback );

    NaBlackMessagingPostPrintf( "NaBlackFileIOOpen hello status=%d\n",hello_open_status);
  }
}


PP_Bool NaBlackHandleInputEvent( PP_Resource input_event )
{
  return PP_TRUE;
}

PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] )
{
  NaBlackMessagingPostPrintf( "Hello, world!" );

  g_TempFileSystem  = NaBlackFileSystemCreate( PP_FILESYSTEMTYPE_LOCALTEMPORARY );
  g_LocalFileSystem = NaBlackFileSystemCreate( PP_FILESYSTEMTYPE_LOCALPERSISTENT );

  PP_CompletionCallback temp_open_callback  = { .func = TempFileSystemOpen, .user_data = NULL, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  PP_CompletionCallback local_open_callback = { .func = LocalFileSystemOpen, .user_data = NULL, .flags = PP_COMPLETIONCALLBACK_FLAG_NONE };
  int32_t               temp_open_status    = NaBlackFileSystemOpen( g_TempFileSystem, kTempFileSystemSize, temp_open_callback );
  int32_t               local_open_status   = NaBlackFileSystemOpen( g_LocalFileSystem, kLocalFileSystemSize, local_open_callback );

  NaBlackMessagingPostPrintf( "TempFileSystemOpen status=%d\n",temp_open_status);
  NaBlackMessagingPostPrintf( "LocalFileSystemOpen status=%d\n",local_open_status);

  if ( temp_open_status != PP_OK_COMPLETIONPENDING )
  {
    NaBlackMessagingPostPrintf( "Could not open TempFileSystem\n");
  }

  if ( local_open_status != PP_OK_COMPLETIONPENDING )
  {
    NaBlackMessagingPostPrintf( "Could not open LocalFileSystem\n");
  }

  return PP_TRUE;
}

void NaBlackInstanceDestroy()
{
}

void NaBlackDidChangeView( PP_Resource view )
{
}

void  NaBlackDidChangeFocus( PP_Bool has_focus )
{
}

PP_Bool NaBlackHandleDocumentLoad( PP_Resource url_loader )
{
  return PP_TRUE;
}

void NaBlackHandleMessage( PP_Var message )
{
}

void NaBlackMouseLockLost()
{
}

void NaBlackRenderFrame()
{
}
