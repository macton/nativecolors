#pragma once

#if defined(DEVEL)
#define TWEAK_FLOAT( var_name, initial_value, display_name, min, max ) \
  static float var_name = initial_value; \
  { \
    static int initialized = 0;   \
    if ( !initialized ) \
    { \
      NaclMessagingPostPrintf("$tweak_float={ \"Name\" : \"%s\", \"Addr\": \"%p\", \"Min\": %g, \"Max\": %g, \"Value\": %g, \"VarName\": \"" #var_name "\" }", display_name, &var_name, min, max, initial_value ); \
      initialized = 1; \
    } \
  }

#define TWEAK_INT32( var_name, initial_value, display_name, min, max ) \
  static int32_t var_name = initial_value; \
  { \
    static int initialized = 0;   \
    if ( !initialized ) \
    { \
      NaclMessagingPostPrintf("$tweak_int32={ \"Name\" : \"%s\", \"Addr\": \"%p\", \"Min\": %d, \"Max\": %d, \"Value\": %d, \"VarName\": \"" #var_name "\" }", display_name, &var_name, min, max, initial_value ); \
      initialized = 1; \
    } \
  }
#else
#define TWEAK_FLOAT( var_name, initial_value, display_name, min, max ) (void)(var_name,initial_value,display_name,min,max)
#define TWEAK_INT32( var_name, initial_value, display_name, min, max ) (void)(var_name,initial_value,display_name,min,max)
#endif

