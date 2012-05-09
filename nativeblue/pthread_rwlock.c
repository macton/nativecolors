// ----------------------------------------------------------------------
// Super quick-and-dirty implementation of pthread_rwlock for NaCl newlib
// macton@insomniacgames.com
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Obvious differences between this and an actual implementation.
//   - It's actually two mutually exclusive locks (read and write)
//   - Multiple reads can be aquired so long as no write is aquired
//   - Multiple writes can be aquired so long as no read is aquired
//     - A constraint of one write lock (or at least contrained to a 
//       single thread) could be added, since it's not generally 
//       used (by me) in a way where that'd make a difference.
//   - None of the typical pthread error checking is done
// ----------------------------------------------------------------------

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
  
  while (1)
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
}

int pthread_rwlock_wrlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   read_lock;

  while (1)
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
}

int pthread_rwlock_unlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   read_lock;
  int32_t   write_lock;

  // One of these two locks will be stable. So we're really only looking
  // out for quick fluctuations of the other type of lock trying to be
  // aquired.

  while (1)
  {
    for (int i=0;i<kRwLockSpinCount;i++)
    {
      read_lock  = __sync_fetch_and_add( read_count,  0 ); 
      write_lock = __sync_fetch_and_add( write_count, 0 ); 

      if (( read_lock > 0 ) && ( write_lock == 0 ))
      {
        __sync_fetch_and_sub( read_count, 1 ); 
        return (0);
      }
    
      if (( write_lock > 0 ) && ( read_lock == 0 ))
      {
        __sync_fetch_and_sub( write_count, 1 ); 
        return (0);
      }
    }
    pthread_yield();
  }
}
