#include <stdarg.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

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
      if ( d == 0 )
      {
        struct winsize* size = va_arg(argp, struct winsize*);    
 
        char* lines   = getenv("LINES");
        char* columns = getenv("COLUMNS");

        if ( (lines == NULL) || ( columns == NULL) )
        {
          exit_code = ENOTTY;
          break;
        }
  
        // Hack in a size for the moment...
        size->ws_col    = atoi( columns );
        size->ws_row    = atoi( lines );
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
