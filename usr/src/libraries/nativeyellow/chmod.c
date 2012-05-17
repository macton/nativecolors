#include <sys/types.h>
#include <sys/stat.h>

int chmod(const char *path, mode_t mode)
{
  (void)path;
  (void)mode;
  return (0);
}

int fchmod(int fildes, mode_t mode)
{
  (void)fildes;
  (void)mode;
  return (0);
}
