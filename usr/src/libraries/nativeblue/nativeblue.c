#include <string.h>
#include <stdio.h>
#include <alloca.h>
#include <ctype.h>
#include <cJSON.h>
#include <nativeblack.h>
#include "nativeblue_private.h"

int    nablue_main(int argc, char **argv);
void*  main_entry(void *parm);
int    __wrap_write(int, void *, size_t);

enum
{
  kMainStatusStarting,
  kMainStatusRunning,
  kMainStatusComplete,
  kMainStatusClosed,
  kMainStatusError,
  kMainStatusUnknown
};

struct MainThreadStatus 
{
  int m_Status;
  int m_Result;
};

typedef struct MainThreadStatus MainThreadStatus ;

pthread_t         g_MainThread;
MainThreadStatus  g_MainThreadStatus;

// --------------------------------------------------------------------------
// Required entry point functions -------------------------------------------
// --------------------------------------------------------------------------

PP_Bool NaBlackHandleInputEvent( PP_Resource input_event )
{
  return PP_TRUE;
}

PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] )
{
  if ( !NaBlueCommandStartup() )
  {
    return PP_FALSE;
  }

  if ( !NaBlueFileSystemStartup() )
  {
    return PP_FALSE;
  }

  // By default stdout and stderr are unbuffered
  setvbuf( stdout, NULL, _IONBF, 0 );  
  setvbuf( stderr, NULL, _IONBF, 0 );  

  g_MainThreadStatus.m_Status = kMainStatusStarting;
  int main_start_result       = pthread_create(&g_MainThread, NULL, main_entry, NULL);
  if ( main_start_result != 0 )
  {
    g_MainThreadStatus.m_Status = kMainStatusError;
    fprintf(stderr,"ERROR: main thread not started.\n");
  }

  return PP_TRUE;
}

void NaBlackInstanceDestroy()
{
  // assumes you can't get a destroy call while you're inside a create call
  if ( ( g_MainThreadStatus.m_Status == kMainStatusStarting ) ||
       ( g_MainThreadStatus.m_Status == kMainStatusRunning  ) ||
       ( g_MainThreadStatus.m_Status == kMainStatusComplete ) )
  {
    (void)pthread_join( g_MainThread, NULL );
  }

  NaBlueCommandShutdown();
  NaBlueFileSystemShutdown();
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
  uint32_t    var_len;
  const char* var = NaBlackVarVarToUtf8( message, &var_len );

  if ( var_len == 0 )
  {
    return;
  }

  cJSON* json = cJSON_Parse( var );

  if ( json == NULL )
  {
    return;
  }

  cJSON* func = cJSON_GetObjectItem( json, "func" );

  if ( func->type == cJSON_String )
  {
    if ( strcmp( func->valuestring, "TTYResize" ) == 0 ) 
    {
      cJSON* width  = cJSON_GetObjectItem( json, "width"  );
      cJSON* height = cJSON_GetObjectItem( json, "height" );

      NaBlueTTYResize( width->valueint, height->valueint );
    }
  }

  cJSON_Delete( json );
}

void NaBlackMouseLockLost()
{
}

void NaBlackRenderFrame()
{
  // check if main thread still running
  if ( g_MainThreadStatus.m_Status == kMainStatusComplete )
  {
    // pthread_join( g_MainThread, NULL );
    g_MainThreadStatus.m_Status = kMainStatusClosed;
  }

  NaBlueFlushCommands();
}

// --------------------------------------------------------------------------
// Thread entry point to call application main(2)
// --------------------------------------------------------------------------

void* main_entry(void* parm)
{
  (void)parm;

  g_MainThreadStatus.m_Status = kMainStatusRunning;
  g_MainThreadStatus.m_Result = nablue_main( 0, NULL );
  g_MainThreadStatus.m_Status = kMainStatusComplete;

  return NULL;
}
