
#define NATIVE_BLACK_C_INLINE

#include "nativeblack.h"

#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <alloca.h>
#include <stdio.h>

// --------------------------------------------------------------------------------------------
// Global instance (There can be only one!)
// --------------------------------------------------------------------------------------------

PP_Instance                      g_NaBlackInstanceId;
int32_t                          g_NaBlackViewWidth;
int32_t                          g_NaBlackViewHeight;
PP_Resource                      g_NaBlackGraphics3dId;
int32_t                          g_NaBlackGraphics3dLastSwapResult;

// --------------------------------------------------------------------------------------------
// PPB Interface globals
// --------------------------------------------------------------------------------------------

const PPB_Audio*                 g_NaBlackAudio;                  // (5) functions
const PPB_AudioConfig*           g_NaBlackAudioConfig;            // (5) functions
const PPB_Core*                  g_NaBlackCore;                   // (6) functions
const PPB_FileIO*                g_NaBlackFileIO;                 // (10) functions
const PPB_FileRef*               g_NaBlackFileRef;                // (10) functions
const PPB_FileSystem*            g_NaBlackFileSystem;             // (4) functions
const PPB_Fullscreen*            g_NaBlackFullscreen;             // (3) functions
const PPB_Graphics2D*            g_NaBlackGraphics2D;             // (7) functions
const PPB_Graphics3D*            g_NaBlackGraphics3D;             // (8) functions
const PPB_ImageData*             g_NaBlackImageData;              // (7) functions
const PPB_InputEvent*            g_NaBlackInputEvent;             // (7) functions
const PPB_Instance*              g_NaBlackInstance;               // (2) functions
const PPB_KeyboardInputEvent*    g_NaBlackKeyboardInputEvent;     // (4) functions
const PPB_Messaging*             g_NaBlackMessaging;              // (1) functions
const PPB_MouseInputEvent*       g_NaBlackMouseInputEvent;        // (6) functions
const PPB_MouseLock*             g_NaBlackMouseLock;              // (2) functions
const PPB_OpenGLES2*             g_NaBlackOpenGLES2;              // (142) functions
const PPB_URLLoader*             g_NaBlackURLLoader;              // (10) functions
const PPB_URLRequestInfo*        g_NaBlackURLRequestInfo;         // (5) functions
const PPB_URLResponseInfo*       g_NaBlackURLResponseInfo;        // (3) functions
const PPB_Var*                   g_NaBlackVar;                    // (4) functions
const PPB_VarArrayBuffer*        g_NaBlackVarArrayBuffer;         // (4) functions
const PPB_View*                  g_NaBlackView;                   // (6) functions
const PPB_WebSocket*             g_NaBlackWebSocket;              // (14) functions
const PPB_WheelInputEvent*       g_NaBlackWheelInputEvent;        // (5) functions

// --------------------------------------------------------------------------------------------
// Internal functions
// --------------------------------------------------------------------------------------------
static void PPP_RenderLoop( void* user_data, int32_t result );

// --------------------------------------------------------------------------------------------
// Application overridable
//  If you want to assign different attributes:
//  CCFLAGS += -Xlinker --wrap -Xlinker PPP_Graphics3D_GetAttributes
// --------------------------------------------------------------------------------------------

int32_t* PPP_Graphics3D_GetAttributes( int32_t width, int32_t height )
{
  enum
  {
    kAttribWidthIndex  = 11,
    kAttribHeightIndex = 13,
  };

  enum
  {
    kNaBlackGraphicsAttribAlphaSize     = 8,
    kNaBlackGraphicsAttribDepthSize     = 24,
    kNaBlackGraphicsAttribStencilSize   = 0,
    kNaBlackGraphicsAttribSamples       = 0,
    kNaBlackGraphicsAttribSampleBuffers = 0,
  };

  static int32_t attribs[] = 
  {
    PP_GRAPHICS3DATTRIB_ALPHA_SIZE,           kNaBlackGraphicsAttribAlphaSize,
    PP_GRAPHICS3DATTRIB_DEPTH_SIZE,           kNaBlackGraphicsAttribDepthSize,
    PP_GRAPHICS3DATTRIB_STENCIL_SIZE,         kNaBlackGraphicsAttribStencilSize,
    PP_GRAPHICS3DATTRIB_SAMPLES,              kNaBlackGraphicsAttribSamples,
    PP_GRAPHICS3DATTRIB_SAMPLE_BUFFERS,       kNaBlackGraphicsAttribSampleBuffers,
    PP_GRAPHICS3DATTRIB_WIDTH,                0,
    PP_GRAPHICS3DATTRIB_HEIGHT,               0,
    PP_GRAPHICS3DATTRIB_NONE
  };

  attribs[ kAttribWidthIndex  ] = width;
  attribs[ kAttribHeightIndex ] = height;
  
  return attribs;
}

