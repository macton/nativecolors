#include <curses.h>

int PDC_clearclipboard(void)
{
  return (1);
}

int PDC_freeclipboard(char *contents)
{
  return (1);
}

int PDC_getclipboard(char **contents, long *length)
{
  return (1);
}

int PDC_setclipboard(const char *contents, long length)
{
  return (1);
}

