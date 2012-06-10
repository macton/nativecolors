
// How to run?
//   $(NACL_SDK_ROOT)/tools/sel_ldr_x86_64 -a -S -B $(NACL_SDK_ROOT)/tools/irt_x86_64.nexe hello_x86_64.nexe hello.c
//   ...should print this file.

// See also native-client-discuss thread: "Segmentation fault: sel_ldr (Pepper 18)"
//   https://groups.google.com/d/topic/native-client-discuss/u68-MImnT0U/discussion

#include <stdio.h>
#include <alloca.h>

char data[] = "Hello, world!"; 

int
main ( int argc, char** argv )
{
  FILE* file = fmemopen( data, sizeof(data), "r" );
  if ( file == NULL )
  {
    printf("fmemopen failed\n");
    return (-1);
  }

  char* copy = (char*)alloca( sizeof(data ) );
  int read_result = fread( copy, sizeof(data), 1, file );
  if ( read_result == 1 )
  {
    printf("%s\n", copy );
  }
  else
  {
    printf("read failed result=%d\n",read_result);
  }
  int close_result = fclose(file);
  if ( close_result != 0 )
  {
    printf("close failed result=%d\n",close_result);
  }

  return ( 0 );
}
