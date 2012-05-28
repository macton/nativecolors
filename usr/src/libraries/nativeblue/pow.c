#include <stdint.h>

uint32_t next_pow2_u32(uint32_t x)
{
  x -= 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  
  return x + 1;
}

uint64_t next_pow2_u64(uint64_t x)
{
  x -= 1;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);
  
  return x + 1;
}