// --------------------------------------------------------------------------------------------
// PPP_Instance entry points
// --------------------------------------------------------------------------------------------

PP_Bool PPP_Instance_DidCreate(PP_Instance instance, uint32_t argc, const char* argn[], const char* argv[]) 
{
  if ( g_NaBlackInstanceId != kNaBlackInvalidInstance )
  {
    return PP_FALSE; // Only one instance allowed.
  }

  g_NaBlackInstanceId               = instance;
  g_NaBlackViewWidth                = 0;
  g_NaBlackViewHeight               = 0;
  g_NaBlackGraphics3dId             = kNaBlackInvalidResource;
  g_NaBlackGraphics3dLastSwapResult = 0;

  // --------------------------------------------------------------------------
  // Request the event types that this instance will recieve. (ALL)
  // --------------------------------------------------------------------------

  NaBlackInputEventRequestInputEvents ( PP_INPUTEVENT_CLASS_MOUSE | PP_INPUTEVENT_CLASS_KEYBOARD | PP_INPUTEVENT_CLASS_WHEEL | PP_INPUTEVENT_CLASS_IME );


  return NaBlackInstanceCreate( argc, argn, argv );
}

void PPP_Instance_DidDestroy(PP_Instance instance) 
{
  assert(instance == g_NaBlackInstanceId);

  NaBlackInstanceDestroy();

  g_NaBlackInstanceId = kNaBlackInvalidInstance;

  if ( g_NaBlackGraphics3dId != kNaBlackInvalidResource ) 
  {
    NaBlackCoreReleaseResource( g_NaBlackGraphics3dId );
    g_NaBlackGraphics3dId = kNaBlackInvalidResource;
  }
}

void PPP_Instance_DidChangeView(PP_Instance instance, PP_Resource view_resource)
{
  assert(instance == g_NaBlackInstanceId);

  PP_Rect position;
  PP_Rect clip;

  if ( !NaBlackViewGetRect( view_resource, &position ) )
  {
    return;
  }

  if ( !NaBlackViewGetClipRect( view_resource, &clip ) )
  {
    return;
  }

  if ( g_NaBlackGraphics3dId == kNaBlackInvalidResource )
  {
    int32_t* attribs       = PPP_Graphics3D_GetAttributes( position.size.width, position.size.height );

    g_NaBlackGraphics3dId  = NaBlackGraphics3DCreate( kNaBlackInvalidResource, attribs );

    int32_t success        = NaBlackInstanceBindGraphics( g_NaBlackGraphics3dId );

    if (success == PP_TRUE)  
    {
      PP_CompletionCallback cc = PP_MakeCompletionCallback( PPP_RenderLoop, NULL );
      NaBlackCoreCallOnMainThread(0, cc, PP_OK);
    }
  } 
  else if ((position.size.width != g_NaBlackViewWidth) || (position.size.height != g_NaBlackViewHeight))
  {
    NaBlackGraphics3DResizeBuffers( g_NaBlackGraphics3dId, position.size.width, position.size.height );
  }

  g_NaBlackViewWidth  = position.size.width;
  g_NaBlackViewHeight = position.size.height;

  NaBlackDidChangeView( view_resource );
}

void PPP_Instance_DidChangeFocus(PP_Instance instance, PP_Bool has_focus) 
{
  assert(instance == g_NaBlackInstanceId);
  NaBlackDidChangeFocus( has_focus );
}

PP_Bool PPP_Instance_HandleDocumentLoad(PP_Instance instance, PP_Resource url_loader) 
{
  assert(instance == g_NaBlackInstanceId);
  return NaBlackHandleDocumentLoad( url_loader );
}

// --------------------------------------------------------------------------------------------
// PPP_InputEvent entry points
// --------------------------------------------------------------------------------------------

PP_Bool PPP_InputEvent_HandleInputEvent( PP_Instance instance, PP_Resource input_event )
{
  assert(instance == g_NaBlackInstanceId);
  return NaBlackHandleInputEvent( input_event );
}

// --------------------------------------------------------------------------------------------
// PPP_Messaging entry points
// --------------------------------------------------------------------------------------------

