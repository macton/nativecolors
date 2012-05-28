#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define VERSION "54"

// Use for debugging:
//  - File System Explorer https://chrome.google.com/webstore/detail/nhnjmpbdkieehidddbaeajffijockaea

char* data = "Hello, World! version=" VERSION "\r\n";

int main( void )
{
  printf("\033[31;27m%s", data );

  FILE* file = fopen("/hello.txt","w+");

  if ( file == NULL )
  {
    printf("Couldn't open file\r\n");
    return (-1);
  }

  printf("Opened file=%p\r\n",(void*)file);

  fprintf( file, data );
  for (int i=0;i<5000;i++)
  {
    fprintf( file, "Hello %d\n",i);
  }
  fprintf( file, data );

  int close_result = fclose( file );
  printf("Close result=%d\r\n",close_result);

  return (0);
}
