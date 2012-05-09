#include <nativeblack/nativeblack.h>

// --------------------------------------------------------------------------
// Required entry point functions -------------------------------------------
// --------------------------------------------------------------------------

PP_Bool NaBlackHandleInputEvent( PP_Resource input_event )
{
  return PP_TRUE;
}

PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] )
{
  NaBlackMessagingPostPrintf( "Hello, world!" );
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
