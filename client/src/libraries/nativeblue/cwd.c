#include <string.h>
#include <errno.h>
#include <limits.h>
#include "nativeblue_private.h"

static char s_Cwd[ kNaBlueMaxPath+1 ];

void NaBlueSetCwd( const char* path )
{
  if ( path[0] == '/' )
  {
    strncpy( s_Cwd, path, kNaBlueMaxPath );
    s_Cwd[ kNaBlueMaxPath ] = 0;
  }
  else
  {
    int cwd_len = strlen( s_Cwd );

    strncpy( s_Cwd + cwd_len, path, kNaBlueMaxPath - cwd_len );
    s_Cwd[ kNaBlueMaxPath ] = 0;
  }

  int len = strlen( s_Cwd );
  if ( s_Cwd[len-1] != '/' )
  {
    if ( len < (kNaBlueMaxPath-1) )
    {
      s_Cwd[ len   ] = '/';
      s_Cwd[ len+1 ] = 0;
    }
  }
}

const char* NaBlueGetCwd( void )
{
  return s_Cwd;
}

// The getcwd() function copies an absolute pathname of the current working directory to the array pointed 
// to by buf, which is of length size.
//
// If the length of the absolute pathname of the current working directory, including the terminating null 
// byte, exceeds size bytes, NULL is returned, and errno is set to ERANGE; an application should check for 
// this error, and allocate a larger buffer if necessary.
//
// As an extension to the POSIX.1-2001 standard, Linux (libc4, libc5, glibc) getcwd() allocates the buffer 
// dynamically using malloc(3) if buf is NULL. In this case, the allocated buffer has the length size 
// unless size is zero, when buf is allocated as big as necessary. The caller should free(3) the returned buffer.

char *getcwd(char *buf, size_t size)
{
  if ( buf == NULL )
  {
    size = ( size ) ? size : kNaBlueMaxPath+1;
    buf  = (char*)malloc( size );
    if ( buf == NULL )
    {
      return NULL;
    }
  }

  strncpy( buf, s_Cwd, size-1 );
  buf[ size ] = 0;

  if ( strlen( buf ) == size )
  {
    errno = ERANGE;
    return NULL;
  }

  return buf;
}

// getwd() does not malloc(3) any memory. The buf argument should be a pointer to an array at least 
// PATH_MAX bytes long. If the length of the absolute pathname of the current working directory, 
// including the terminating null byte, exceeds PATH_MAX bytes, NULL is returned, and errno is set to 
// ENAMETOOLONG. (Note that on some systems, PATH_MAX may not be a compile-time constant; furthermore, 
// its value may depend on the file system, see pathconf(3).) For portability and security reasons, 
// use of getwd() is deprecated.

char *getwd(char *buf)
{
  // No real way to fix this...
  if ( strlen( s_Cwd ) > (PATH_MAX-1) )
  {
    errno = ENAMETOOLONG;
    return NULL;
  }

  strncpy( buf, s_Cwd, PATH_MAX-1 );  
  buf[ PATH_MAX ] = 0;

  return buf;
}

// get_current_dir_name() will malloc(3) an array big enough to hold the absolute pathname of the 
// current working directory. If the environment variable PWD is set, and its value is correct, then 
// that value will be returned. The caller should free(3) the returned buffer.

char *get_current_dir_name(void)
{
  char* buf = (char*)malloc( kNaBlueMaxPath+1 );
  if ( buf == NULL )
  {
    return NULL;
  } 

  strncpy( buf, s_Cwd, kNaBlueMaxPath );
  buf[ kNaBlueMaxPath ] = 0;
  
  return buf;
}

// chdir() changes the current working directory of the calling process to the directory specified in path.

int chdir(const char *path)
{
  NaBlueSetCwd( path );
  return (0);
}
