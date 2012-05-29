#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include "nativeblue_private.h"

int __wrap_creat(const char *pathname, mode_t mode)
{
  (void)mode;
  return open( pathname, O_CREAT|O_WRONLY|O_TRUNC );
}