void PPP_Messaging_HandleMessage( PP_Instance instance, PP_Var message )
{
  assert(instance == g_NaBlackInstanceId);

  if ( message.type == PP_VARTYPE_STRING )
  {
    uint32_t    message_len;
    const char* message_str = NaBlackVarVarToUtf8( message, &message_len );
    float*      float_addr;
    float       float_value;
    int32_t*    int32_addr;
    int32_t     int32_value;

    int  match_float = sscanf( message_str, "$poke_float %p %f", (void**)&float_addr, &float_value );
    if ( match_float )
    {
      *float_addr = float_value;
      NaBlackMessagingPostPrintf( "(float*)%p = %g", float_addr, float_value);
      return;
    }

    int  match_int32 = sscanf( message_str, "$poke_int32 %p %d", (void**)&int32_addr, &int32_value );
    if ( match_int32 )
    {
      *int32_addr = int32_value;
      NaBlackMessagingPostPrintf( "(int32_t*)%p = %d", int32_addr, int32_value);
      return;
    }
  }

  NaBlackHandleMessage( message );
}

// --------------------------------------------------------------------------------------------
// PPP_MouseLock entry points
// --------------------------------------------------------------------------------------------

void PPP_MouseLock_MouseLockLost( PP_Instance instance )
{
  assert(instance == g_NaBlackInstanceId);
  NaBlackMouseLockLost();
}

// --------------------------------------------------------------------------------------------
// Module entry points
// --------------------------------------------------------------------------------------------

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module, PPB_GetInterface get_browser_interface) 
{
  if (!get_browser_interface)
  {
    return -1;
  }

  g_NaBlackInstanceId            = kNaBlackInvalidInstance;
  g_NaBlackAudio                 = (const PPB_Audio*)               get_browser_interface(PPB_AUDIO_INTERFACE);
  g_NaBlackAudioConfig           = (const PPB_AudioConfig*)         get_browser_interface(PPB_AUDIO_CONFIG_INTERFACE);
  g_NaBlackCore                  = (const PPB_Core*)                get_browser_interface(PPB_CORE_INTERFACE);
  g_NaBlackFileIO                = (const PPB_FileIO*)              get_browser_interface(PPB_FILEIO_INTERFACE);
  g_NaBlackFileRef               = (const PPB_FileRef*)             get_browser_interface(PPB_FILEREF_INTERFACE);
  g_NaBlackFileSystem            = (const PPB_FileSystem*)          get_browser_interface(PPB_FILESYSTEM_INTERFACE);
  g_NaBlackFullscreen            = (const PPB_Fullscreen*)          get_browser_interface(PPB_FULLSCREEN_INTERFACE);
  g_NaBlackGraphics2D            = (const PPB_Graphics2D*)          get_browser_interface(PPB_GRAPHICS_2D_INTERFACE);
  g_NaBlackGraphics3D            = (const PPB_Graphics3D*)          get_browser_interface(PPB_GRAPHICS_3D_INTERFACE);
  g_NaBlackImageData             = (const PPB_ImageData*)           get_browser_interface(PPB_IMAGEDATA_INTERFACE);
  g_NaBlackInputEvent            = (const PPB_InputEvent*)          get_browser_interface(PPB_INPUT_EVENT_INTERFACE);
  g_NaBlackInstance              = (const PPB_Instance*)            get_browser_interface(PPB_INSTANCE_INTERFACE);
  g_NaBlackKeyboardInputEvent    = (const PPB_KeyboardInputEvent*)  get_browser_interface(PPB_KEYBOARD_INPUT_EVENT_INTERFACE);
  g_NaBlackMessaging             = (const PPB_Messaging*)           get_browser_interface(PPB_MESSAGING_INTERFACE);
  g_NaBlackMouseInputEvent       = (const PPB_MouseInputEvent*)     get_browser_interface(PPB_MOUSE_INPUT_EVENT_INTERFACE);
  g_NaBlackMouseLock             = (const PPB_MouseLock*)           get_browser_interface(PPB_MOUSELOCK_INTERFACE);
  g_NaBlackOpenGLES2             = (const PPB_OpenGLES2*)           get_browser_interface(PPB_OPENGLES2_INTERFACE);
  g_NaBlackURLLoader             = (const PPB_URLLoader*)           get_browser_interface(PPB_URLLOADER_INTERFACE);
  g_NaBlackURLRequestInfo        = (const PPB_URLRequestInfo*)      get_browser_interface(PPB_URLREQUESTINFO_INTERFACE);
  g_NaBlackURLResponseInfo       = (const PPB_URLResponseInfo*)     get_browser_interface(PPB_URLRESPONSEINFO_INTERFACE);
  g_NaBlackVar                   = (const PPB_Var*)                 get_browser_interface(PPB_VAR_INTERFACE);
  g_NaBlackVarArrayBuffer        = (const PPB_VarArrayBuffer*)      get_browser_interface(PPB_VAR_ARRAY_BUFFER_INTERFACE);
  g_NaBlackView                  = (const PPB_View*)                get_browser_interface(PPB_VIEW_INTERFACE);
  g_NaBlackWebSocket             = (const PPB_WebSocket*)           get_browser_interface(PPB_WEBSOCKET_INTERFACE);
  g_NaBlackWheelInputEvent       = (const PPB_WheelInputEvent*)     get_browser_interface(PPB_WHEEL_INPUT_EVENT_INTERFACE);

  if ( (!g_NaBlackInstance)
    || (!g_NaBlackAudio)
    || (!g_NaBlackAudioConfig)
    || (!g_NaBlackCore)
    || (!g_NaBlackFileIO)
    || (!g_NaBlackFileRef)
    || (!g_NaBlackFileSystem)
    || (!g_NaBlackFullscreen)
    || (!g_NaBlackGraphics2D)
    || (!g_NaBlackGraphics3D)
    || (!g_NaBlackImageData)
    || (!g_NaBlackInputEvent)
    || (!g_NaBlackInstance)
    || (!g_NaBlackKeyboardInputEvent)
    || (!g_NaBlackMessaging)
    || (!g_NaBlackMouseInputEvent)
    || (!g_NaBlackMouseLock)
    || (!g_NaBlackOpenGLES2)
    || (!g_NaBlackURLLoader)
    || (!g_NaBlackURLRequestInfo)
    || (!g_NaBlackURLResponseInfo)
    || (!g_NaBlackVar)
    || (!g_NaBlackVarArrayBuffer)
    || (!g_NaBlackView)
    || (!g_NaBlackWebSocket)
    || (!g_NaBlackWheelInputEvent) )
  {
    return -1;
  }

  // Initialize gl w/ browser interface
  if (!glInitializePPAPI(get_browser_interface)) 
  {
    return -1;
  }

  return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule() 
{
  glTerminatePPAPI();
}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name) 
{
  static PPP_Instance instance_interface = 
  {
    &PPP_Instance_DidCreate,
    &PPP_Instance_DidDestroy,
    &PPP_Instance_DidChangeView,
    &PPP_Instance_DidChangeFocus,
    &PPP_Instance_HandleDocumentLoad
  };

  static PPP_InputEvent input_event_interface =
  {
    &PPP_InputEvent_HandleInputEvent
  };

  static PPP_Messaging messaging_interface =
  {
    &PPP_Messaging_HandleMessage
  };

  static PPP_MouseLock mouse_lock_lost_interface = 
  {
    &PPP_MouseLock_MouseLockLost
  };

  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0)
  {
    return &instance_interface;
  }

  if (strcmp(interface_name, PPP_INPUT_EVENT_INTERFACE) == 0)
  {
    return &input_event_interface;
  }

  if (strcmp(interface_name, PPP_MESSAGING_INTERFACE) == 0)
  {
    return &messaging_interface;
  }

  if (strcmp(interface_name, PPP_MOUSELOCK_INTERFACE) == 0)
  {
    return &mouse_lock_lost_interface;
  }

  return NULL;
}

