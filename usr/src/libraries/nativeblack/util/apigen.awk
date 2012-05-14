BEGIN {

}

function AddPPB( name ) 
{
  PPBCount++;
  PPBName[ PPBCount ]             = name;
  PPFunctionCount[ "PPB", name ]  = 0;
  PPName                          = name;
  PPType                          = "PPB";
  PPIndex                         = PPBCount;
}

function AddPPP( name ) 
{
  PPPCount++;
  PPPName[ PPPCount ]             = name;
  PPFunctionCount[ "PPP", name ]  = 0;
  PPName                          = name;
  PPType                          = "PPP";
  PPIndex                         = PPPCount;
}

function SelectPPB()
{
  PPType  = "PPB";
  PPIndex = 0;
}

function GetInterfaceCount()
{
  if ( PPType == "PPP" ) 
  {
    return PPPCount;
  }
  else if ( PPType == "PPB" )
  {
    return PPBCount;
  }
  return 0;
}

function SelectInterface( ndx )
{
  PPIndex = ndx;
  if ( PPType == "PPP" ) 
  {
    PPName = PPPName[ ndx ];
  }
  else if ( PPType == "PPB" )
  {
    PPName = PPBName[ ndx ];
  }
}

function GetInterfaceName()
{
  if ( PPType == "PPP" ) 
  {
    return PPPName[ PPIndex ];
  }
  else if ( PPType == "PPB" )
  {
    return PPBName[ PPIndex ];
  }
  return "UKNOWN";
}

function SelectPPP()
{
  PPType  = "PPP";
  PPIndex = 0;
}

function GetFunctionCount()
{
  return PPFunctionCount[ PPType, GetInterfaceName() ];
}

function SelectFunction( ndx )
{
  PPFuncIndex = ndx;
  PPFuncName  = PPFunctionName[ PPType, PPName, ndx ];
}

function GetFunctionName()
{
  return PPFuncName;
}

function GetFunctionFlag( )
{
  return PPFunctionFlag[ PPType, PPName, PPFuncName ];
}

function AddFunctionName( name )
{
  PPFunctionCount[ PPType, PPName ]++;
  PPFunctionName[ PPType, PPName, PPFunctionCount[ PPType, PPName ] ] = name;
  PPFuncName = name;
  PPFunctionFlagCount[ PPType, PPName, PPFuncName ] = 0;
  PPFunctionParamCount[ PPType, PPName, PPFuncName ] = 0;
}

function AddFunctionReturnType( type )
{
  PPFunctionReturnType[ PPType, PPName, PPFuncName ] = type;
}

function AddParam( type, name, flag )
{
  PPFunctionParamCount[ PPType, PPName, PPFuncName ]++;
  PPFunctionParamName[ PPType, PPName, PPFuncName, PPFunctionParamCount[ PPType, PPName, PPFuncName ] ] = name;
  PPFunctionParamType[ PPType, PPName, PPFuncName, PPFunctionParamCount[ PPType, PPName, PPFuncName ] ] = type;
  PPFunctionParamFlag[ PPType, PPName, PPFuncName, PPFunctionParamCount[ PPType, PPName, PPFuncName ] ] = flag;
}

function GetFunctionParamCount()
{
  return PPFunctionParamCount[ PPType, PPName, PPFuncName ];
}

function SelectFunctionParam( ndx )
{
  PPParamIndex = ndx;
  PPParamName  = PPFunctionParamName[ PPType, PPName, PPFuncName, PPParamIndex ];
  PPParamType  = PPFunctionParamType[ PPType, PPName, PPFuncName, PPParamIndex ];
  PPParamFlag  = PPFunctionParamFlag[ PPType, PPName, PPFuncName, PPParamIndex ];
}

function GetFunctionParamName()
{
  return PPParamName;
}

function CleanType(type)
{
  if ( type == "struct PP_Var" )
  {
    return "PP_Var";
  }
  if ( type == "struct PP_CompletionCallback" )
  {
    return "PP_CompletionCallback";
  }
  if ( type == "struct PP_FileInfo" )
  {
    return "PP_FileInfo";
  }
  if ( type == "struct PP_Size" )
  {
    return "PP_Size";
  }
  if ( type == "struct PP_Point" )
  {
    return "PP_Point";
  }
  if ( type == "struct PP_Rect" )
  {
    return "PP_Rect";
  }
  return type;
}

function GetFunctionReturnType( )
{
  return CleanType( PPFunctionReturnType[ PPType, PPName, PPFuncName ] );
}

