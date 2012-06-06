#include <stdarg.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "nativeblue_private.h"

int ioctl(int d, unsigned long request, ...)
{
  int     exit_code = EINVAL;
  va_list argp;

  va_start( argp, request );

  switch ( request )
  {
    // Get terminal dimensions
    case TIOCGWINSZ:
    {
      // ATM only ZERO is supported for d
      if ( ( d == 0 ) && ( NaBlueWaitTTYConnected( fileno(stdout) ) ) )
      {
        struct winsize* size = va_arg(argp, struct winsize*);    
  
        // Hack in a size for the moment...
        size->ws_col    = NaBlueTTYWidth( d );
        size->ws_row    = NaBlueTTYHeight( d ) ;
        size->ws_xpixel = 0;
        size->ws_ypixel = 0;
        exit_code = 0;
      }
      else
      {
        exit_code = ENOTTY;
      }
    }
    break;
  }
 
  va_end( argp );
  return (exit_code);
}
