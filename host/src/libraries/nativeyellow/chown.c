#include <sys/types.h>
#include <unistd.h>

int chown(const char *path, uid_t owner, gid_t group)
{
  (void)path;
  (void)owner;
  (void)group;
  return (0);
}
