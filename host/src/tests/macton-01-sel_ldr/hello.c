
// How to run?
//   $(NACL_SDK_ROOT)/tools/sel_ldr_x86_64 -B $(NACL_SDK_ROOT)/tools/irt_x86_64.nexe hello_x86_64.nexe < hello.c
//   ...should print this file.

// See also native-client-discuss thread: "Segmentation fault: sel_ldr (Pepper 18)"
//   https://groups.google.com/d/topic/native-client-discuss/u68-MImnT0U/discussion

#include <stdio.h>

int fileno( FILE* file );

int
main( void )
{
  printf("Hello, World!\n");
  printf("fileno(stdout) = %d\n",fileno(stdout));
  printf("fileno(stdin)  = %d\n",fileno(stdin));
  printf("fileno(stderr) = %d\n",fileno(stderr));
  return ( 0 );
}
