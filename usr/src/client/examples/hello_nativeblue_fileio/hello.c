#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>

#define VERSION "64"
#define TEST_FILE "hello.txt"
#define VBUF_SIZE (1024*1024)

// Use for debugging:
//  - File System Explorer https://chrome.google.com/webstore/detail/nhnjmpbdkieehidddbaeajffijockaea

char* data = "Hello, World! version=" VERSION "\r\n";

// setvbuf use isn't required here, just for testing buffering.
char  file_vbuf[ VBUF_SIZE ]; 

void test_write( void );
void test_size( void );
void test_read( void );

int main( void )
{
  printf("\033[31;27m%s", data );

  test_write();
  test_size();
  test_read();

  return (0);
}

void test_write( void )
{
  FILE* file = fopen( TEST_FILE, "w+" );
  setvbuf( file, file_vbuf, _IOFBF, VBUF_SIZE );  

  if ( file == NULL )
  {
    printf("Couldn't open file for write\r\n");
    return;
  }

  printf("Opened file=%p\r\n",(void*)file);

  fprintf( file, "%s", data );
  for (int i=0;i<5000;i++)
  {
    fprintf( file, "Hello %d\r\n",i);
  }
  fprintf( file, "%s", data );

  int close_result = fclose( file );
  printf("Close result=%d\r\n",close_result);
}

void test_size( void )
{
  FILE* file = fopen( TEST_FILE, "r+" );
  setvbuf( file, file_vbuf, _IOFBF, VBUF_SIZE );  
   
  if ( file == NULL )
  {
    printf("Couldn't open file for read\r\n");
    return;
  }

  int pos = fseek(file,0,SEEK_END);
  printf("seek end pos: %d\r\n",pos);

  long size = ftell( file );
  printf("size %d\r\n",size);

  int close_result = fclose( file );
  printf("Close result=%d\r\n",close_result);
}


void test_read( void )
{
  FILE* file = fopen( TEST_FILE, "r+" );
  setvbuf( file, file_vbuf, _IOFBF, VBUF_SIZE );  
   
  if ( file == NULL )
  {
    printf("Couldn't open file for read\r\n");
    return;
  }

  int pos = fseek(file,0,SEEK_END);
  printf("seek end pos: %d\r\n",pos);

  long size = ftell( file );
  printf("size %d\r\n",size);

  int reset_pos = fseek(file,0,SEEK_SET);
  printf("seek set pos: %d\r\n",reset_pos);

  char* buf = (char*)alloca( size );
  if ( buf == NULL )
  {
    printf("Couldn't allocate %d bytes for read\r\n",size);
    int close_result = fclose( file );
    printf("Close result=%d\r\n",close_result);
    return;
  }
  
  int read_count = fread( buf, size, 1, file );
  printf("Read count=%d\r\n",read_count); 

  fwrite( buf, size, 1, stdout );
  printf("\r\n");
  fflush( stdout );

  int close_result = fclose( file );
  printf("Close result=%d\r\n",close_result);
}

