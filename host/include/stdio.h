#pragma once
#include_next <stdio.h>

FILE *fmemopen(void *buf, size_t size, const char *mode);
