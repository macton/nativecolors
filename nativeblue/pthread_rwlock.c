// Super quick-and-dirty implementation of pthread_rwlock
// For NaCl-newlib

#include "pthread_rwlock.h"

enum
{
  kRwLockSpinCount = 3
};

int pthread_yield( void )
{
  sched_yield();
  return (0);
}

int pthread_rwlock_rdlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   write_lock;
  
  do 
  {
    __sync_fetch_and_add( read_count, 1 ); 
    for (int i=0;i<kRwLockSpinCount;i++)
    {
      write_lock = __sync_fetch_and_add( write_count, 0 ); 
      if ( write_lock == 0 )
      {
        return (0);
      }
    }
    __sync_fetch_and_sub( read_count, 1 ); 
    pthread_yield();
  }
  while (1);
}

int pthread_rwlock_wrlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   read_lock;

  do 
  {
    __sync_fetch_and_add( write_count, 1 ); 
    for (int i=0;i<kRwLockSpinCount;i++)
    {
      read_lock = __sync_fetch_and_add( read_count, 0 ); 
      if ( read_lock == 0 )
      {
        return (0);
      }
    }
    __sync_fetch_and_sub( write_count, 1 ); 
    pthread_yield();
  }
  while (1);
}

int pthread_rwlock_unlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   read_lock;

  read_lock = __sync_fetch_and_add( read_count, 0 ); 
  if ( read_lock == 0 )
  {
    __sync_fetch_and_sub( write_count, 1 ); 
    return (0);
  }
  __sync_fetch_and_sub( read_count, 1 ); 
  return (0);
}
