
// How to run?
//   $(NACL_SDK_ROOT)/tools/sel_ldr_x86_64 -a -S -B $(NACL_SDK_ROOT)/tools/irt_x86_64.nexe hello_x86_64.nexe hello.c
//   ...should print this file.

// See also native-client-discuss thread: "Segmentation fault: sel_ldr (Pepper 18)"
//   https://groups.google.com/d/topic/native-client-discuss/u68-MImnT0U/discussion

#include <stdio.h>
#include <alloca.h>

int
main ( int argc, char** argv )
{
  printf ( "Hello, world! Echo input file (fopen)\n" );

  if ( argc < 2 )
  {
    printf("Usage: nacl-hello-02-fopen <input_file>\n");
    return (-1);
  }

  const char* file_path = argv[1];
  FILE*       file      = fopen( file_path, "r" );

  if ( file == NULL )
  {
    printf("ERROR: Could not open file (%s)\n", file_path);
    return (-1);
  }

  (void)fseek( file, 0, SEEK_END );

  long file_size = ftell( file );

  (void)fseek( file, 0, SEEK_SET );

  char*  input_buffer   = alloca ( file_size );

  if ( !input_buffer )
  {
    printf ( "ERROR: Could not allocate buffer from stack." );
    return ( -1 );
  }

  fread( input_buffer, file_size, 1, file );

  printf ( "Size of input file: %ld\n", file_size );
  printf ( "----- INPUT BEGIN -----\n" );
  fwrite ( input_buffer, file_size, 1, stdout );
  printf ( "----- INPUT END -----\n" );
  return ( 0 );
}