function GetFunctionParamType()
{
  return CleanType( PPParamType );
}

function GetFunctionParamFlag()
{
  return PPParamFlag;
}

function SetFunctionFlag( flag )
{
  PPFunctionFlagCount[ PPType, PPName, PPFuncName ]++;
  PPFunctionFlag[ PPType, PPName, PPFuncName, PPFunctionFlagCount[ PPType, PPName, PPFuncName ] ] = flag;
}

function GetParamType( params, var_list, var_count, var_return, var_i )
{
  var_count  = split( params, var_list );
  var_return = var_list[1];

  for (var_i=2;var_i<var_count;var_i++)
  {
    var_return = var_return " " var_list[var_i];
  }

  return var_return;
}

function GetParamName( params, var_list, var_count )
{
  var_count = split( params, var_list );

  return var_list[ var_count ];
}

function AddFunction( declaration, var_segments, var_i, var_len, var_return_type, var_name, var_param_list, var_params, var_param_count, var_param_type, var_param_name )
{
  var_len = split( declaration, var_segments, "(" );

  if ( var_len != 3 )
  {
    return;
  }

  var_return_type = var_segments[1];
  var_name        = var_segments[2];
  var_param_list  = var_segments[3];

  gsub( /\)/,  "", var_name);
  gsub( / /, "",  var_name);
  gsub( /\*/, "",  var_name);
  gsub( /\)/,  "", var_param_list);
  gsub( / +$/, "",  var_return_type);
  gsub( /^ +$/, "",  var_return_type);

  AddFunctionName( var_name );
  AddFunctionReturnType( var_return_type );

  if ( var_return_type == "struct PP_Var" )
  {
    # TODO
    # SetFunctionFlag( "kManageReturnVar" );
  }

  var_param_count = split( var_param_list, var_params, "," );

  # printf("%s:%s\n",PPName,declaration);
  # printf("  (%s) (%s) (%d)\n",var_return_type,var_name, var_param_count);

  for ( var_i=1;var_i<=var_param_count;var_i++) 
  {
    var_param_type = GetParamType( var_params[ var_i ] );
    var_param_name = GetParamName( var_params[ var_i ] );

    if ( var_param_type == "PP_Instance" )
    {
      AddParam( var_param_type, var_param_name, "kManageInstance" );
    }
    # TODO: handle PP_Var conversion automatically
    # else if ( var_param_type == "struct PP_Var" )
    # {
    # AddParam( var_param_type, var_param_name, "kManageVar" );
    # }
    else
    {
      AddParam( var_param_type, var_param_name );
    }
    # printf("  %s | %s\n", var_param_type, var_param_name );
  }
}

{
  if ( $1 == "PPB" ) 
  {
    AddPPB( $2 );
  }
  else if ( $1 == "PPP" ) 
  {
    AddPPP( $2 );
  }
  else if ( NF > 0 ) 
  {
    AddFunction( $0 );
  }
}

# -----------------------------------------------------------------
# OUTPUT FUNCTIONS (Note: no local variables)
# -----------------------------------------------------------------

function rpad( text, len, var_len, var_i )
{
  var_len = length( text );
  while ( var_len < len )
  {
    text = text " ";
    var_len++;
  }
  return text;
}

function OutputPPBGlobalsHpp()
{
  print "// --------------------------------------------------------------------------------------------"
  print "// Globals used by single instance interfaces"
  print "// --------------------------------------------------------------------------------------------"
  print "";

  print "enum"
  print "{"
  print "  kNaBlackInvalidInstance = 0,"
  print "  kNaBlackInvalidResource = 0,"
  print "};"
  print "";

  print rpad( "extern PP_Instance", 40 ) "g_NaBlackInstanceId;"
  print rpad( "extern int32_t", 40 ) "g_NaBlackViewWidth;";
  print rpad( "extern int32_t", 40 ) "g_NaBlackViewHeight;";
  print rpad( "extern PP_Resource", 40 ) "g_NaBlackGraphics3dId;";
  print rpad( "extern int32_t", 40 ) "g_NaBlackGraphics3dLastSwapResult;";

  SelectPPB();

  interface_count = GetInterfaceCount();
  for (i=1;i<=interface_count;i++)
  {
    SelectInterface(i);    
    interface_name = GetInterfaceName();

    function_count = GetFunctionCount();
    print rpad( "extern const PPB_" interface_name "*", 40 )  rpad( "g_NaBlack" interface_name ";", 30) "// (" function_count ") functions";
  }
  print "";
}

