#pragma once
#include_next <unistd.h>

#if defined(_BSD_SOURCE) || (defined(_X_OPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) || (defined(_XOPEN_SOURCE) && defined(_XOPEN_SOURCE_EXTENDED)) || /* Since glibc 2.10: */ (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200809L))
int fchdir(int fd);
#endif
