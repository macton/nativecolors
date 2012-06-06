#include <time.h>
#include "nativeblue_private.h"

enum
{
  kNaBlueSleepShortTimeMs = 1UL,
  kNaBlueSleepLongTimeMs  = 8UL
};

void NaBlueSleepShort( void )
{
  NaBlueSleep( kNaBlueSleepShortTimeMs );
}

void NaBlueSleepLong( void )
{
  NaBlueSleep( kNaBlueSleepShortTimeMs );
}

void NaBlueSleep( int64_t ms )
{
  // instead of sched_yield() use nanosleep
  // Reference: libatomic_ops http://www.hpl.hp.com/research/linux/atomic_ops/index.php4
  // See also: https://groups.google.com/d/topic/native-client-discuss/khBNKzdDZ0w/discussion

  int64_t sec    = ms/1000;
  int64_t rem_ms = ms - ( sec * 1000 );

  struct timespec ts;
  ts.tv_sec  = sec;
  ts.tv_nsec = 1000000 * rem_ms;
  nanosleep(&ts, 0);
}