function OutputFunctionDeclarationHpp()
{
  interface_name = GetInterfaceName();
  function_name  = GetFunctionName();
  return_type    = GetFunctionReturnType();
  function_flag  = GetFunctionFlag();

  printf("extern ");

  if ( function_flag == "kManageReturnVar" )
  {
    printf( rpad( "const char*", 30 ) "NaBlack" interface_name function_name "( " );
  }
  else
  {
    printf( rpad( return_type, 30 ) "NaBlack" interface_name function_name "( " );
  }

  for (param_ndx=1;param_ndx<=GetFunctionParamCount();param_ndx++)
  {
    SelectFunctionParam( param_ndx );
    param_name = GetFunctionParamName();
    param_type = GetFunctionParamType();
    param_flag = GetFunctionParamFlag();

    if ( param_flag == "kManageInstance" )
    {
      # Remove this parameter.
    }
    else if ( param_flag == "kManageVar" )
    {
      # Replace with string interface
      printf( "const char* " param_name );
      if ( param_ndx != GetFunctionParamCount() )
      {
        printf( ", " );
      }
    }
    else
    {
      printf( param_type " " param_name );
      if ( param_ndx != GetFunctionParamCount() )
      {
        printf( ", " );
      }
    }
  }

  printf(" );\n");
}

function OutputFunctionInlineHpp()
{
  interface_name = GetInterfaceName();
  function_name  = GetFunctionName();
  return_type    = GetFunctionReturnType();
  function_flag  = GetFunctionFlag();

  printf("inline ");

  if ( function_flag == "kManageReturnVar" )
  {
    printf( "const char*" "\n" );
  }
  else
  {
    printf( return_type "\n" );
  }
  printf( "NaBlack" interface_name function_name "( " );

  for (param_ndx=1;param_ndx<=GetFunctionParamCount();param_ndx++)
  {
    SelectFunctionParam( param_ndx );
    param_name = GetFunctionParamName();
    param_type = GetFunctionParamType();
    param_flag = GetFunctionParamFlag();

    if ( param_flag == "kManageInstance" )
    {
      # Remove this parameter.
    }
    else if ( param_flag == "kManageVar" )
    {
      # Replace with string interface
      printf( "const char* " param_name );
      if ( param_ndx != GetFunctionParamCount() )
      {
        printf( ", " );
      }
    }
    else
    {
      printf( param_type " " param_name );
      if ( param_ndx != GetFunctionParamCount() )
      {
        printf( ", " );
      }
    }
  }

  printf(" )\n");
  printf("{\n");

  printf("  ");
  if ( return_type != "void" )
  {
    printf( "return " );
  }

  printf( "g_NaBlack" interface_name "->" function_name "(" );

  for (param_ndx=1;param_ndx<=GetFunctionParamCount();param_ndx++)
  {
    SelectFunctionParam( param_ndx );
    param_name = GetFunctionParamName();
    param_type = GetFunctionParamType();
    param_flag = GetFunctionParamFlag();

    # strip param_name of any decorators
    gsub(/\[/,"",param_name);
    gsub(/\]/,"",param_name);

    if ( param_flag == "kManageInstance" )
    {
      # Remove this parameter.
      printf("g_NaBlackInstanceId");
    }
    else if ( param_flag == "kManageVar" )
    {
      # Replace with string interface
      # TODO
    }
    else
    {
      printf( param_name );
    }
    if ( param_ndx != GetFunctionParamCount() )
    {
      printf( ", " );
    }
  }
 
  printf(");\n");
  printf("}\n");
  printf("\n");
}

function OutputPPBDeclarationsHpp()
{
  SelectPPB();

  print "#ifdef NATIVE_BLACK_C_INLINE";

  interface_count = GetInterfaceCount();
  for (interface_ndx=1;interface_ndx<=interface_count;interface_ndx++)
  {
    SelectInterface(interface_ndx);    
    interface_name = GetInterfaceName();

    print "// --------------------------------------------------------------------------------------------";
    print "// PPB_" interface_name " interface";
    print "// --------------------------------------------------------------------------------------------";

    function_count = GetFunctionCount();

    for (function_ndx=1;function_ndx<=function_count;function_ndx++)
    {
      SelectFunction(function_ndx); 
      OutputFunctionDeclarationHpp();
    }
    print "";
  }

  print "#endif // NATIVE_BLACK_C_INLINE";
  print "";
}

