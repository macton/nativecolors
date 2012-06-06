#include <sys/types.h>
#include <utime.h>

int utime(const char *filename, const struct utimbuf *times)
{
  (void)filename;
  (void)times;
  return (0);
}
