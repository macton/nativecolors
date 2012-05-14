#pragma once
// --------------------------------------------------------------------------------------------
// DO NOT EDIT: This is generated code.
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// Native Client (NaCl) single instance interface (Pepper 18)
// Mike Acton <macton@insomniacgames.com> <macton@gmail.com> @mike_acton
//
// This interface makes two assumptions for the common case:
//   - You only want one instance of a module. (*1)
//   - The NaCl API functions are all called from the same (main) thread (*2) (*3).
//
// (*1) See also: https://groups.google.com/d/msg/native-client-discuss/70hXg3AACeE/3Fau7EpCFUEJ
// (*2) Functions which add NaCl API calls to a queue to be called from the main thread could
//      also be generated to relieve this constraint. This is also the recommended approach from
//      Colt McAnlis @ Google. Check out his talk from #GDC12: http://youtu.be/R281PhQufHo
// (*3) With this constraint, most PP_Var conversion can be handled internally. #todo.
//
// #todo Also remove the OGL context params to make equivalent to gl_* calls.
//       ...or just remove them entirely since they're redundant.
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// Except for instance parameters, documentation corresponds to what's in the NaCl include files:
//   ~/nacl_sdk/pepper_18/toolchain/linux_x86_newlib/x86_64-nacl/include/ppapi/
//   Note: The documentation in those headers isn't totally accurate.
// --------------------------------------------------------------------------------------------
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_graphics_3d.h"
#include "ppapi/c/pp_input_event.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppp_graphics_3d.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/ppp_input_event.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_messaging.h"
#include "ppapi/c/ppp_mouse_lock.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/c/ppb_audio.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_file_io.h"
#include "ppapi/c/ppb_file_ref.h"
#include "ppapi/c/ppb_file_system.h"
#include "ppapi/c/ppb_fullscreen.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_graphics_3d.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_mouse_lock.h"
#include "ppapi/c/ppb_opengles2.h"
#include "ppapi/c/ppb_url_loader.h"
#include "ppapi/c/ppb_url_request_info.h"
#include "ppapi/c/ppb_url_response_info.h"
#include "ppapi/c/ppb_var_array_buffer.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppb_view.h"
#include "ppapi/c/ppb_websocket.h"
#include "ppapi/gles2/gl2ext_ppapi.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// --------------------------------------------------------------------------------------------
// Additional (missing?) typedefs
// --------------------------------------------------------------------------------------------
#ifndef __cplusplus
typedef struct PPB_OpenGLES2          PPB_OpenGLES2;
typedef struct PP_Var                 PP_Var;
typedef struct PP_Rect                PP_Rect;
typedef struct PP_Point               PP_Point;
typedef struct PP_Size                PP_Size;
typedef struct PP_FileInfo            PP_FileInfo;
typedef struct PP_CompletionCallback  PP_CompletionCallback;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------------------------
// These are the functions you must provide to use this API:
// --------------------------------------------------------------------------------------------
PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] );
void    NaBlackInstanceDestroy();
void    NaBlackDidChangeView( PP_Resource view );
void    NaBlackDidChangeFocus( PP_Bool has_focus );
void    NaBlackRenderFrame();
PP_Bool NaBlackHandleInputEvent( PP_Resource input_event );
PP_Bool NaBlackHandleDocumentLoad( PP_Resource url_loader );
void    NaBlackHandleMessage( struct PP_Var message );
void    NaBlackMouseLockLost();

#ifdef NATIVE_BLACK_C_INLINE
// --------------------------------------------------------------------------------------------
// PPB_Audio interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackAudioCreate( PP_Resource config, PPB_Audio_Callback audio_callback, void* user_data );
extern PP_Bool                       NaBlackAudioIsAudio( PP_Resource resource );
extern PP_Resource                   NaBlackAudioGetCurrentConfig( PP_Resource audio );
extern PP_Bool                       NaBlackAudioStartPlayback( PP_Resource audio );
extern PP_Bool                       NaBlackAudioStopPlayback( PP_Resource audio );

// --------------------------------------------------------------------------------------------
// PPB_AudioConfig interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackAudioConfigCreateStereo16Bit( PP_AudioSampleRate sample_rate, uint32_t sample_frame_count );
extern uint32_t                      NaBlackAudioConfigRecommendSampleFrameCount( PP_AudioSampleRate sample_rate, uint32_t requested_sample_frame_count );
extern PP_Bool                       NaBlackAudioConfigIsAudioConfig( PP_Resource resource );
extern PP_AudioSampleRate            NaBlackAudioConfigGetSampleRate( PP_Resource config );
extern uint32_t                      NaBlackAudioConfigGetSampleFrameCount( PP_Resource config );
extern PP_AudioSampleRate            NaBlackAudioConfigRecommendSampleRate(  );

// --------------------------------------------------------------------------------------------
// PPB_Core interface
// --------------------------------------------------------------------------------------------
extern void                          NaBlackCoreAddRefResource( PP_Resource resource );
extern void                          NaBlackCoreReleaseResource( PP_Resource resource );
extern PP_Time                       NaBlackCoreGetTime(  );
extern PP_TimeTicks                  NaBlackCoreGetTimeTicks(  );
extern void                          NaBlackCoreCallOnMainThread( int32_t delay_in_milliseconds, PP_CompletionCallback callback, int32_t result );
extern PP_Bool                       NaBlackCoreIsMainThread(  );