function OutputPPBInlineHpp()
{
  SelectPPB();

  interface_count = GetInterfaceCount();
  for (interface_ndx=1;interface_ndx<=interface_count;interface_ndx++)
  {
    SelectInterface(interface_ndx);    
    interface_name = GetInterfaceName();

    print "// --------------------------------------------------------------------------------------------";
    print "// PPB_" interface_name " inline definitions";
    print "// --------------------------------------------------------------------------------------------";

    function_count = GetFunctionCount();

    for (function_ndx=1;function_ndx<=function_count;function_ndx++)
    {
      SelectFunction(function_ndx); 
      OutputFunctionInlineHpp();
    }
    print "";
  }
}

function OutputInterface()
{
  interface_count = GetInterfaceCount();
  for (i=1;i<=interface_count;i++)
  {
    SelectInterface(i);    
    interface_name = GetInterfaceName();
    function_count = GetFunctionCount();
    printf("%s (%d)\n",interface_name, function_count);
  }
}

function OutputAssignGlobals()
{
  interface_count = GetInterfaceCount();
  for (i=1;i<=interface_count;i++)
  {
    SelectInterface(i);    
    interface_name = GetInterfaceName();
    function_count = GetFunctionCount();
    printf( rpad("g_NaBlack" interface_name, 40) " = " rpad("(const PPB_" interface_name "*)",40) "get_browser_interface(PPB_" toupper(interface_name) ");\n");
  }
}

