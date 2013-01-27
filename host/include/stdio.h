#pragma once
#include_next <stdio.h>

int fileno( FILE* file );
FILE *fmemopen(void *buf, size_t size, const char *mode);

