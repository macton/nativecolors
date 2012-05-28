#pragma once

#include <stdint.h>

// Quick and dirty pthread_rwlock implementation
// no: pthread_yield

typedef struct pthread_rwlock_t pthread_rwlock_t;

struct pthread_rwlock_t 
{
  int32_t m_ReadCount;
  int32_t m_WriteCount;
}
__attribute__ ((aligned(4)));

#define PTHREAD_RWLOCK_INITIALIZER { 0, 0 }

int pthread_yield( void );
int pthread_rwlock_rdlock( pthread_rwlock_t* lock );
int pthread_rwlock_wrlock( pthread_rwlock_t* lock );
int pthread_rwlock_unlock( pthread_rwlock_t* lock );