// --------------------------------------------------------------------------------------------
// PPB_FileIO interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackFileIOCreate(  );
extern PP_Bool                       NaBlackFileIOIsFileIO( PP_Resource resource );
extern int32_t                       NaBlackFileIOOpen( PP_Resource file_io, PP_Resource file_ref, int32_t open_flags, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileIOQuery( PP_Resource file_io, struct PP_FileInfo* info, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileIOTouch( PP_Resource file_io, PP_Time last_access_time, PP_Time last_modified_time, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileIORead( PP_Resource file_io, int64_t offset, char* buffer, int32_t bytes_to_read, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileIOWrite( PP_Resource file_io, int64_t offset, const char* buffer, int32_t bytes_to_write, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileIOSetLength( PP_Resource file_io, int64_t length, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileIOFlush( PP_Resource file_io, PP_CompletionCallback callback );
extern void                          NaBlackFileIOClose( PP_Resource file_io );

// --------------------------------------------------------------------------------------------
// PPB_FileRef interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackFileRefCreate( PP_Resource file_system, const char* path );
extern PP_Bool                       NaBlackFileRefIsFileRef( PP_Resource resource );
extern PP_FileSystemType             NaBlackFileRefGetFileSystemType( PP_Resource file_ref );
extern PP_Var                        NaBlackFileRefGetName( PP_Resource file_ref );
extern PP_Var                        NaBlackFileRefGetPath( PP_Resource file_ref );
extern PP_Resource                   NaBlackFileRefGetParent( PP_Resource file_ref );
extern int32_t                       NaBlackFileRefMakeDirectory( PP_Resource directory_ref, PP_Bool make_ancestors, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileRefTouch( PP_Resource file_ref, PP_Time last_access_time, PP_Time last_modified_time, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileRefDelete( PP_Resource file_ref, PP_CompletionCallback callback );
extern int32_t                       NaBlackFileRefRename( PP_Resource file_ref, PP_Resource new_file_ref, PP_CompletionCallback callback );

// --------------------------------------------------------------------------------------------
// PPB_FileSystem interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackFileSystemCreate( PP_FileSystemType type );
extern PP_Bool                       NaBlackFileSystemIsFileSystem( PP_Resource resource );
extern int32_t                       NaBlackFileSystemOpen( PP_Resource file_system, int64_t expected_size, PP_CompletionCallback callback );
extern PP_FileSystemType             NaBlackFileSystemGetType( PP_Resource file_system );

// --------------------------------------------------------------------------------------------
// PPB_Fullscreen interface
// --------------------------------------------------------------------------------------------
extern PP_Bool                       NaBlackFullscreenIsFullscreen(  );
extern PP_Bool                       NaBlackFullscreenSetFullscreen( PP_Bool fullscreen );
extern PP_Bool                       NaBlackFullscreenGetScreenSize( struct PP_Size* size );

// --------------------------------------------------------------------------------------------
// PPB_Graphics2D interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackGraphics2DCreate( const struct PP_Size* size, PP_Bool is_always_opaque );
extern PP_Bool                       NaBlackGraphics2DIsGraphics2D( PP_Resource resource );
extern PP_Bool                       NaBlackGraphics2DDescribe( PP_Resource graphics_2d, struct PP_Size* size, PP_Bool* is_always_opqaue );
extern void                          NaBlackGraphics2DPaintImageData( PP_Resource graphics_2d, PP_Resource image_data, const struct PP_Point* top_left, const struct PP_Rect* src_rect );
extern void                          NaBlackGraphics2DScroll( PP_Resource graphics_2d, const struct PP_Rect* clip_rect, const struct PP_Point* amount );
extern void                          NaBlackGraphics2DReplaceContents( PP_Resource graphics_2d, PP_Resource image_data );
extern int32_t                       NaBlackGraphics2DFlush( PP_Resource graphics_2d, PP_CompletionCallback callback );

// --------------------------------------------------------------------------------------------
// PPB_Graphics3D interface
// --------------------------------------------------------------------------------------------
extern int32_t                       NaBlackGraphics3DGetAttribMaxValue( PP_Resource instance, int32_t attribute, int32_t* value );
extern PP_Resource                   NaBlackGraphics3DCreate( PP_Resource share_context, const int32_t attrib_list[] );
extern PP_Bool                       NaBlackGraphics3DIsGraphics3D( PP_Resource resource );
extern int32_t                       NaBlackGraphics3DGetAttribs( PP_Resource context, int32_t attrib_list[] );
extern int32_t                       NaBlackGraphics3DSetAttribs( PP_Resource context, int32_t attrib_list[] );
extern int32_t                       NaBlackGraphics3DGetError( PP_Resource context );
extern int32_t                       NaBlackGraphics3DResizeBuffers( PP_Resource context, int32_t width, int32_t height );
extern int32_t                       NaBlackGraphics3DSwapBuffers( PP_Resource context, PP_CompletionCallback callback );

// --------------------------------------------------------------------------------------------
// PPB_ImageData interface
// --------------------------------------------------------------------------------------------
extern PP_ImageDataFormat            NaBlackImageDataGetNativeImageDataFormat(  );
extern PP_Bool                       NaBlackImageDataIsImageDataFormatSupported( PP_ImageDataFormat format );
extern PP_Resource                   NaBlackImageDataCreate( PP_ImageDataFormat format, const struct PP_Size* size, PP_Bool init_to_zero );
extern PP_Bool                       NaBlackImageDataIsImageData( PP_Resource image_data );
extern PP_Bool                       NaBlackImageDataDescribe( PP_Resource image_data, struct PP_ImageDataDesc* desc );
extern void*                         NaBlackImageDataMap( PP_Resource image_data );
extern void                          NaBlackImageDataUnmap( PP_Resource image_data );

// --------------------------------------------------------------------------------------------
// PPB_InputEvent interface
// --------------------------------------------------------------------------------------------
extern int32_t                       NaBlackInputEventRequestInputEvents( uint32_t event_classes );
extern int32_t                       NaBlackInputEventRequestFilteringInputEvents( uint32_t event_classes );
extern void                          NaBlackInputEventClearInputEventRequest( uint32_t event_classes );
extern PP_Bool                       NaBlackInputEventIsInputEvent( PP_Resource resource );
extern PP_InputEvent_Type            NaBlackInputEventGetType( PP_Resource event );
extern PP_TimeTicks                  NaBlackInputEventGetTimeStamp( PP_Resource event );
extern uint32_t                      NaBlackInputEventGetModifiers( PP_Resource event );

// --------------------------------------------------------------------------------------------
// PPB_Instance interface
// --------------------------------------------------------------------------------------------
extern PP_Bool                       NaBlackInstanceBindGraphics( PP_Resource device );
extern PP_Bool                       NaBlackInstanceIsFullFrame(  );

// --------------------------------------------------------------------------------------------
// PPB_KeyboardInputEvent interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackKeyboardInputEventCreate( PP_InputEvent_Type type, PP_TimeTicks time_stamp, uint32_t modifiers, uint32_t key_code, PP_Var character_text );
extern PP_Bool                       NaBlackKeyboardInputEventIsKeyboardInputEvent( PP_Resource resource );
extern uint32_t                      NaBlackKeyboardInputEventGetKeyCode( PP_Resource key_event );
extern PP_Var                        NaBlackKeyboardInputEventGetCharacterText( PP_Resource character_event );

// --------------------------------------------------------------------------------------------
// PPB_Messaging interface
// --------------------------------------------------------------------------------------------
extern void                          NaBlackMessagingPostMessage( PP_Var message );

// --------------------------------------------------------------------------------------------
// PPB_MouseInputEvent interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackMouseInputEventCreate( PP_InputEvent_Type type, PP_TimeTicks time_stamp, uint32_t modifiers, PP_InputEvent_MouseButton mouse_button, const struct PP_Point* mouse_position, int32_t click_count, const struct PP_Point* mouse_movement );
extern PP_Bool                       NaBlackMouseInputEventIsMouseInputEvent( PP_Resource resource );
extern PP_InputEvent_MouseButton     NaBlackMouseInputEventGetButton( PP_Resource mouse_event );
extern PP_Point                      NaBlackMouseInputEventGetPosition( PP_Resource mouse_event );
extern int32_t                       NaBlackMouseInputEventGetClickCount( PP_Resource mouse_event );
extern PP_Point                      NaBlackMouseInputEventGetMovement( PP_Resource mouse_event );

// --------------------------------------------------------------------------------------------
// PPB_MouseLock interface
// --------------------------------------------------------------------------------------------
extern int32_t                       NaBlackMouseLockLockMouse( PP_CompletionCallback callback );
extern void                          NaBlackMouseLockUnlockMouse(  );

// --------------------------------------------------------------------------------------------
// PPB_OpenGLES2 interface
// --------------------------------------------------------------------------------------------
extern void                          NaBlackOpenGLES2ActiveTexture( PP_Resource context, GLenum texture );
extern void                          NaBlackOpenGLES2AttachShader( PP_Resource context, GLuint program, GLuint shader );
extern void                          NaBlackOpenGLES2BindAttribLocation( PP_Resource context, GLuint program, GLuint index, const char* name );
extern void                          NaBlackOpenGLES2BindBuffer( PP_Resource context, GLenum target, GLuint buffer );
extern void                          NaBlackOpenGLES2BindFramebuffer( PP_Resource context, GLenum target, GLuint framebuffer );
extern void                          NaBlackOpenGLES2BindRenderbuffer( PP_Resource context, GLenum target, GLuint renderbuffer );
extern void                          NaBlackOpenGLES2BindTexture( PP_Resource context, GLenum target, GLuint texture );
extern void                          NaBlackOpenGLES2BlendColor( PP_Resource context, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
extern void                          NaBlackOpenGLES2BlendEquation( PP_Resource context, GLenum mode );
extern void                          NaBlackOpenGLES2BlendEquationSeparate( PP_Resource context, GLenum modeRGB, GLenum modeAlpha );
extern void                          NaBlackOpenGLES2BlendFunc( PP_Resource context, GLenum sfactor, GLenum dfactor );
extern void                          NaBlackOpenGLES2BlendFuncSeparate( PP_Resource context, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );
extern void                          NaBlackOpenGLES2BufferData( PP_Resource context, GLenum target, GLsizeiptr size, const void* data, GLenum usage );
extern void                          NaBlackOpenGLES2BufferSubData( PP_Resource context, GLenum target, GLintptr offset, GLsizeiptr size, const void* data );
extern GLenum                        NaBlackOpenGLES2CheckFramebufferStatus( PP_Resource context, GLenum target );
extern void                          NaBlackOpenGLES2Clear( PP_Resource context, GLbitfield mask );
extern void                          NaBlackOpenGLES2ClearColor( PP_Resource context, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
extern void                          NaBlackOpenGLES2ClearDepthf( PP_Resource context, GLclampf depth );
extern void                          NaBlackOpenGLES2ClearStencil( PP_Resource context, GLint s );
extern void                          NaBlackOpenGLES2ColorMask( PP_Resource context, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
extern void                          NaBlackOpenGLES2CompileShader( PP_Resource context, GLuint shader );
extern void                          NaBlackOpenGLES2CompressedTexImage2D( PP_Resource context, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data );
extern void                          NaBlackOpenGLES2CompressedTexSubImage2D( PP_Resource context, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data );
extern void                          NaBlackOpenGLES2CopyTexImage2D( PP_Resource context, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border );
extern void                          NaBlackOpenGLES2CopyTexSubImage2D( PP_Resource context, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height );
extern GLuint                        NaBlackOpenGLES2CreateProgram( PP_Resource context );
extern GLuint                        NaBlackOpenGLES2CreateShader( PP_Resource context, GLenum type );
extern void                          NaBlackOpenGLES2CullFace( PP_Resource context, GLenum mode );
extern void                          NaBlackOpenGLES2DeleteBuffers( PP_Resource context, GLsizei n, const GLuint* buffers );
extern void                          NaBlackOpenGLES2DeleteFramebuffers( PP_Resource context, GLsizei n, const GLuint* framebuffers );
extern void                          NaBlackOpenGLES2DeleteProgram( PP_Resource context, GLuint program );
extern void                          NaBlackOpenGLES2DeleteRenderbuffers( PP_Resource context, GLsizei n, const GLuint* renderbuffers );
extern void                          NaBlackOpenGLES2DeleteShader( PP_Resource context, GLuint shader );
extern void                          NaBlackOpenGLES2DeleteTextures( PP_Resource context, GLsizei n, const GLuint* textures );
extern void                          NaBlackOpenGLES2DepthFunc( PP_Resource context, GLenum func );
extern void                          NaBlackOpenGLES2DepthMask( PP_Resource context, GLboolean flag );
extern void                          NaBlackOpenGLES2DepthRangef( PP_Resource context, GLclampf zNear, GLclampf zFar );
extern void                          NaBlackOpenGLES2DetachShader( PP_Resource context, GLuint program, GLuint shader );
extern void                          NaBlackOpenGLES2Disable( PP_Resource context, GLenum cap );
extern void                          NaBlackOpenGLES2DisableVertexAttribArray( PP_Resource context, GLuint index );
extern void                          NaBlackOpenGLES2DrawArrays( PP_Resource context, GLenum mode, GLint first, GLsizei count );
extern void                          NaBlackOpenGLES2DrawElements( PP_Resource context, GLenum mode, GLsizei count, GLenum type, const void* indices );
extern void                          NaBlackOpenGLES2Enable( PP_Resource context, GLenum cap );
extern void                          NaBlackOpenGLES2EnableVertexAttribArray( PP_Resource context, GLuint index );
extern void                          NaBlackOpenGLES2Finish( PP_Resource context );
extern void                          NaBlackOpenGLES2Flush( PP_Resource context );
extern void                          NaBlackOpenGLES2FramebufferRenderbuffer( PP_Resource context, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );
extern void                          NaBlackOpenGLES2FramebufferTexture2D( PP_Resource context, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
extern void                          NaBlackOpenGLES2FrontFace( PP_Resource context, GLenum mode );
extern void                          NaBlackOpenGLES2GenBuffers( PP_Resource context, GLsizei n, GLuint* buffers );
extern void                          NaBlackOpenGLES2GenerateMipmap( PP_Resource context, GLenum target );
extern void                          NaBlackOpenGLES2GenFramebuffers( PP_Resource context, GLsizei n, GLuint* framebuffers );
extern void                          NaBlackOpenGLES2GenRenderbuffers( PP_Resource context, GLsizei n, GLuint* renderbuffers );
extern void                          NaBlackOpenGLES2GenTextures( PP_Resource context, GLsizei n, GLuint* textures );
extern void                          NaBlackOpenGLES2GetActiveAttrib( PP_Resource context, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name );
extern void                          NaBlackOpenGLES2GetActiveUniform( PP_Resource context, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name );
extern void                          NaBlackOpenGLES2GetAttachedShaders( PP_Resource context, GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders );
extern GLint                         NaBlackOpenGLES2GetAttribLocation( PP_Resource context, GLuint program, const char* name );
extern void                          NaBlackOpenGLES2GetBooleanv( PP_Resource context, GLenum pname, GLboolean* params );
extern void                          NaBlackOpenGLES2GetBufferParameteriv( PP_Resource context, GLenum target, GLenum pname, GLint* params );
extern GLenum                        NaBlackOpenGLES2GetError( PP_Resource context );
extern void                          NaBlackOpenGLES2GetFloatv( PP_Resource context, GLenum pname, GLfloat* params );
extern void                          NaBlackOpenGLES2GetFramebufferAttachmentParameteriv( PP_Resource context, GLenum target, GLenum attachment, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetIntegerv( PP_Resource context, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetProgramiv( PP_Resource context, GLuint program, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetProgramInfoLog( PP_Resource context, GLuint program, GLsizei bufsize, GLsizei* length, char* infolog );
extern void                          NaBlackOpenGLES2GetRenderbufferParameteriv( PP_Resource context, GLenum target, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetShaderiv( PP_Resource context, GLuint shader, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetShaderInfoLog( PP_Resource context, GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog );
extern void                          NaBlackOpenGLES2GetShaderPrecisionFormat( PP_Resource context, GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision );
extern void                          NaBlackOpenGLES2GetShaderSource( PP_Resource context, GLuint shader, GLsizei bufsize, GLsizei* length, char* source );
extern const GLubyte*                NaBlackOpenGLES2GetString( PP_Resource context, GLenum name );
extern void                          NaBlackOpenGLES2GetTexParameterfv( PP_Resource context, GLenum target, GLenum pname, GLfloat* params );
extern void                          NaBlackOpenGLES2GetTexParameteriv( PP_Resource context, GLenum target, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetUniformfv( PP_Resource context, GLuint program, GLint location, GLfloat* params );
extern void                          NaBlackOpenGLES2GetUniformiv( PP_Resource context, GLuint program, GLint location, GLint* params );
extern GLint                         NaBlackOpenGLES2GetUniformLocation( PP_Resource context, GLuint program, const char* name );
extern void                          NaBlackOpenGLES2GetVertexAttribfv( PP_Resource context, GLuint index, GLenum pname, GLfloat* params );
extern void                          NaBlackOpenGLES2GetVertexAttribiv( PP_Resource context, GLuint index, GLenum pname, GLint* params );
extern void                          NaBlackOpenGLES2GetVertexAttribPointerv( PP_Resource context, GLuint index, GLenum pname, void** pointer );
extern void                          NaBlackOpenGLES2Hint( PP_Resource context, GLenum target, GLenum mode );
extern GLboolean                     NaBlackOpenGLES2IsBuffer( PP_Resource context, GLuint buffer );
extern GLboolean                     NaBlackOpenGLES2IsEnabled( PP_Resource context, GLenum cap );
extern GLboolean                     NaBlackOpenGLES2IsFramebuffer( PP_Resource context, GLuint framebuffer );
extern GLboolean                     NaBlackOpenGLES2IsProgram( PP_Resource context, GLuint program );
extern GLboolean                     NaBlackOpenGLES2IsRenderbuffer( PP_Resource context, GLuint renderbuffer );
extern GLboolean                     NaBlackOpenGLES2IsShader( PP_Resource context, GLuint shader );
extern GLboolean                     NaBlackOpenGLES2IsTexture( PP_Resource context, GLuint texture );
extern void                          NaBlackOpenGLES2LineWidth( PP_Resource context, GLfloat width );
extern void                          NaBlackOpenGLES2LinkProgram( PP_Resource context, GLuint program );
extern void                          NaBlackOpenGLES2PixelStorei( PP_Resource context, GLenum pname, GLint param );
extern void                          NaBlackOpenGLES2PolygonOffset( PP_Resource context, GLfloat factor, GLfloat units );
extern void                          NaBlackOpenGLES2ReadPixels( PP_Resource context, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels );
extern void                          NaBlackOpenGLES2ReleaseShaderCompiler( PP_Resource context );
extern void                          NaBlackOpenGLES2RenderbufferStorage( PP_Resource context, GLenum target, GLenum internalformat, GLsizei width, GLsizei height );
extern void                          NaBlackOpenGLES2SampleCoverage( PP_Resource context, GLclampf value, GLboolean invert );
extern void                          NaBlackOpenGLES2Scissor( PP_Resource context, GLint x, GLint y, GLsizei width, GLsizei height );
extern void                          NaBlackOpenGLES2ShaderBinary( PP_Resource context, GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length );
extern void                          NaBlackOpenGLES2ShaderSource( PP_Resource context, GLuint shader, GLsizei count, const char** str, const GLint* length );
extern void                          NaBlackOpenGLES2StencilFunc( PP_Resource context, GLenum func, GLint ref, GLuint mask );
extern void                          NaBlackOpenGLES2StencilFuncSeparate( PP_Resource context, GLenum face, GLenum func, GLint ref, GLuint mask );
extern void                          NaBlackOpenGLES2StencilMask( PP_Resource context, GLuint mask );
extern void                          NaBlackOpenGLES2StencilMaskSeparate( PP_Resource context, GLenum face, GLuint mask );
extern void                          NaBlackOpenGLES2StencilOp( PP_Resource context, GLenum fail, GLenum zfail, GLenum zpass );
extern void                          NaBlackOpenGLES2StencilOpSeparate( PP_Resource context, GLenum face, GLenum fail, GLenum zfail, GLenum zpass );
extern void                          NaBlackOpenGLES2TexImage2D( PP_Resource context, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels );
extern void                          NaBlackOpenGLES2TexParameterf( PP_Resource context, GLenum target, GLenum pname, GLfloat param );
extern void                          NaBlackOpenGLES2TexParameterfv( PP_Resource context, GLenum target, GLenum pname, const GLfloat* params );
extern void                          NaBlackOpenGLES2TexParameteri( PP_Resource context, GLenum target, GLenum pname, GLint param );
extern void                          NaBlackOpenGLES2TexParameteriv( PP_Resource context, GLenum target, GLenum pname, const GLint* params );
extern void                          NaBlackOpenGLES2TexSubImage2D( PP_Resource context, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels );
extern void                          NaBlackOpenGLES2Uniform1f( PP_Resource context, GLint location, GLfloat x );
extern void                          NaBlackOpenGLES2Uniform1fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v );
extern void                          NaBlackOpenGLES2Uniform1i( PP_Resource context, GLint location, GLint x );
extern void                          NaBlackOpenGLES2Uniform1iv( PP_Resource context, GLint location, GLsizei count, const GLint* v );
extern void                          NaBlackOpenGLES2Uniform2f( PP_Resource context, GLint location, GLfloat x, GLfloat y );
extern void                          NaBlackOpenGLES2Uniform2fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v );
extern void                          NaBlackOpenGLES2Uniform2i( PP_Resource context, GLint location, GLint x, GLint y );
extern void                          NaBlackOpenGLES2Uniform2iv( PP_Resource context, GLint location, GLsizei count, const GLint* v );
extern void                          NaBlackOpenGLES2Uniform3f( PP_Resource context, GLint location, GLfloat x, GLfloat y, GLfloat z );
extern void                          NaBlackOpenGLES2Uniform3fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v );
extern void                          NaBlackOpenGLES2Uniform3i( PP_Resource context, GLint location, GLint x, GLint y, GLint z );
extern void                          NaBlackOpenGLES2Uniform3iv( PP_Resource context, GLint location, GLsizei count, const GLint* v );
extern void                          NaBlackOpenGLES2Uniform4f( PP_Resource context, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
extern void                          NaBlackOpenGLES2Uniform4fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v );
extern void                          NaBlackOpenGLES2Uniform4i( PP_Resource context, GLint location, GLint x, GLint y, GLint z, GLint w );
extern void                          NaBlackOpenGLES2Uniform4iv( PP_Resource context, GLint location, GLsizei count, const GLint* v );
extern void                          NaBlackOpenGLES2UniformMatrix2fv( PP_Resource context, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
extern void                          NaBlackOpenGLES2UniformMatrix3fv( PP_Resource context, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
extern void                          NaBlackOpenGLES2UniformMatrix4fv( PP_Resource context, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
extern void                          NaBlackOpenGLES2UseProgram( PP_Resource context, GLuint program );
extern void                          NaBlackOpenGLES2ValidateProgram( PP_Resource context, GLuint program );
extern void                          NaBlackOpenGLES2VertexAttrib1f( PP_Resource context, GLuint indx, GLfloat x );
extern void                          NaBlackOpenGLES2VertexAttrib1fv( PP_Resource context, GLuint indx, const GLfloat* values );
extern void                          NaBlackOpenGLES2VertexAttrib2f( PP_Resource context, GLuint indx, GLfloat x, GLfloat y );
extern void                          NaBlackOpenGLES2VertexAttrib2fv( PP_Resource context, GLuint indx, const GLfloat* values );
extern void                          NaBlackOpenGLES2VertexAttrib3f( PP_Resource context, GLuint indx, GLfloat x, GLfloat y, GLfloat z );
extern void                          NaBlackOpenGLES2VertexAttrib3fv( PP_Resource context, GLuint indx, const GLfloat* values );
extern void                          NaBlackOpenGLES2VertexAttrib4f( PP_Resource context, GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
extern void                          NaBlackOpenGLES2VertexAttrib4fv( PP_Resource context, GLuint indx, const GLfloat* values );
extern void                          NaBlackOpenGLES2VertexAttribPointer( PP_Resource context, GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr );
extern void                          NaBlackOpenGLES2Viewport( PP_Resource context, GLint x, GLint y, GLsizei width, GLsizei height );

// --------------------------------------------------------------------------------------------
// PPB_URLLoader interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackURLLoaderCreate(  );
extern PP_Bool                       NaBlackURLLoaderIsURLLoader( PP_Resource resource );
extern int32_t                       NaBlackURLLoaderOpen( PP_Resource loader, PP_Resource request_info, PP_CompletionCallback callback );
extern int32_t                       NaBlackURLLoaderFollowRedirect( PP_Resource loader, PP_CompletionCallback callback );
extern PP_Bool                       NaBlackURLLoaderGetUploadProgress( PP_Resource loader, int64_t* bytes_sent, int64_t* total_bytes_to_be_sent );
extern PP_Bool                       NaBlackURLLoaderGetDownloadProgress( PP_Resource loader, int64_t* bytes_received, int64_t* total_bytes_to_be_received );
extern PP_Resource                   NaBlackURLLoaderGetResponseInfo( PP_Resource loader );
extern int32_t                       NaBlackURLLoaderReadResponseBody( PP_Resource loader, void* buffer, int32_t bytes_to_read, PP_CompletionCallback callback );
extern int32_t                       NaBlackURLLoaderFinishStreamingToFile( PP_Resource loader, PP_CompletionCallback callback );
extern void                          NaBlackURLLoaderClose( PP_Resource loader );

// --------------------------------------------------------------------------------------------
// PPB_URLRequestInfo interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackURLRequestInfoCreate(  );
extern PP_Bool                       NaBlackURLRequestInfoIsURLRequestInfo( PP_Resource resource );
extern PP_Bool                       NaBlackURLRequestInfoSetProperty( PP_Resource request, PP_URLRequestProperty property, PP_Var value );
extern PP_Bool                       NaBlackURLRequestInfoAppendDataToBody( PP_Resource request, const void* data, uint32_t len );
extern PP_Bool                       NaBlackURLRequestInfoAppendFileToBody( PP_Resource request, PP_Resource file_ref, int64_t start_offset, int64_t number_of_bytes, PP_Time expected_last_modified_time );

// --------------------------------------------------------------------------------------------
// PPB_URLResponseInfo interface
// --------------------------------------------------------------------------------------------
extern PP_Bool                       NaBlackURLResponseInfoIsURLResponseInfo( PP_Resource resource );
extern PP_Var                        NaBlackURLResponseInfoGetProperty( PP_Resource response, PP_URLResponseProperty property );
extern PP_Resource                   NaBlackURLResponseInfoGetBodyAsFileRef( PP_Resource response );

// --------------------------------------------------------------------------------------------
// PPB_Var interface
// --------------------------------------------------------------------------------------------
extern void                          NaBlackVarAddRef( PP_Var var );
extern void                          NaBlackVarRelease( PP_Var var );
extern PP_Var                        NaBlackVarVarFromUtf8( const char* data, uint32_t len );
extern const char*                   NaBlackVarVarToUtf8( PP_Var var, uint32_t* len );

// --------------------------------------------------------------------------------------------
// PPB_VarArrayBuffer interface
// --------------------------------------------------------------------------------------------
extern PP_Var                        NaBlackVarArrayBufferCreate( uint32_t size_in_bytes );
extern PP_Bool                       NaBlackVarArrayBufferByteLength( PP_Var array, uint32_t* byte_length );
extern void*                         NaBlackVarArrayBufferMap( PP_Var array );
extern void                          NaBlackVarArrayBufferUnmap( PP_Var array );

// --------------------------------------------------------------------------------------------
// PPB_View interface
// --------------------------------------------------------------------------------------------
extern PP_Bool                       NaBlackViewIsView( PP_Resource resource );
extern PP_Bool                       NaBlackViewGetRect( PP_Resource resource, struct PP_Rect* rect );
extern PP_Bool                       NaBlackViewIsFullscreen( PP_Resource resource );
extern PP_Bool                       NaBlackViewIsVisible( PP_Resource resource );
extern PP_Bool                       NaBlackViewIsPageVisible( PP_Resource resource );
extern PP_Bool                       NaBlackViewGetClipRect( PP_Resource resource, struct PP_Rect* clip );

// --------------------------------------------------------------------------------------------
// PPB_WebSocket interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackWebSocketCreate(  );
extern PP_Bool                       NaBlackWebSocketIsWebSocket( PP_Resource resource );
extern int32_t                       NaBlackWebSocketConnect( PP_Resource web_socket, PP_Var url, const struct PP_Var protocols[], uint32_t protocol_count, PP_CompletionCallback callback );
extern int32_t                       NaBlackWebSocketClose( PP_Resource web_socket, uint16_t code, PP_Var reason, PP_CompletionCallback callback );
extern int32_t                       NaBlackWebSocketReceiveMessage( PP_Resource web_socket, struct PP_Var* message, PP_CompletionCallback callback );
extern int32_t                       NaBlackWebSocketSendMessage( PP_Resource web_socket, PP_Var message );
extern uint64_t                      NaBlackWebSocketGetBufferedAmount( PP_Resource web_socket );
extern uint16_t                      NaBlackWebSocketGetCloseCode( PP_Resource web_socket );
extern PP_Var                        NaBlackWebSocketGetCloseReason( PP_Resource web_socket );
extern PP_Bool                       NaBlackWebSocketGetCloseWasClean( PP_Resource web_socket );
extern PP_Var                        NaBlackWebSocketGetExtensions( PP_Resource web_socket );
extern PP_Var                        NaBlackWebSocketGetProtocol( PP_Resource web_socket );
extern PP_WebSocketReadyState        NaBlackWebSocketGetReadyState( PP_Resource web_socket );
extern PP_Var                        NaBlackWebSocketGetURL( PP_Resource web_socket );

// --------------------------------------------------------------------------------------------
// PPB_WheelInputEvent interface
// --------------------------------------------------------------------------------------------
extern PP_Resource                   NaBlackWheelInputEventCreate( PP_TimeTicks time_stamp, uint32_t modifiers, const struct PP_FloatPoint* wheel_delta, const struct PP_FloatPoint* wheel_ticks, PP_Bool scroll_by_page );
extern PP_Bool                       NaBlackWheelInputEventIsWheelInputEvent( PP_Resource resource );
extern struct PP_FloatPoint          NaBlackWheelInputEventGetDelta( PP_Resource wheel_event );
extern struct PP_FloatPoint          NaBlackWheelInputEventGetTicks( PP_Resource wheel_event );
extern PP_Bool                       NaBlackWheelInputEventGetScrollByPage( PP_Resource wheel_event );

#endif // NATIVE_BLACK_C_INLINE

// --------------------------------------------------------------------------------------------
// Globals used by single instance interfaces
// --------------------------------------------------------------------------------------------

enum
{
  kNaBlackInvalidInstance = 0,
  kNaBlackInvalidResource = 0,
};

extern PP_Instance                      g_NaBlackInstanceId;
extern int32_t                          g_NaBlackViewWidth;
extern int32_t                          g_NaBlackViewHeight;
extern PP_Resource                      g_NaBlackGraphics3dId;
extern int32_t                          g_NaBlackGraphics3dLastSwapResult;
extern const PPB_Audio*                 g_NaBlackAudio;               // (5) functions
extern const PPB_AudioConfig*           g_NaBlackAudioConfig;         // (6) functions
extern const PPB_Core*                  g_NaBlackCore;                // (6) functions
extern const PPB_FileIO*                g_NaBlackFileIO;              // (10) functions
extern const PPB_FileRef*               g_NaBlackFileRef;             // (10) functions
extern const PPB_FileSystem*            g_NaBlackFileSystem;          // (4) functions
extern const PPB_Fullscreen*            g_NaBlackFullscreen;          // (3) functions
extern const PPB_Graphics2D*            g_NaBlackGraphics2D;          // (7) functions
extern const PPB_Graphics3D*            g_NaBlackGraphics3D;          // (8) functions
extern const PPB_ImageData*             g_NaBlackImageData;           // (7) functions
extern const PPB_InputEvent*            g_NaBlackInputEvent;          // (7) functions
extern const PPB_Instance*              g_NaBlackInstance;            // (2) functions
extern const PPB_KeyboardInputEvent*    g_NaBlackKeyboardInputEvent;  // (4) functions
extern const PPB_Messaging*             g_NaBlackMessaging;           // (1) functions
extern const PPB_MouseInputEvent*       g_NaBlackMouseInputEvent;     // (6) functions
extern const PPB_MouseLock*             g_NaBlackMouseLock;           // (2) functions
extern const PPB_OpenGLES2*             g_NaBlackOpenGLES2;           // (142) functions
extern const PPB_URLLoader*             g_NaBlackURLLoader;           // (10) functions
extern const PPB_URLRequestInfo*        g_NaBlackURLRequestInfo;      // (5) functions
extern const PPB_URLResponseInfo*       g_NaBlackURLResponseInfo;     // (3) functions
extern const PPB_Var*                   g_NaBlackVar;                 // (4) functions
extern const PPB_VarArrayBuffer*        g_NaBlackVarArrayBuffer;      // (4) functions
extern const PPB_View*                  g_NaBlackView;                // (6) functions
extern const PPB_WebSocket*             g_NaBlackWebSocket;           // (14) functions
extern const PPB_WheelInputEvent*       g_NaBlackWheelInputEvent;     // (5) functions

// --------------------------------------------------------------------------------------------
// PPB_Audio inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackAudioCreate( PP_Resource config, PPB_Audio_Callback audio_callback, void* user_data )
{
  return g_NaBlackAudio->Create(g_NaBlackInstanceId, config, audio_callback, user_data);
}

inline PP_Bool
NaBlackAudioIsAudio( PP_Resource resource )
{
  return g_NaBlackAudio->IsAudio(resource);
}

inline PP_Resource
NaBlackAudioGetCurrentConfig( PP_Resource audio )
{
  return g_NaBlackAudio->GetCurrentConfig(audio);
}

inline PP_Bool
NaBlackAudioStartPlayback( PP_Resource audio )
{
  return g_NaBlackAudio->StartPlayback(audio);
}

inline PP_Bool
NaBlackAudioStopPlayback( PP_Resource audio )
{
  return g_NaBlackAudio->StopPlayback(audio);
}


// --------------------------------------------------------------------------------------------
// PPB_AudioConfig inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackAudioConfigCreateStereo16Bit( PP_AudioSampleRate sample_rate, uint32_t sample_frame_count )
{
  return g_NaBlackAudioConfig->CreateStereo16Bit(g_NaBlackInstanceId, sample_rate, sample_frame_count);
}

inline uint32_t
NaBlackAudioConfigRecommendSampleFrameCount( PP_AudioSampleRate sample_rate, uint32_t requested_sample_frame_count )
{
  return g_NaBlackAudioConfig->RecommendSampleFrameCount(g_NaBlackInstanceId, sample_rate, requested_sample_frame_count);
}

inline PP_Bool
NaBlackAudioConfigIsAudioConfig( PP_Resource resource )
{
  return g_NaBlackAudioConfig->IsAudioConfig(resource);
}

inline PP_AudioSampleRate
NaBlackAudioConfigGetSampleRate( PP_Resource config )
{
  return g_NaBlackAudioConfig->GetSampleRate(config);
}

inline uint32_t
NaBlackAudioConfigGetSampleFrameCount( PP_Resource config )
{
  return g_NaBlackAudioConfig->GetSampleFrameCount(config);
}

inline PP_AudioSampleRate
NaBlackAudioConfigRecommendSampleRate(  )
{
  return g_NaBlackAudioConfig->RecommendSampleRate(g_NaBlackInstanceId);
}


// --------------------------------------------------------------------------------------------
// PPB_Core inline definitions
// --------------------------------------------------------------------------------------------
inline void
NaBlackCoreAddRefResource( PP_Resource resource )
{
  g_NaBlackCore->AddRefResource(resource);
}

inline void
NaBlackCoreReleaseResource( PP_Resource resource )
{
  g_NaBlackCore->ReleaseResource(resource);
}

inline PP_Time
NaBlackCoreGetTime(  )
{
  return g_NaBlackCore->GetTime();
}

inline PP_TimeTicks
NaBlackCoreGetTimeTicks(  )
{
  return g_NaBlackCore->GetTimeTicks();
}

inline void
NaBlackCoreCallOnMainThread( int32_t delay_in_milliseconds, PP_CompletionCallback callback, int32_t result )
{
  g_NaBlackCore->CallOnMainThread(delay_in_milliseconds, callback, result);
}

inline PP_Bool
NaBlackCoreIsMainThread(  )
{
  return g_NaBlackCore->IsMainThread();
}


// --------------------------------------------------------------------------------------------
// PPB_FileIO inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackFileIOCreate(  )
{
  return g_NaBlackFileIO->Create(g_NaBlackInstanceId);
}

inline PP_Bool
NaBlackFileIOIsFileIO( PP_Resource resource )
{
  return g_NaBlackFileIO->IsFileIO(resource);
}

inline int32_t
NaBlackFileIOOpen( PP_Resource file_io, PP_Resource file_ref, int32_t open_flags, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->Open(file_io, file_ref, open_flags, callback);
}

inline int32_t
NaBlackFileIOQuery( PP_Resource file_io, struct PP_FileInfo* info, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->Query(file_io, info, callback);
}

inline int32_t
NaBlackFileIOTouch( PP_Resource file_io, PP_Time last_access_time, PP_Time last_modified_time, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->Touch(file_io, last_access_time, last_modified_time, callback);
}

inline int32_t
NaBlackFileIORead( PP_Resource file_io, int64_t offset, char* buffer, int32_t bytes_to_read, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->Read(file_io, offset, buffer, bytes_to_read, callback);
}

inline int32_t
NaBlackFileIOWrite( PP_Resource file_io, int64_t offset, const char* buffer, int32_t bytes_to_write, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->Write(file_io, offset, buffer, bytes_to_write, callback);
}

inline int32_t
NaBlackFileIOSetLength( PP_Resource file_io, int64_t length, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->SetLength(file_io, length, callback);
}

inline int32_t
NaBlackFileIOFlush( PP_Resource file_io, PP_CompletionCallback callback )
{
  return g_NaBlackFileIO->Flush(file_io, callback);
}

inline void
NaBlackFileIOClose( PP_Resource file_io )
{
  g_NaBlackFileIO->Close(file_io);
}


// --------------------------------------------------------------------------------------------
// PPB_FileRef inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackFileRefCreate( PP_Resource file_system, const char* path )
{
  return g_NaBlackFileRef->Create(file_system, path);
}

inline PP_Bool
NaBlackFileRefIsFileRef( PP_Resource resource )
{
  return g_NaBlackFileRef->IsFileRef(resource);
}

inline PP_FileSystemType
NaBlackFileRefGetFileSystemType( PP_Resource file_ref )
{
  return g_NaBlackFileRef->GetFileSystemType(file_ref);
}

inline PP_Var
NaBlackFileRefGetName( PP_Resource file_ref )
{
  return g_NaBlackFileRef->GetName(file_ref);
}

inline PP_Var
NaBlackFileRefGetPath( PP_Resource file_ref )
{
  return g_NaBlackFileRef->GetPath(file_ref);
}

inline PP_Resource
NaBlackFileRefGetParent( PP_Resource file_ref )
{
  return g_NaBlackFileRef->GetParent(file_ref);
}

inline int32_t
NaBlackFileRefMakeDirectory( PP_Resource directory_ref, PP_Bool make_ancestors, PP_CompletionCallback callback )
{
  return g_NaBlackFileRef->MakeDirectory(directory_ref, make_ancestors, callback);
}

inline int32_t
NaBlackFileRefTouch( PP_Resource file_ref, PP_Time last_access_time, PP_Time last_modified_time, PP_CompletionCallback callback )
{
  return g_NaBlackFileRef->Touch(file_ref, last_access_time, last_modified_time, callback);
}

inline int32_t
NaBlackFileRefDelete( PP_Resource file_ref, PP_CompletionCallback callback )
{
  return g_NaBlackFileRef->Delete(file_ref, callback);
}

inline int32_t
NaBlackFileRefRename( PP_Resource file_ref, PP_Resource new_file_ref, PP_CompletionCallback callback )
{
  return g_NaBlackFileRef->Rename(file_ref, new_file_ref, callback);
}


// --------------------------------------------------------------------------------------------
// PPB_FileSystem inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackFileSystemCreate( PP_FileSystemType type )
{
  return g_NaBlackFileSystem->Create(g_NaBlackInstanceId, type);
}

inline PP_Bool
NaBlackFileSystemIsFileSystem( PP_Resource resource )
{
  return g_NaBlackFileSystem->IsFileSystem(resource);
}

inline int32_t
NaBlackFileSystemOpen( PP_Resource file_system, int64_t expected_size, PP_CompletionCallback callback )
{
  return g_NaBlackFileSystem->Open(file_system, expected_size, callback);
}

inline PP_FileSystemType
NaBlackFileSystemGetType( PP_Resource file_system )
{
  return g_NaBlackFileSystem->GetType(file_system);
}


// --------------------------------------------------------------------------------------------
// PPB_Fullscreen inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Bool
NaBlackFullscreenIsFullscreen(  )
{
  return g_NaBlackFullscreen->IsFullscreen(g_NaBlackInstanceId);
}

inline PP_Bool
NaBlackFullscreenSetFullscreen( PP_Bool fullscreen )
{
  return g_NaBlackFullscreen->SetFullscreen(g_NaBlackInstanceId, fullscreen);
}

inline PP_Bool
NaBlackFullscreenGetScreenSize( struct PP_Size* size )
{
  return g_NaBlackFullscreen->GetScreenSize(g_NaBlackInstanceId, size);
}


// --------------------------------------------------------------------------------------------
// PPB_Graphics2D inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackGraphics2DCreate( const struct PP_Size* size, PP_Bool is_always_opaque )
{
  return g_NaBlackGraphics2D->Create(g_NaBlackInstanceId, size, is_always_opaque);
}

inline PP_Bool
NaBlackGraphics2DIsGraphics2D( PP_Resource resource )
{
  return g_NaBlackGraphics2D->IsGraphics2D(resource);
}

inline PP_Bool
NaBlackGraphics2DDescribe( PP_Resource graphics_2d, struct PP_Size* size, PP_Bool* is_always_opqaue )
{
  return g_NaBlackGraphics2D->Describe(graphics_2d, size, is_always_opqaue);
}

inline void
NaBlackGraphics2DPaintImageData( PP_Resource graphics_2d, PP_Resource image_data, const struct PP_Point* top_left, const struct PP_Rect* src_rect )
{
  g_NaBlackGraphics2D->PaintImageData(graphics_2d, image_data, top_left, src_rect);
}

inline void
NaBlackGraphics2DScroll( PP_Resource graphics_2d, const struct PP_Rect* clip_rect, const struct PP_Point* amount )
{
  g_NaBlackGraphics2D->Scroll(graphics_2d, clip_rect, amount);
}

inline void
NaBlackGraphics2DReplaceContents( PP_Resource graphics_2d, PP_Resource image_data )
{
  g_NaBlackGraphics2D->ReplaceContents(graphics_2d, image_data);
}

inline int32_t
NaBlackGraphics2DFlush( PP_Resource graphics_2d, PP_CompletionCallback callback )
{
  return g_NaBlackGraphics2D->Flush(graphics_2d, callback);
}


// --------------------------------------------------------------------------------------------
// PPB_Graphics3D inline definitions
// --------------------------------------------------------------------------------------------
inline int32_t
NaBlackGraphics3DGetAttribMaxValue( PP_Resource instance, int32_t attribute, int32_t* value )
{
  return g_NaBlackGraphics3D->GetAttribMaxValue(instance, attribute, value);
}

inline PP_Resource
NaBlackGraphics3DCreate( PP_Resource share_context, const int32_t attrib_list[] )
{
  return g_NaBlackGraphics3D->Create(g_NaBlackInstanceId, share_context, attrib_list);
}

inline PP_Bool
NaBlackGraphics3DIsGraphics3D( PP_Resource resource )
{
  return g_NaBlackGraphics3D->IsGraphics3D(resource);
}

inline int32_t
NaBlackGraphics3DGetAttribs( PP_Resource context, int32_t attrib_list[] )
{
  return g_NaBlackGraphics3D->GetAttribs(context, attrib_list);
}

inline int32_t
NaBlackGraphics3DSetAttribs( PP_Resource context, int32_t attrib_list[] )
{
  return g_NaBlackGraphics3D->SetAttribs(context, attrib_list);
}

inline int32_t
NaBlackGraphics3DGetError( PP_Resource context )
{
  return g_NaBlackGraphics3D->GetError(context);
}

inline int32_t
NaBlackGraphics3DResizeBuffers( PP_Resource context, int32_t width, int32_t height )
{
  return g_NaBlackGraphics3D->ResizeBuffers(context, width, height);
}

inline int32_t
NaBlackGraphics3DSwapBuffers( PP_Resource context, PP_CompletionCallback callback )
{
  return g_NaBlackGraphics3D->SwapBuffers(context, callback);
}


// --------------------------------------------------------------------------------------------
// PPB_ImageData inline definitions
// --------------------------------------------------------------------------------------------
inline PP_ImageDataFormat
NaBlackImageDataGetNativeImageDataFormat(  )
{
  return g_NaBlackImageData->GetNativeImageDataFormat();
}

inline PP_Bool
NaBlackImageDataIsImageDataFormatSupported( PP_ImageDataFormat format )
{
  return g_NaBlackImageData->IsImageDataFormatSupported(format);
}

inline PP_Resource
NaBlackImageDataCreate( PP_ImageDataFormat format, const struct PP_Size* size, PP_Bool init_to_zero )
{
  return g_NaBlackImageData->Create(g_NaBlackInstanceId, format, size, init_to_zero);
}

inline PP_Bool
NaBlackImageDataIsImageData( PP_Resource image_data )
{
  return g_NaBlackImageData->IsImageData(image_data);
}

inline PP_Bool
NaBlackImageDataDescribe( PP_Resource image_data, struct PP_ImageDataDesc* desc )
{
  return g_NaBlackImageData->Describe(image_data, desc);
}

inline void*
NaBlackImageDataMap( PP_Resource image_data )
{
  return g_NaBlackImageData->Map(image_data);
}

inline void
NaBlackImageDataUnmap( PP_Resource image_data )
{
  g_NaBlackImageData->Unmap(image_data);
}


// --------------------------------------------------------------------------------------------
// PPB_InputEvent inline definitions
// --------------------------------------------------------------------------------------------
inline int32_t
NaBlackInputEventRequestInputEvents( uint32_t event_classes )
{
  return g_NaBlackInputEvent->RequestInputEvents(g_NaBlackInstanceId, event_classes);
}

inline int32_t
NaBlackInputEventRequestFilteringInputEvents( uint32_t event_classes )
{
  return g_NaBlackInputEvent->RequestFilteringInputEvents(g_NaBlackInstanceId, event_classes);
}

inline void
NaBlackInputEventClearInputEventRequest( uint32_t event_classes )
{
  g_NaBlackInputEvent->ClearInputEventRequest(g_NaBlackInstanceId, event_classes);
}

inline PP_Bool
NaBlackInputEventIsInputEvent( PP_Resource resource )
{
  return g_NaBlackInputEvent->IsInputEvent(resource);
}

inline PP_InputEvent_Type
NaBlackInputEventGetType( PP_Resource event )
{
  return g_NaBlackInputEvent->GetType(event);
}

inline PP_TimeTicks
NaBlackInputEventGetTimeStamp( PP_Resource event )
{
  return g_NaBlackInputEvent->GetTimeStamp(event);
}

inline uint32_t
NaBlackInputEventGetModifiers( PP_Resource event )
{
  return g_NaBlackInputEvent->GetModifiers(event);
}


// --------------------------------------------------------------------------------------------
// PPB_Instance inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Bool
NaBlackInstanceBindGraphics( PP_Resource device )
{
  return g_NaBlackInstance->BindGraphics(g_NaBlackInstanceId, device);
}

inline PP_Bool
NaBlackInstanceIsFullFrame(  )
{
  return g_NaBlackInstance->IsFullFrame(g_NaBlackInstanceId);
}


// --------------------------------------------------------------------------------------------
// PPB_KeyboardInputEvent inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackKeyboardInputEventCreate( PP_InputEvent_Type type, PP_TimeTicks time_stamp, uint32_t modifiers, uint32_t key_code, PP_Var character_text )
{
  return g_NaBlackKeyboardInputEvent->Create(g_NaBlackInstanceId, type, time_stamp, modifiers, key_code, character_text);
}

inline PP_Bool
NaBlackKeyboardInputEventIsKeyboardInputEvent( PP_Resource resource )
{
  return g_NaBlackKeyboardInputEvent->IsKeyboardInputEvent(resource);
}

inline uint32_t
NaBlackKeyboardInputEventGetKeyCode( PP_Resource key_event )
{
  return g_NaBlackKeyboardInputEvent->GetKeyCode(key_event);
}

inline PP_Var
NaBlackKeyboardInputEventGetCharacterText( PP_Resource character_event )
{
  return g_NaBlackKeyboardInputEvent->GetCharacterText(character_event);
}


// --------------------------------------------------------------------------------------------
// PPB_Messaging inline definitions
// --------------------------------------------------------------------------------------------
inline void
NaBlackMessagingPostMessage( PP_Var message )
{
  g_NaBlackMessaging->PostMessage(g_NaBlackInstanceId, message);
}


// --------------------------------------------------------------------------------------------
// PPB_MouseInputEvent inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackMouseInputEventCreate( PP_InputEvent_Type type, PP_TimeTicks time_stamp, uint32_t modifiers, PP_InputEvent_MouseButton mouse_button, const struct PP_Point* mouse_position, int32_t click_count, const struct PP_Point* mouse_movement )
{
  return g_NaBlackMouseInputEvent->Create(g_NaBlackInstanceId, type, time_stamp, modifiers, mouse_button, mouse_position, click_count, mouse_movement);
}

inline PP_Bool
NaBlackMouseInputEventIsMouseInputEvent( PP_Resource resource )
{
  return g_NaBlackMouseInputEvent->IsMouseInputEvent(resource);
}

inline PP_InputEvent_MouseButton
NaBlackMouseInputEventGetButton( PP_Resource mouse_event )
{
  return g_NaBlackMouseInputEvent->GetButton(mouse_event);
}

inline PP_Point
NaBlackMouseInputEventGetPosition( PP_Resource mouse_event )
{
  return g_NaBlackMouseInputEvent->GetPosition(mouse_event);
}

inline int32_t
NaBlackMouseInputEventGetClickCount( PP_Resource mouse_event )
{
  return g_NaBlackMouseInputEvent->GetClickCount(mouse_event);
}

inline PP_Point
NaBlackMouseInputEventGetMovement( PP_Resource mouse_event )
{
  return g_NaBlackMouseInputEvent->GetMovement(mouse_event);
}


// --------------------------------------------------------------------------------------------
// PPB_MouseLock inline definitions
// --------------------------------------------------------------------------------------------
inline int32_t
NaBlackMouseLockLockMouse( PP_CompletionCallback callback )
{
  return g_NaBlackMouseLock->LockMouse(g_NaBlackInstanceId, callback);
}

inline void
NaBlackMouseLockUnlockMouse(  )
{
  g_NaBlackMouseLock->UnlockMouse(g_NaBlackInstanceId);
}


// --------------------------------------------------------------------------------------------
// PPB_OpenGLES2 inline definitions
// --------------------------------------------------------------------------------------------
inline void
NaBlackOpenGLES2ActiveTexture( PP_Resource context, GLenum texture )
{
  g_NaBlackOpenGLES2->ActiveTexture(context, texture);
}

inline void
NaBlackOpenGLES2AttachShader( PP_Resource context, GLuint program, GLuint shader )
{
  g_NaBlackOpenGLES2->AttachShader(context, program, shader);
}

inline void
NaBlackOpenGLES2BindAttribLocation( PP_Resource context, GLuint program, GLuint index, const char* name )
{
  g_NaBlackOpenGLES2->BindAttribLocation(context, program, index, name);
}

inline void
NaBlackOpenGLES2BindBuffer( PP_Resource context, GLenum target, GLuint buffer )
{
  g_NaBlackOpenGLES2->BindBuffer(context, target, buffer);
}

inline void
NaBlackOpenGLES2BindFramebuffer( PP_Resource context, GLenum target, GLuint framebuffer )
{
  g_NaBlackOpenGLES2->BindFramebuffer(context, target, framebuffer);
}

inline void
NaBlackOpenGLES2BindRenderbuffer( PP_Resource context, GLenum target, GLuint renderbuffer )
{
  g_NaBlackOpenGLES2->BindRenderbuffer(context, target, renderbuffer);
}

inline void
NaBlackOpenGLES2BindTexture( PP_Resource context, GLenum target, GLuint texture )
{
  g_NaBlackOpenGLES2->BindTexture(context, target, texture);
}

inline void
NaBlackOpenGLES2BlendColor( PP_Resource context, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
  g_NaBlackOpenGLES2->BlendColor(context, red, green, blue, alpha);
}

inline void
NaBlackOpenGLES2BlendEquation( PP_Resource context, GLenum mode )
{
  g_NaBlackOpenGLES2->BlendEquation(context, mode);
}

inline void
NaBlackOpenGLES2BlendEquationSeparate( PP_Resource context, GLenum modeRGB, GLenum modeAlpha )
{
  g_NaBlackOpenGLES2->BlendEquationSeparate(context, modeRGB, modeAlpha);
}

inline void
NaBlackOpenGLES2BlendFunc( PP_Resource context, GLenum sfactor, GLenum dfactor )
{
  g_NaBlackOpenGLES2->BlendFunc(context, sfactor, dfactor);
}

inline void
NaBlackOpenGLES2BlendFuncSeparate( PP_Resource context, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha )
{
  g_NaBlackOpenGLES2->BlendFuncSeparate(context, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

inline void
NaBlackOpenGLES2BufferData( PP_Resource context, GLenum target, GLsizeiptr size, const void* data, GLenum usage )
{
  g_NaBlackOpenGLES2->BufferData(context, target, size, data, usage);
}

inline void
NaBlackOpenGLES2BufferSubData( PP_Resource context, GLenum target, GLintptr offset, GLsizeiptr size, const void* data )
{
  g_NaBlackOpenGLES2->BufferSubData(context, target, offset, size, data);
}

inline GLenum
NaBlackOpenGLES2CheckFramebufferStatus( PP_Resource context, GLenum target )
{
  return g_NaBlackOpenGLES2->CheckFramebufferStatus(context, target);
}

inline void
NaBlackOpenGLES2Clear( PP_Resource context, GLbitfield mask )
{
  g_NaBlackOpenGLES2->Clear(context, mask);
}

inline void
NaBlackOpenGLES2ClearColor( PP_Resource context, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
  g_NaBlackOpenGLES2->ClearColor(context, red, green, blue, alpha);
}

inline void
NaBlackOpenGLES2ClearDepthf( PP_Resource context, GLclampf depth )
{
  g_NaBlackOpenGLES2->ClearDepthf(context, depth);
}

inline void
NaBlackOpenGLES2ClearStencil( PP_Resource context, GLint s )
{
  g_NaBlackOpenGLES2->ClearStencil(context, s);
}

inline void
NaBlackOpenGLES2ColorMask( PP_Resource context, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
  g_NaBlackOpenGLES2->ColorMask(context, red, green, blue, alpha);
}

inline void
NaBlackOpenGLES2CompileShader( PP_Resource context, GLuint shader )
{
  g_NaBlackOpenGLES2->CompileShader(context, shader);
}

inline void
NaBlackOpenGLES2CompressedTexImage2D( PP_Resource context, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data )
{
  g_NaBlackOpenGLES2->CompressedTexImage2D(context, target, level, internalformat, width, height, border, imageSize, data);
}

inline void
NaBlackOpenGLES2CompressedTexSubImage2D( PP_Resource context, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data )
{
  g_NaBlackOpenGLES2->CompressedTexSubImage2D(context, target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

inline void
NaBlackOpenGLES2CopyTexImage2D( PP_Resource context, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border )
{
  g_NaBlackOpenGLES2->CopyTexImage2D(context, target, level, internalformat, x, y, width, height, border);
}

inline void
NaBlackOpenGLES2CopyTexSubImage2D( PP_Resource context, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
  g_NaBlackOpenGLES2->CopyTexSubImage2D(context, target, level, xoffset, yoffset, x, y, width, height);
}

inline GLuint
NaBlackOpenGLES2CreateProgram( PP_Resource context )
{
  return g_NaBlackOpenGLES2->CreateProgram(context);
}

inline GLuint
NaBlackOpenGLES2CreateShader( PP_Resource context, GLenum type )
{
  return g_NaBlackOpenGLES2->CreateShader(context, type);
}

inline void
NaBlackOpenGLES2CullFace( PP_Resource context, GLenum mode )
{
  g_NaBlackOpenGLES2->CullFace(context, mode);
}

inline void
NaBlackOpenGLES2DeleteBuffers( PP_Resource context, GLsizei n, const GLuint* buffers )
{
  g_NaBlackOpenGLES2->DeleteBuffers(context, n, buffers);
}

inline void
NaBlackOpenGLES2DeleteFramebuffers( PP_Resource context, GLsizei n, const GLuint* framebuffers )
{
  g_NaBlackOpenGLES2->DeleteFramebuffers(context, n, framebuffers);
}

inline void
NaBlackOpenGLES2DeleteProgram( PP_Resource context, GLuint program )
{
  g_NaBlackOpenGLES2->DeleteProgram(context, program);
}

inline void
NaBlackOpenGLES2DeleteRenderbuffers( PP_Resource context, GLsizei n, const GLuint* renderbuffers )
{
  g_NaBlackOpenGLES2->DeleteRenderbuffers(context, n, renderbuffers);
}

inline void
NaBlackOpenGLES2DeleteShader( PP_Resource context, GLuint shader )
{
  g_NaBlackOpenGLES2->DeleteShader(context, shader);
}

inline void
NaBlackOpenGLES2DeleteTextures( PP_Resource context, GLsizei n, const GLuint* textures )
{
  g_NaBlackOpenGLES2->DeleteTextures(context, n, textures);
}

inline void
NaBlackOpenGLES2DepthFunc( PP_Resource context, GLenum func )
{
  g_NaBlackOpenGLES2->DepthFunc(context, func);
}

inline void
NaBlackOpenGLES2DepthMask( PP_Resource context, GLboolean flag )
{
  g_NaBlackOpenGLES2->DepthMask(context, flag);
}

inline void
NaBlackOpenGLES2DepthRangef( PP_Resource context, GLclampf zNear, GLclampf zFar )
{
  g_NaBlackOpenGLES2->DepthRangef(context, zNear, zFar);
}

inline void
NaBlackOpenGLES2DetachShader( PP_Resource context, GLuint program, GLuint shader )
{
  g_NaBlackOpenGLES2->DetachShader(context, program, shader);
}

inline void
NaBlackOpenGLES2Disable( PP_Resource context, GLenum cap )
{
  g_NaBlackOpenGLES2->Disable(context, cap);
}

inline void
NaBlackOpenGLES2DisableVertexAttribArray( PP_Resource context, GLuint index )
{
  g_NaBlackOpenGLES2->DisableVertexAttribArray(context, index);
}

inline void
NaBlackOpenGLES2DrawArrays( PP_Resource context, GLenum mode, GLint first, GLsizei count )
{
  g_NaBlackOpenGLES2->DrawArrays(context, mode, first, count);
}

inline void
NaBlackOpenGLES2DrawElements( PP_Resource context, GLenum mode, GLsizei count, GLenum type, const void* indices )
{
  g_NaBlackOpenGLES2->DrawElements(context, mode, count, type, indices);
}

inline void
NaBlackOpenGLES2Enable( PP_Resource context, GLenum cap )
{
  g_NaBlackOpenGLES2->Enable(context, cap);
}

inline void
NaBlackOpenGLES2EnableVertexAttribArray( PP_Resource context, GLuint index )
{
  g_NaBlackOpenGLES2->EnableVertexAttribArray(context, index);
}

inline void
NaBlackOpenGLES2Finish( PP_Resource context )
{
  g_NaBlackOpenGLES2->Finish(context);
}

inline void
NaBlackOpenGLES2Flush( PP_Resource context )
{
  g_NaBlackOpenGLES2->Flush(context);
}

inline void
NaBlackOpenGLES2FramebufferRenderbuffer( PP_Resource context, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
{
  g_NaBlackOpenGLES2->FramebufferRenderbuffer(context, target, attachment, renderbuffertarget, renderbuffer);
}

inline void
NaBlackOpenGLES2FramebufferTexture2D( PP_Resource context, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
  g_NaBlackOpenGLES2->FramebufferTexture2D(context, target, attachment, textarget, texture, level);
}

inline void
NaBlackOpenGLES2FrontFace( PP_Resource context, GLenum mode )
{
  g_NaBlackOpenGLES2->FrontFace(context, mode);
}

inline void
NaBlackOpenGLES2GenBuffers( PP_Resource context, GLsizei n, GLuint* buffers )
{
  g_NaBlackOpenGLES2->GenBuffers(context, n, buffers);
}

inline void
NaBlackOpenGLES2GenerateMipmap( PP_Resource context, GLenum target )
{
  g_NaBlackOpenGLES2->GenerateMipmap(context, target);
}

inline void
NaBlackOpenGLES2GenFramebuffers( PP_Resource context, GLsizei n, GLuint* framebuffers )
{
  g_NaBlackOpenGLES2->GenFramebuffers(context, n, framebuffers);
}

inline void
NaBlackOpenGLES2GenRenderbuffers( PP_Resource context, GLsizei n, GLuint* renderbuffers )
{
  g_NaBlackOpenGLES2->GenRenderbuffers(context, n, renderbuffers);
}

inline void
NaBlackOpenGLES2GenTextures( PP_Resource context, GLsizei n, GLuint* textures )
{
  g_NaBlackOpenGLES2->GenTextures(context, n, textures);
}

inline void
NaBlackOpenGLES2GetActiveAttrib( PP_Resource context, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name )
{
  g_NaBlackOpenGLES2->GetActiveAttrib(context, program, index, bufsize, length, size, type, name);
}

inline void
NaBlackOpenGLES2GetActiveUniform( PP_Resource context, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name )
{
  g_NaBlackOpenGLES2->GetActiveUniform(context, program, index, bufsize, length, size, type, name);
}

inline void
NaBlackOpenGLES2GetAttachedShaders( PP_Resource context, GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders )
{
  g_NaBlackOpenGLES2->GetAttachedShaders(context, program, maxcount, count, shaders);
}

inline GLint
NaBlackOpenGLES2GetAttribLocation( PP_Resource context, GLuint program, const char* name )
{
  return g_NaBlackOpenGLES2->GetAttribLocation(context, program, name);
}

inline void
NaBlackOpenGLES2GetBooleanv( PP_Resource context, GLenum pname, GLboolean* params )
{
  g_NaBlackOpenGLES2->GetBooleanv(context, pname, params);
}

inline void
NaBlackOpenGLES2GetBufferParameteriv( PP_Resource context, GLenum target, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetBufferParameteriv(context, target, pname, params);
}

inline GLenum
NaBlackOpenGLES2GetError( PP_Resource context )
{
  return g_NaBlackOpenGLES2->GetError(context);
}

inline void
NaBlackOpenGLES2GetFloatv( PP_Resource context, GLenum pname, GLfloat* params )
{
  g_NaBlackOpenGLES2->GetFloatv(context, pname, params);
}

inline void
NaBlackOpenGLES2GetFramebufferAttachmentParameteriv( PP_Resource context, GLenum target, GLenum attachment, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetFramebufferAttachmentParameteriv(context, target, attachment, pname, params);
}

inline void
NaBlackOpenGLES2GetIntegerv( PP_Resource context, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetIntegerv(context, pname, params);
}

inline void
NaBlackOpenGLES2GetProgramiv( PP_Resource context, GLuint program, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetProgramiv(context, program, pname, params);
}

inline void
NaBlackOpenGLES2GetProgramInfoLog( PP_Resource context, GLuint program, GLsizei bufsize, GLsizei* length, char* infolog )
{
  g_NaBlackOpenGLES2->GetProgramInfoLog(context, program, bufsize, length, infolog);
}

inline void
NaBlackOpenGLES2GetRenderbufferParameteriv( PP_Resource context, GLenum target, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetRenderbufferParameteriv(context, target, pname, params);
}

inline void
NaBlackOpenGLES2GetShaderiv( PP_Resource context, GLuint shader, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetShaderiv(context, shader, pname, params);
}

inline void
NaBlackOpenGLES2GetShaderInfoLog( PP_Resource context, GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog )
{
  g_NaBlackOpenGLES2->GetShaderInfoLog(context, shader, bufsize, length, infolog);
}

inline void
NaBlackOpenGLES2GetShaderPrecisionFormat( PP_Resource context, GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision )
{
  g_NaBlackOpenGLES2->GetShaderPrecisionFormat(context, shadertype, precisiontype, range, precision);
}

inline void
NaBlackOpenGLES2GetShaderSource( PP_Resource context, GLuint shader, GLsizei bufsize, GLsizei* length, char* source )
{
  g_NaBlackOpenGLES2->GetShaderSource(context, shader, bufsize, length, source);
}

inline const GLubyte*
NaBlackOpenGLES2GetString( PP_Resource context, GLenum name )
{
  return g_NaBlackOpenGLES2->GetString(context, name);
}

inline void
NaBlackOpenGLES2GetTexParameterfv( PP_Resource context, GLenum target, GLenum pname, GLfloat* params )
{
  g_NaBlackOpenGLES2->GetTexParameterfv(context, target, pname, params);
}

inline void
NaBlackOpenGLES2GetTexParameteriv( PP_Resource context, GLenum target, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetTexParameteriv(context, target, pname, params);
}

inline void
NaBlackOpenGLES2GetUniformfv( PP_Resource context, GLuint program, GLint location, GLfloat* params )
{
  g_NaBlackOpenGLES2->GetUniformfv(context, program, location, params);
}

inline void
NaBlackOpenGLES2GetUniformiv( PP_Resource context, GLuint program, GLint location, GLint* params )
{
  g_NaBlackOpenGLES2->GetUniformiv(context, program, location, params);
}

inline GLint
NaBlackOpenGLES2GetUniformLocation( PP_Resource context, GLuint program, const char* name )
{
  return g_NaBlackOpenGLES2->GetUniformLocation(context, program, name);
}

inline void
NaBlackOpenGLES2GetVertexAttribfv( PP_Resource context, GLuint index, GLenum pname, GLfloat* params )
{
  g_NaBlackOpenGLES2->GetVertexAttribfv(context, index, pname, params);
}

inline void
NaBlackOpenGLES2GetVertexAttribiv( PP_Resource context, GLuint index, GLenum pname, GLint* params )
{
  g_NaBlackOpenGLES2->GetVertexAttribiv(context, index, pname, params);
}

inline void
NaBlackOpenGLES2GetVertexAttribPointerv( PP_Resource context, GLuint index, GLenum pname, void** pointer )
{
  g_NaBlackOpenGLES2->GetVertexAttribPointerv(context, index, pname, pointer);
}

inline void
NaBlackOpenGLES2Hint( PP_Resource context, GLenum target, GLenum mode )
{
  g_NaBlackOpenGLES2->Hint(context, target, mode);
}

inline GLboolean
NaBlackOpenGLES2IsBuffer( PP_Resource context, GLuint buffer )
{
  return g_NaBlackOpenGLES2->IsBuffer(context, buffer);
}

inline GLboolean
NaBlackOpenGLES2IsEnabled( PP_Resource context, GLenum cap )
{
  return g_NaBlackOpenGLES2->IsEnabled(context, cap);
}

inline GLboolean
NaBlackOpenGLES2IsFramebuffer( PP_Resource context, GLuint framebuffer )
{
  return g_NaBlackOpenGLES2->IsFramebuffer(context, framebuffer);
}

inline GLboolean
NaBlackOpenGLES2IsProgram( PP_Resource context, GLuint program )
{
  return g_NaBlackOpenGLES2->IsProgram(context, program);
}

inline GLboolean
NaBlackOpenGLES2IsRenderbuffer( PP_Resource context, GLuint renderbuffer )
{
  return g_NaBlackOpenGLES2->IsRenderbuffer(context, renderbuffer);
}

inline GLboolean
NaBlackOpenGLES2IsShader( PP_Resource context, GLuint shader )
{
  return g_NaBlackOpenGLES2->IsShader(context, shader);
}

inline GLboolean
NaBlackOpenGLES2IsTexture( PP_Resource context, GLuint texture )
{
  return g_NaBlackOpenGLES2->IsTexture(context, texture);
}

inline void
NaBlackOpenGLES2LineWidth( PP_Resource context, GLfloat width )
{
  g_NaBlackOpenGLES2->LineWidth(context, width);
}

inline void
NaBlackOpenGLES2LinkProgram( PP_Resource context, GLuint program )
{
  g_NaBlackOpenGLES2->LinkProgram(context, program);
}

inline void
NaBlackOpenGLES2PixelStorei( PP_Resource context, GLenum pname, GLint param )
{
  g_NaBlackOpenGLES2->PixelStorei(context, pname, param);
}

inline void
NaBlackOpenGLES2PolygonOffset( PP_Resource context, GLfloat factor, GLfloat units )
{
  g_NaBlackOpenGLES2->PolygonOffset(context, factor, units);
}

inline void
NaBlackOpenGLES2ReadPixels( PP_Resource context, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels )
{
  g_NaBlackOpenGLES2->ReadPixels(context, x, y, width, height, format, type, pixels);
}

inline void
NaBlackOpenGLES2ReleaseShaderCompiler( PP_Resource context )
{
  g_NaBlackOpenGLES2->ReleaseShaderCompiler(context);
}

inline void
NaBlackOpenGLES2RenderbufferStorage( PP_Resource context, GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
{
  g_NaBlackOpenGLES2->RenderbufferStorage(context, target, internalformat, width, height);
}

inline void
NaBlackOpenGLES2SampleCoverage( PP_Resource context, GLclampf value, GLboolean invert )
{
  g_NaBlackOpenGLES2->SampleCoverage(context, value, invert);
}

inline void
NaBlackOpenGLES2Scissor( PP_Resource context, GLint x, GLint y, GLsizei width, GLsizei height )
{
  g_NaBlackOpenGLES2->Scissor(context, x, y, width, height);
}

inline void
NaBlackOpenGLES2ShaderBinary( PP_Resource context, GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length )
{
  g_NaBlackOpenGLES2->ShaderBinary(context, n, shaders, binaryformat, binary, length);
}

inline void
NaBlackOpenGLES2ShaderSource( PP_Resource context, GLuint shader, GLsizei count, const char** str, const GLint* length )
{
  g_NaBlackOpenGLES2->ShaderSource(context, shader, count, str, length);
}

inline void
NaBlackOpenGLES2StencilFunc( PP_Resource context, GLenum func, GLint ref, GLuint mask )
{
  g_NaBlackOpenGLES2->StencilFunc(context, func, ref, mask);
}

inline void
NaBlackOpenGLES2StencilFuncSeparate( PP_Resource context, GLenum face, GLenum func, GLint ref, GLuint mask )
{
  g_NaBlackOpenGLES2->StencilFuncSeparate(context, face, func, ref, mask);
}

inline void
NaBlackOpenGLES2StencilMask( PP_Resource context, GLuint mask )
{
  g_NaBlackOpenGLES2->StencilMask(context, mask);
}

inline void
NaBlackOpenGLES2StencilMaskSeparate( PP_Resource context, GLenum face, GLuint mask )
{
  g_NaBlackOpenGLES2->StencilMaskSeparate(context, face, mask);
}

inline void
NaBlackOpenGLES2StencilOp( PP_Resource context, GLenum fail, GLenum zfail, GLenum zpass )
{
  g_NaBlackOpenGLES2->StencilOp(context, fail, zfail, zpass);
}

inline void
NaBlackOpenGLES2StencilOpSeparate( PP_Resource context, GLenum face, GLenum fail, GLenum zfail, GLenum zpass )
{
  g_NaBlackOpenGLES2->StencilOpSeparate(context, face, fail, zfail, zpass);
}

inline void
NaBlackOpenGLES2TexImage2D( PP_Resource context, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels )
{
  g_NaBlackOpenGLES2->TexImage2D(context, target, level, internalformat, width, height, border, format, type, pixels);
}

inline void
NaBlackOpenGLES2TexParameterf( PP_Resource context, GLenum target, GLenum pname, GLfloat param )
{
  g_NaBlackOpenGLES2->TexParameterf(context, target, pname, param);
}

inline void
NaBlackOpenGLES2TexParameterfv( PP_Resource context, GLenum target, GLenum pname, const GLfloat* params )
{
  g_NaBlackOpenGLES2->TexParameterfv(context, target, pname, params);
}

inline void
NaBlackOpenGLES2TexParameteri( PP_Resource context, GLenum target, GLenum pname, GLint param )
{
  g_NaBlackOpenGLES2->TexParameteri(context, target, pname, param);
}

inline void
NaBlackOpenGLES2TexParameteriv( PP_Resource context, GLenum target, GLenum pname, const GLint* params )
{
  g_NaBlackOpenGLES2->TexParameteriv(context, target, pname, params);
}

inline void
NaBlackOpenGLES2TexSubImage2D( PP_Resource context, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels )
{
  g_NaBlackOpenGLES2->TexSubImage2D(context, target, level, xoffset, yoffset, width, height, format, type, pixels);
}

inline void
NaBlackOpenGLES2Uniform1f( PP_Resource context, GLint location, GLfloat x )
{
  g_NaBlackOpenGLES2->Uniform1f(context, location, x);
}

inline void
NaBlackOpenGLES2Uniform1fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v )
{
  g_NaBlackOpenGLES2->Uniform1fv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform1i( PP_Resource context, GLint location, GLint x )
{
  g_NaBlackOpenGLES2->Uniform1i(context, location, x);
}

inline void
NaBlackOpenGLES2Uniform1iv( PP_Resource context, GLint location, GLsizei count, const GLint* v )
{
  g_NaBlackOpenGLES2->Uniform1iv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform2f( PP_Resource context, GLint location, GLfloat x, GLfloat y )
{
  g_NaBlackOpenGLES2->Uniform2f(context, location, x, y);
}

inline void
NaBlackOpenGLES2Uniform2fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v )
{
  g_NaBlackOpenGLES2->Uniform2fv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform2i( PP_Resource context, GLint location, GLint x, GLint y )
{
  g_NaBlackOpenGLES2->Uniform2i(context, location, x, y);
}

inline void
NaBlackOpenGLES2Uniform2iv( PP_Resource context, GLint location, GLsizei count, const GLint* v )
{
  g_NaBlackOpenGLES2->Uniform2iv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform3f( PP_Resource context, GLint location, GLfloat x, GLfloat y, GLfloat z )
{
  g_NaBlackOpenGLES2->Uniform3f(context, location, x, y, z);
}

inline void
NaBlackOpenGLES2Uniform3fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v )
{
  g_NaBlackOpenGLES2->Uniform3fv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform3i( PP_Resource context, GLint location, GLint x, GLint y, GLint z )
{
  g_NaBlackOpenGLES2->Uniform3i(context, location, x, y, z);
}

inline void
NaBlackOpenGLES2Uniform3iv( PP_Resource context, GLint location, GLsizei count, const GLint* v )
{
  g_NaBlackOpenGLES2->Uniform3iv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform4f( PP_Resource context, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
  g_NaBlackOpenGLES2->Uniform4f(context, location, x, y, z, w);
}

inline void
NaBlackOpenGLES2Uniform4fv( PP_Resource context, GLint location, GLsizei count, const GLfloat* v )
{
  g_NaBlackOpenGLES2->Uniform4fv(context, location, count, v);
}

inline void
NaBlackOpenGLES2Uniform4i( PP_Resource context, GLint location, GLint x, GLint y, GLint z, GLint w )
{
  g_NaBlackOpenGLES2->Uniform4i(context, location, x, y, z, w);
}

inline void
NaBlackOpenGLES2Uniform4iv( PP_Resource context, GLint location, GLsizei count, const GLint* v )
{
  g_NaBlackOpenGLES2->Uniform4iv(context, location, count, v);
}

inline void
NaBlackOpenGLES2UniformMatrix2fv( PP_Resource context, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value )
{
  g_NaBlackOpenGLES2->UniformMatrix2fv(context, location, count, transpose, value);
}

inline void
NaBlackOpenGLES2UniformMatrix3fv( PP_Resource context, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value )
{
  g_NaBlackOpenGLES2->UniformMatrix3fv(context, location, count, transpose, value);
}

inline void
NaBlackOpenGLES2UniformMatrix4fv( PP_Resource context, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value )
{
  g_NaBlackOpenGLES2->UniformMatrix4fv(context, location, count, transpose, value);
}

inline void
NaBlackOpenGLES2UseProgram( PP_Resource context, GLuint program )
{
  g_NaBlackOpenGLES2->UseProgram(context, program);
}

inline void
NaBlackOpenGLES2ValidateProgram( PP_Resource context, GLuint program )
{
  g_NaBlackOpenGLES2->ValidateProgram(context, program);
}

inline void
NaBlackOpenGLES2VertexAttrib1f( PP_Resource context, GLuint indx, GLfloat x )
{
  g_NaBlackOpenGLES2->VertexAttrib1f(context, indx, x);
}

inline void
NaBlackOpenGLES2VertexAttrib1fv( PP_Resource context, GLuint indx, const GLfloat* values )
{
  g_NaBlackOpenGLES2->VertexAttrib1fv(context, indx, values);
}

inline void
NaBlackOpenGLES2VertexAttrib2f( PP_Resource context, GLuint indx, GLfloat x, GLfloat y )
{
  g_NaBlackOpenGLES2->VertexAttrib2f(context, indx, x, y);
}

inline void
NaBlackOpenGLES2VertexAttrib2fv( PP_Resource context, GLuint indx, const GLfloat* values )
{
  g_NaBlackOpenGLES2->VertexAttrib2fv(context, indx, values);
}

inline void
NaBlackOpenGLES2VertexAttrib3f( PP_Resource context, GLuint indx, GLfloat x, GLfloat y, GLfloat z )
{
  g_NaBlackOpenGLES2->VertexAttrib3f(context, indx, x, y, z);
}

inline void
NaBlackOpenGLES2VertexAttrib3fv( PP_Resource context, GLuint indx, const GLfloat* values )
{
  g_NaBlackOpenGLES2->VertexAttrib3fv(context, indx, values);
}

inline void
NaBlackOpenGLES2VertexAttrib4f( PP_Resource context, GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
  g_NaBlackOpenGLES2->VertexAttrib4f(context, indx, x, y, z, w);
}

inline void
NaBlackOpenGLES2VertexAttrib4fv( PP_Resource context, GLuint indx, const GLfloat* values )
{
  g_NaBlackOpenGLES2->VertexAttrib4fv(context, indx, values);
}

inline void
NaBlackOpenGLES2VertexAttribPointer( PP_Resource context, GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr )
{
  g_NaBlackOpenGLES2->VertexAttribPointer(context, indx, size, type, normalized, stride, ptr);
}

inline void
NaBlackOpenGLES2Viewport( PP_Resource context, GLint x, GLint y, GLsizei width, GLsizei height )
{
  g_NaBlackOpenGLES2->Viewport(context, x, y, width, height);
}


// --------------------------------------------------------------------------------------------
// PPB_URLLoader inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackURLLoaderCreate(  )
{
  return g_NaBlackURLLoader->Create(g_NaBlackInstanceId);
}

inline PP_Bool
NaBlackURLLoaderIsURLLoader( PP_Resource resource )
{
  return g_NaBlackURLLoader->IsURLLoader(resource);
}

inline int32_t
NaBlackURLLoaderOpen( PP_Resource loader, PP_Resource request_info, PP_CompletionCallback callback )
{
  return g_NaBlackURLLoader->Open(loader, request_info, callback);
}

inline int32_t
NaBlackURLLoaderFollowRedirect( PP_Resource loader, PP_CompletionCallback callback )
{
  return g_NaBlackURLLoader->FollowRedirect(loader, callback);
}

inline PP_Bool
NaBlackURLLoaderGetUploadProgress( PP_Resource loader, int64_t* bytes_sent, int64_t* total_bytes_to_be_sent )
{
  return g_NaBlackURLLoader->GetUploadProgress(loader, bytes_sent, total_bytes_to_be_sent);
}

inline PP_Bool
NaBlackURLLoaderGetDownloadProgress( PP_Resource loader, int64_t* bytes_received, int64_t* total_bytes_to_be_received )
{
  return g_NaBlackURLLoader->GetDownloadProgress(loader, bytes_received, total_bytes_to_be_received);
}

inline PP_Resource
NaBlackURLLoaderGetResponseInfo( PP_Resource loader )
{
  return g_NaBlackURLLoader->GetResponseInfo(loader);
}

inline int32_t
NaBlackURLLoaderReadResponseBody( PP_Resource loader, void* buffer, int32_t bytes_to_read, PP_CompletionCallback callback )
{
  return g_NaBlackURLLoader->ReadResponseBody(loader, buffer, bytes_to_read, callback);
}

inline int32_t
NaBlackURLLoaderFinishStreamingToFile( PP_Resource loader, PP_CompletionCallback callback )
{
  return g_NaBlackURLLoader->FinishStreamingToFile(loader, callback);
}

inline void
NaBlackURLLoaderClose( PP_Resource loader )
{
  g_NaBlackURLLoader->Close(loader);
}


// --------------------------------------------------------------------------------------------
// PPB_URLRequestInfo inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackURLRequestInfoCreate(  )
{
  return g_NaBlackURLRequestInfo->Create(g_NaBlackInstanceId);
}

inline PP_Bool
NaBlackURLRequestInfoIsURLRequestInfo( PP_Resource resource )
{
  return g_NaBlackURLRequestInfo->IsURLRequestInfo(resource);
}

inline PP_Bool
NaBlackURLRequestInfoSetProperty( PP_Resource request, PP_URLRequestProperty property, PP_Var value )
{
  return g_NaBlackURLRequestInfo->SetProperty(request, property, value);
}

inline PP_Bool
NaBlackURLRequestInfoAppendDataToBody( PP_Resource request, const void* data, uint32_t len )
{
  return g_NaBlackURLRequestInfo->AppendDataToBody(request, data, len);
}

inline PP_Bool
NaBlackURLRequestInfoAppendFileToBody( PP_Resource request, PP_Resource file_ref, int64_t start_offset, int64_t number_of_bytes, PP_Time expected_last_modified_time )
{
  return g_NaBlackURLRequestInfo->AppendFileToBody(request, file_ref, start_offset, number_of_bytes, expected_last_modified_time);
}


// --------------------------------------------------------------------------------------------
// PPB_URLResponseInfo inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Bool
NaBlackURLResponseInfoIsURLResponseInfo( PP_Resource resource )
{
  return g_NaBlackURLResponseInfo->IsURLResponseInfo(resource);
}

inline PP_Var
NaBlackURLResponseInfoGetProperty( PP_Resource response, PP_URLResponseProperty property )
{
  return g_NaBlackURLResponseInfo->GetProperty(response, property);
}

inline PP_Resource
NaBlackURLResponseInfoGetBodyAsFileRef( PP_Resource response )
{
  return g_NaBlackURLResponseInfo->GetBodyAsFileRef(response);
}


// --------------------------------------------------------------------------------------------
// PPB_Var inline definitions
// --------------------------------------------------------------------------------------------
inline void
NaBlackVarAddRef( PP_Var var )
{
  g_NaBlackVar->AddRef(var);
}

inline void
NaBlackVarRelease( PP_Var var )
{
  g_NaBlackVar->Release(var);
}

inline PP_Var
NaBlackVarVarFromUtf8( const char* data, uint32_t len )
{
  return g_NaBlackVar->VarFromUtf8(data, len);
}

inline const char*
NaBlackVarVarToUtf8( PP_Var var, uint32_t* len )
{
  return g_NaBlackVar->VarToUtf8(var, len);
}


// --------------------------------------------------------------------------------------------
// PPB_VarArrayBuffer inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Var
NaBlackVarArrayBufferCreate( uint32_t size_in_bytes )
{
  return g_NaBlackVarArrayBuffer->Create(size_in_bytes);
}

inline PP_Bool
NaBlackVarArrayBufferByteLength( PP_Var array, uint32_t* byte_length )
{
  return g_NaBlackVarArrayBuffer->ByteLength(array, byte_length);
}

inline void*
NaBlackVarArrayBufferMap( PP_Var array )
{
  return g_NaBlackVarArrayBuffer->Map(array);
}

inline void
NaBlackVarArrayBufferUnmap( PP_Var array )
{
  g_NaBlackVarArrayBuffer->Unmap(array);
}


// --------------------------------------------------------------------------------------------
// PPB_View inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Bool
NaBlackViewIsView( PP_Resource resource )
{
  return g_NaBlackView->IsView(resource);
}

inline PP_Bool
NaBlackViewGetRect( PP_Resource resource, struct PP_Rect* rect )
{
  return g_NaBlackView->GetRect(resource, rect);
}

inline PP_Bool
NaBlackViewIsFullscreen( PP_Resource resource )
{
  return g_NaBlackView->IsFullscreen(resource);
}

inline PP_Bool
NaBlackViewIsVisible( PP_Resource resource )
{
  return g_NaBlackView->IsVisible(resource);
}

inline PP_Bool
NaBlackViewIsPageVisible( PP_Resource resource )
{
  return g_NaBlackView->IsPageVisible(resource);
}

inline PP_Bool
NaBlackViewGetClipRect( PP_Resource resource, struct PP_Rect* clip )
{
  return g_NaBlackView->GetClipRect(resource, clip);
}


// --------------------------------------------------------------------------------------------
// PPB_WebSocket inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackWebSocketCreate(  )
{
  return g_NaBlackWebSocket->Create(g_NaBlackInstanceId);
}

inline PP_Bool
NaBlackWebSocketIsWebSocket( PP_Resource resource )
{
  return g_NaBlackWebSocket->IsWebSocket(resource);
}

inline int32_t
NaBlackWebSocketConnect( PP_Resource web_socket, PP_Var url, const struct PP_Var protocols[], uint32_t protocol_count, PP_CompletionCallback callback )
{
  return g_NaBlackWebSocket->Connect(web_socket, url, protocols, protocol_count, callback);
}

inline int32_t
NaBlackWebSocketClose( PP_Resource web_socket, uint16_t code, PP_Var reason, PP_CompletionCallback callback )
{
  return g_NaBlackWebSocket->Close(web_socket, code, reason, callback);
}

inline int32_t
NaBlackWebSocketReceiveMessage( PP_Resource web_socket, struct PP_Var* message, PP_CompletionCallback callback )
{
  return g_NaBlackWebSocket->ReceiveMessage(web_socket, message, callback);
}

inline int32_t
NaBlackWebSocketSendMessage( PP_Resource web_socket, PP_Var message )
{
  return g_NaBlackWebSocket->SendMessage(web_socket, message);
}

inline uint64_t
NaBlackWebSocketGetBufferedAmount( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetBufferedAmount(web_socket);
}

inline uint16_t
NaBlackWebSocketGetCloseCode( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetCloseCode(web_socket);
}

inline PP_Var
NaBlackWebSocketGetCloseReason( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetCloseReason(web_socket);
}

inline PP_Bool
NaBlackWebSocketGetCloseWasClean( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetCloseWasClean(web_socket);
}

inline PP_Var
NaBlackWebSocketGetExtensions( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetExtensions(web_socket);
}

inline PP_Var
NaBlackWebSocketGetProtocol( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetProtocol(web_socket);
}

inline PP_WebSocketReadyState
NaBlackWebSocketGetReadyState( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetReadyState(web_socket);
}

inline PP_Var
NaBlackWebSocketGetURL( PP_Resource web_socket )
{
  return g_NaBlackWebSocket->GetURL(web_socket);
}


// --------------------------------------------------------------------------------------------
// PPB_WheelInputEvent inline definitions
// --------------------------------------------------------------------------------------------
inline PP_Resource
NaBlackWheelInputEventCreate( PP_TimeTicks time_stamp, uint32_t modifiers, const struct PP_FloatPoint* wheel_delta, const struct PP_FloatPoint* wheel_ticks, PP_Bool scroll_by_page )
{
  return g_NaBlackWheelInputEvent->Create(g_NaBlackInstanceId, time_stamp, modifiers, wheel_delta, wheel_ticks, scroll_by_page);
}

inline PP_Bool
NaBlackWheelInputEventIsWheelInputEvent( PP_Resource resource )
{
  return g_NaBlackWheelInputEvent->IsWheelInputEvent(resource);
}

inline struct PP_FloatPoint
NaBlackWheelInputEventGetDelta( PP_Resource wheel_event )
{
  return g_NaBlackWheelInputEvent->GetDelta(wheel_event);
}

inline struct PP_FloatPoint
NaBlackWheelInputEventGetTicks( PP_Resource wheel_event )
{
  return g_NaBlackWheelInputEvent->GetTicks(wheel_event);
}

inline PP_Bool
NaBlackWheelInputEventGetScrollByPage( PP_Resource wheel_event )
{
  return g_NaBlackWheelInputEvent->GetScrollByPage(wheel_event);
}



// --------------------------------------------------------------------------------------------
// Utility functions (not part of NaCl API)
// --------------------------------------------------------------------------------------------

// #nablack-utility NaBlackMessagingPostUtf8 Post string to browser via PPAPI
void NaBlackMessagingPostUtf8(const char* msg);

// #nablack-utility NaBlackMessagingPostUtf8 Post formatted string to browser via PPAPI
void NaBlackMessagingPostPrintf( const char* format, ... );

#ifdef __cplusplus
} // extern "C"
#endif
