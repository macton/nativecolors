#include <string.h>
#include "nativeblue_private.h"

static char s_Cwd[ kNaBlueMaxPath+1 ];

void NaBlueSetCwd( const char* path )
{
  strncpy( s_Cwd, path, kNaBlueMaxPath );
  s_Cwd[ kNaBlueMaxPath ] = 0;
}

const char* NaBlueGetCwd( void )
{
  return s_Cwd;
}
