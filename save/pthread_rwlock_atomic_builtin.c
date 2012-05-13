// ----------------------------------------------------------------------
// Super quick-and-dirty implementation of pthread_rwlock for NaCl newlib
// macton@insomniacgames.com
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Obvious differences between this and an actual implementation.
//   - Multiple reads can be aquired so long as no write is aquired
//   - Multiple writes can be aquired so long as no read is aquired
//     - A constraint of one write lock (or at least contrained to a 
//       single thread) could be added, but it's not generally 
//       used (by me) in a way where that'd make a difference.
//   - None of the typical pthread error checking is done
// ----------------------------------------------------------------------

#include "pthread_rwlock.h"

#include <sys/cdefs.h>
#include <sys/_types.h>

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
  int       i;
  
  while (1)
  {
    __atomic_add_fetch( read_count, 1, __ATOMIC_RELEASE );
    for (i=0;i<kRwLockSpinCount;i++)
    {
      write_lock = __atomic_load_n( write_count, __ATOMIC_ACQUIRE );
      if ( write_lock == 0 )
      {
        return (0);
      }
    }
    __atomic_sub_fetch( read_count, 1, __ATOMIC_RELEASE );
    pthread_yield();
  }
}

int pthread_rwlock_wrlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   read_lock;
  int       i;

  while (1)
  {
    __atomic_add_fetch( write_count, 1, __ATOMIC_RELEASE );
    for (i=0;i<kRwLockSpinCount;i++)
    {
      read_lock = __atomic_load_n( read_count, __ATOMIC_ACQUIRE );
      if ( read_lock == 0 )
      {
        return (0);
      }
    }
    __atomic_sub_fetch( write_count, 1, __ATOMIC_RELEASE );
    pthread_yield();
  }
}

int pthread_rwlock_unlock( pthread_rwlock_t* lock )
{
  int32_t*  read_count  = &lock->m_ReadCount;
  int32_t*  write_count = &lock->m_WriteCount;
  int32_t   read_lock;
  int32_t   write_lock;
  int       i;

  // One of these two locks will be stable. So we're really only looking
  // out for quick fluctuations of the other type of lock trying to be
  // aquired.

  while (1)
  {
    for (i=0;i<kRwLockSpinCount;i++)
    {
      read_lock  = __atomic_load_n( read_count,  __ATOMIC_ACQUIRE );
      write_lock = __atomic_load_n( write_count, __ATOMIC_ACQUIRE );

      if (( read_lock > 0 ) && ( write_lock == 0 ))
      {
        __atomic_sub_fetch( read_count, 1, __ATOMIC_RELEASE );
        return (0);
      }
    
      if (( write_lock > 0 ) && ( read_lock == 0 ))
      {
        __atomic_sub_fetch( write_count, 1, __ATOMIC_RELEASE );
        return (0);
      }
    }
    pthread_yield();
  }
}
