#include <stdio.h>

int main( void )
{
  // Added test for hterm inverse bug. This text should be red.
  printf("\033[31;27mHello, World!\r\n");

  return (0);
}
