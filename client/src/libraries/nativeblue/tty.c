#include "nativeblue_private.h"

int s_TtyWidth     = 0;
int s_TtyHeight    = 0;
int s_TtyConnected = 0;

void NaBlueTTYResize( int width, int height )
{
  s_TtyWidth     = width;
  s_TtyHeight    = height;
  s_TtyConnected = 1;
}

int NaBlueTTYWidth( int fd )
{
  // #todo manager per file (stdin/stdout/stderr)
  (void)(fd);
  return (s_TtyWidth);
}

int NaBlueTTYHeight( int fd )
{
  // #todo manager per file (stdin/stdout/stderr)
  (void)(fd);
  return (s_TtyHeight);
}

int NaBlueWaitTTYConnected( int fd )
{
  // #todo manager per file (stdin/stdout/stderr)
  (void)(fd);

  for (;;)
  {
    volatile int* tty_connected_addr = &s_TtyConnected;
    int           tty_connected      = __sync_fetch_and_add( tty_connected_addr, 0 ); 

    if ( tty_connected )
    {
      return (1);
    }
    NaBlueSleepLong();
  }
  return (0);
}

int __wrap_isatty( int fd )
{
  if ( fd == fileno( stdout ) )
  {
    // #todo allow browser to redirect
    return (1);
  } 

  if ( fd == fileno( stderr ) )
  {
    // #todo allow browser to redirect
    return (1);
  }

  if ( fd == fileno( stdin ) )
  {
    // #todo
    return (0);
  }

  return (0);
}
