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
//   - pthread
//     - warning: pthread_kill is not implemented and will always fail
//     - warning: pthread_cancel is not implemented and will always fail
//     - pthread_yield not defined.
//   - STDOUT_FILENO, STDERR_FILENO not defined
//     - Exists in glibc
//   - fileno() is missing from <stdio.h>
//     - Don't forget to -D_GNU_SOURCE
//   - pthread_rwlock_t doesn't exist. ARGH! See: ../saved/nativeblue.c
//     - See: pthread_rwlock.c
//     - Exists in glibc
// --------------------------------------------------------------------------

int fileno( FILE* stream );

