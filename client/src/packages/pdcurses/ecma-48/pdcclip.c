#include <curses.h>

int PDC_clearclipboard(void)
{
  return (PDC_CLIP_ACCESS_ERROR);
}

int PDC_freeclipboard(char *contents)
{
  return (PDC_CLIP_ACCESS_ERROR);
}

int PDC_getclipboard(char **contents, long *length)
{
  return (PDC_CLIP_ACCESS_ERROR);
}

int PDC_setclipboard(const char *contents, long length)
{
  return (PDC_CLIP_ACCESS_ERROR);
}