END {
  print "#pragma once";
  print "// --------------------------------------------------------------------------------------------";
  print "// DO NOT EDIT: This is generated code.";
  print "// --------------------------------------------------------------------------------------------";
  print "";
  print "// --------------------------------------------------------------------------------------------";
  print "// Native Client (NaCl) single instance interface (Pepper 18)";
  print "// Mike Acton <macton@insomniacgames.com> <macton@gmail.com> @mike_acton";
  print "//";
  print "// This interface makes two assumptions for the common case:";
  print "//   - You only want one instance of a module. (*1)";
  print "//   - The NaCl API functions are all called from the same (main) thread (*2) (*3)."
  print "//";
  print "// (*1) See also: https://groups.google.com/d/msg/native-client-discuss/70hXg3AACeE/3Fau7EpCFUEJ";
  print "// (*2) Functions which add NaCl API calls to a queue to be called from the main thread could";
  print "//      also be generated to relieve this constraint. This is also the recommended approach from";
  print "//      Colt McAnlis @ Google. Check out his talk from #GDC12: http://youtu.be/R281PhQufHo";
  print "// (*3) With this constraint, most PP_Var conversion can be handled internally. #todo.";
  print "//";
  print "// #todo Also remove the OGL context params to make equivalent to gl_* calls.";
  print "//       ...or just remove them entirely since they're redundant.";
  print "// --------------------------------------------------------------------------------------------";
  print "";
  print "// --------------------------------------------------------------------------------------------";
  print "// Except for instance parameters, documentation corresponds to what's in the NaCl include files:";
  print "//   ~/nacl_sdk/pepper_18/toolchain/linux_x86_newlib/x86_64-nacl/include/ppapi/";
  print "//   Note: The documentation in those headers isn't totally accurate.";
  print "// --------------------------------------------------------------------------------------------";
  print "#include \"ppapi/c/pp_point.h\"";
  print "#include \"ppapi/c/pp_rect.h\"";
  print "#include \"ppapi/c/pp_resource.h\"";
  print "#include \"ppapi/c/pp_size.h\"";
  print "#include \"ppapi/c/pp_stdint.h\"";
  print "#include \"ppapi/c/pp_time.h\"";
  print "#include \"ppapi/c/pp_var.h\"";
  print "#include \"ppapi/c/pp_bool.h\"";
  print "#include \"ppapi/c/pp_completion_callback.h\"";
  print "#include \"ppapi/c/pp_errors.h\"";
  print "#include \"ppapi/c/pp_file_info.h\"";
  print "#include \"ppapi/c/pp_graphics_3d.h\"";
  print "#include \"ppapi/c/pp_input_event.h\"";
  print "#include \"ppapi/c/pp_instance.h\"";
  print "#include \"ppapi/c/pp_macros.h\"";
  print "#include \"ppapi/c/pp_module.h\"";
  print "#include \"ppapi/c/ppp_graphics_3d.h\"";
  print "#include \"ppapi/c/ppp.h\"";
  print "#include \"ppapi/c/ppp_input_event.h\"";
  print "#include \"ppapi/c/ppp_instance.h\"";
  print "#include \"ppapi/c/ppp_messaging.h\"";
  print "#include \"ppapi/c/ppp_mouse_lock.h\"";
  print "#include \"ppapi/c/ppb_audio_config.h\"";
  print "#include \"ppapi/c/ppb_audio.h\"";
  print "#include \"ppapi/c/ppb_core.h\"";
  print "#include \"ppapi/c/ppb_file_io.h\"";
  print "#include \"ppapi/c/ppb_file_ref.h\"";
  print "#include \"ppapi/c/ppb_file_system.h\"";
  print "#include \"ppapi/c/ppb_fullscreen.h\"";
  print "#include \"ppapi/c/ppb_graphics_2d.h\"";
  print "#include \"ppapi/c/ppb_graphics_3d.h\"";
  print "#include \"ppapi/c/ppb.h\"";
  print "#include \"ppapi/c/ppb_image_data.h\"";
  print "#include \"ppapi/c/ppb_input_event.h\"";
  print "#include \"ppapi/c/ppb_instance.h\"";
  print "#include \"ppapi/c/ppb_messaging.h\"";
  print "#include \"ppapi/c/ppb_mouse_lock.h\"";
  print "#include \"ppapi/c/ppb_opengles2.h\"";
  print "#include \"ppapi/c/ppb_url_loader.h\"";
  print "#include \"ppapi/c/ppb_url_request_info.h\"";
  print "#include \"ppapi/c/ppb_url_response_info.h\"";
  print "#include \"ppapi/c/ppb_var_array_buffer.h\"";
  print "#include \"ppapi/c/ppb_var.h\"";
  print "#include \"ppapi/c/ppb_view.h\"";
  print "#include \"ppapi/c/ppb_websocket.h\"";
  print "#include \"ppapi/gles2/gl2ext_ppapi.h\"";
  print "#include <GLES2/gl2.h>";
  print "#include <GLES2/gl2ext.h>";
  print "";
  print "// --------------------------------------------------------------------------------------------";
  print "// Additional (missing?) typedefs";
  print "// --------------------------------------------------------------------------------------------";
  print "#ifndef __cplusplus";
  print "typedef struct PPB_OpenGLES2          PPB_OpenGLES2;";
  print "typedef struct PP_Var                 PP_Var;";
  print "typedef struct PP_Rect                PP_Rect;";
  print "typedef struct PP_Point               PP_Point;";
  print "typedef struct PP_Size                PP_Size;";
  print "typedef struct PP_FileInfo            PP_FileInfo;";
  print "typedef struct PP_CompletionCallback  PP_CompletionCallback;";
  print "#endif";
  print "";
  print "#ifdef __cplusplus"
  print "extern \"C\" {";
  print "#endif";
  print "";
  print "// --------------------------------------------------------------------------------------------";
  print "// These are the functions you must provide to use this API:";
  print "// --------------------------------------------------------------------------------------------";
  print "PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] );";
  print "void    NaBlackInstanceDestroy();";
  print "void    NaBlackDidChangeView( PP_Resource view );";
  print "void    NaBlackDidChangeFocus( PP_Bool has_focus );";
  print "void    NaBlackRenderFrame();";
  print "PP_Bool NaBlackHandleInputEvent( PP_Resource input_event );";
  print "PP_Bool NaBlackHandleDocumentLoad( PP_Resource url_loader );";
  print "void    NaBlackHandleMessage( struct PP_Var message );";
  print "void    NaBlackMouseLockLost();";
  print "";

  OutputPPBDeclarationsHpp();
  OutputPPBGlobalsHpp();
  OutputPPBInlineHpp();

  print "";
  print "// --------------------------------------------------------------------------------------------";
  print "// Utility functions (not part of NaCl API)";
  print "// --------------------------------------------------------------------------------------------";
  print "";
  print "// #nablack-utility NaBlackMessagingPostUtf8 Post string to browser via PPAPI";
  print "void NaBlackMessagingPostUtf8(const char* msg);";
  print "";
  print "// #nablack-utility NaBlackMessagingPostUtf8 Post formatted string to browser via PPAPI";
  print "void NaBlackMessagingPostPrintf( const char* format, ... );";

  print "";
  print "#ifdef __cplusplus"
  print "} // extern \"C\"";
  print "#endif";

  # SelectPPB();
  # OutputAssignGlobals();

  # SelectPPB();
  # OutputInterface();
  # SelectPPP();
  # OutputInterface();
}

# Keep here for easy cut & paste
# ~/nacl_sdk/pepper_19/toolchain/linux_x86_newlib/x86_64-nacl/include/ppapi/