// --------------------------------------------------------------------------------------------
// Internal functions
// --------------------------------------------------------------------------------------------

void PPP_RenderLoop( void* user_data, int32_t result ) 
{
  glSetCurrentContextPPAPI( g_NaBlackGraphics3dId );

  NaBlackRenderFrame();

  glSetCurrentContextPPAPI(0);

  PP_CompletionCallback cc  = PP_MakeCompletionCallback( PPP_RenderLoop, NULL );

  g_NaBlackGraphics3dLastSwapResult = NaBlackGraphics3DSwapBuffers( g_NaBlackGraphics3dId, cc );
}


// --------------------------------------------------------------------------------------------
// Utility functions (not part of API)
// --------------------------------------------------------------------------------------------

// void NaBlackMessagingPostUtf8(const char* msg)
// Also see: Yuriy O'Donnell's ArcadeShooter NaCl Postmortem
//     http://www.kayru.org/articles/arcadeshooter-nacl-postmortem/
//     http://www.kayru.org/articles/arcadeshooter-nacl-postmortem/index.html.txt
//     http://www.kayru.org/articles/arcadeshooter-nacl-postmortem/minimal_nacl_gles2.cpp.txt

void NaBlackMessagingPostUtf8(const char* msg)
{
  PP_Var msg_var = NaBlackVarVarFromUtf8( msg, strlen(msg) );
  NaBlackMessagingPostMessage(msg_var);
  NaBlackVarRelease(msg_var);
}

void NaBlackMessagingPostPrintf( const char* format, ... )
{
  int     length;
  va_list args;
  char*   buffer;

  va_start( args, format );
  length = vsnprintf( NULL, 0, format, args );
  va_end( args );

  buffer = alloca( length + 1 );

  if ( buffer ) 
  {
    va_start( args, format );
    vsnprintf( buffer, length+1, format, args );
    va_end( args );

    NaBlackMessagingPostUtf8( buffer );
  }
}
