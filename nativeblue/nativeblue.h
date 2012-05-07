#pragma once

#include <stdio.h>
#include <stdint.h>

// Returns a url-encoded version of str
int strnurlencode( char* dest, const char* str, size_t max_len );

// Returns a url-decoded version of str
int strnurldecode( char* dest, const char* str, size_t max_len );

uint32_t next_pow2_u32(uint32_t x);
uint64_t next_pow2_u64(uint64_t x);

// --------------------------------------------------------------------------
// Native Client issues:
//   - warning: pthread_kill is not implemented and will always fail
//   - warning: pthread_cancel is not implemented and will always fail
//   - STDOUT_FILENO, STDERR_FILENO not defined
//   - fileno() is missing from <stdio.h>
// --------------------------------------------------------------------------

int fileno( FILE* stream );

