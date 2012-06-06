#pragma once

#include <stdio.h>
#include <stdint.h>

void NaBlueSleepShort( void );
void NaBlueSleepLong( void );
void NaBlueSleep( int64_t ms );

// --------------------------------------------------------------------------
// int strnurlencode( char* dest, int dest_max_len, const char* str, size_t str_max_len ) 
// int strnurldecode( char* dest, int dest_max_len, const char* str, size_t str_max_len ) 
//
// Convert to/from url encoded string
// Example:
// 
//  int   encode_len = strnurlencode( NULL, 0, str, len );
//  char* encode_str = (char*)alloca( encode_len+1 );
//
//  strnurlencode( encode_str, encode_len+1, str, len );
//
//  int   decode_len = strnurlencode( NULL, 0, encode_str, encode_len+1 );
//  char* decode_str = (char*)alloca( decode_len+1 );
//
//  strnurldecode( decode_str, decode_len+1, encode_str, encode_len+1 );
//
//  if (strncmp(decode_str,str,len) != 0)
//  {
//    // -- ERROR! strings should match.
//  }
// --------------------------------------------------------------------------

int strnurlencode( char* dest, int dest_max_len, const char* str, size_t str_max_len );
int strnurldecode( char* dest, int dest_max_len, const char* str, size_t str_max_len );

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

