#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>
#include <sys/stat.h>
#include <sys/types.h>

#define VERSION            "86"
#define TEST_DIR           "hellodir"
#define TEST_FAIL_DIR      "some_nonexistant_directory"
#define TEST_FILE          "hello.txt"
#define VBUF_SIZE          (1024*1024)
#define EXPECTED_FILE_SIZE 58942

char* data = "Hello, World! version=" VERSION "\r\n";

// setvbuf use isn't required here, just for testing buffering.
char  file_vbuf[ VBUF_SIZE ]; 
char  test_buf[ VBUF_SIZE ];

void test_write( void );
void test_size( void );
void test_read( void );

int fileno( FILE* file );

int main( void )
{
  printf("fileno(stdout) = %d\r\n",fileno(stdout));
  printf("fileno(stdin)  = %d\r\n",fileno(stdin));
  printf("fileno(stderr) = %d\r\n",fileno(stderr));
  printf("\033[33m%s", data );
  printf("\r\n");
  printf("\033[33mTIP: Use File System Explorer for debugging:\r\n");
  printf("     https://chrome.google.com/webstore/detail/nhnjmpbdkieehidddbaeajffijockaea\r\n");
  printf("\r\n");

  int mkdir_status = mkdir( TEST_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  printf("\033[37mmkdir TEST = %s\r\n", ( mkdir_status == 0 ) ? "\033[32mPASSED" : "\033[31mFAILED" );

  int chdir_status = chdir( TEST_DIR );
  printf("\033[37mchdir TEST = %s\r\n", ( chdir_status == 0 ) ? "\033[32mPASSED" : "\033[31mFAILED" );

  int dir_fd = open( "/" TEST_DIR, O_RDONLY );
  printf("\033[37mdir open TEST = %s (fd=%d)\r\n", ( dir_fd >= 0 ) ? "\033[32mPASSED" : "\033[31mFAILED", dir_fd );

  if ( dir_fd < 0 )
  {
    printf("\033[31m    FileIO::Open and FileIO::Query return PP_ERROR_FAILED on directories.\r\n");
    printf("\033[31m    Follow-up: https://groups.google.com/d/msg/native-client-discuss/2huiRxDois4/TVTaFKmuUIgJ\r\n");  
  }
  
  if ( dir_fd >= 0 )
  {
    int close_result = close( dir_fd );
    printf("\033[37mclose for dir open TEST = %s\r\n", ( close_result == 0 ) ? "\033[32mPASSED" : "\033[31mFAILED" );
  }

  int fail_dir_fd = open( "/" TEST_FAIL_DIR, O_RDONLY );
  printf("\033[37mdir open bad dir TEST = %s (fd=%d)\r\n", ( fail_dir_fd == -1 ) ? "\033[32mPASSED" : "\033[31mFAILED", dir_fd );

  if ( fail_dir_fd >= 0 )
  {
    int close_result = close( dir_fd );
    printf("\033[31mclose result = %d\n",close_result);
  }

  test_write();
  test_size();
  test_read();

  return (0);
}

void test_write( void )
{
  printf("\033[37mtest_write BEGIN\r\n");

  FILE* file = fopen( TEST_FILE, "w+" );
  setvbuf( file, file_vbuf, _IOFBF, VBUF_SIZE );  

  if ( file == NULL )
  {
    printf("\033[37mfopen for write TEST = \033[31mFAILED\r\n");
    return;
  }

  printf("\033[37mfopen for write TEST = \033[32mPASSED\r\n");

  int fprint_size = 0;
  int sprint_size = 0;

  fprint_size += fprintf( file, "%s", data );
  sprint_size += snprintf( test_buf, VBUF_SIZE, "%s", data );

  for (int i=0;i<5000;i++)
  {
    fprint_size += fprintf( file, "Hello %d\r\n", i );
    sprint_size += snprintf( test_buf, VBUF_SIZE, "Hello %d\r\n", i );
  }

  fprint_size += fprintf( file, "%s", data );
  sprint_size += snprintf( test_buf, VBUF_SIZE, "%s", data );

  printf("\033[37mfprintf TEST = %s\r\n", ( fprint_size == sprint_size ) ? "\033[32mPASSED" : "\033[31mFAILED" );

  int close_result = fclose( file );
  printf("\033[37mfclose for write TEST = %s\r\n", ( close_result == 0 ) ? "\033[32mPASSED" : "\033[31mFAILED" );
}

void test_size( void )
{
  FILE* file = fopen( TEST_FILE, "r+" );
  setvbuf( file, file_vbuf, _IOFBF, VBUF_SIZE );  
   
  if ( file == NULL )
  {
    printf("\033[37mfopen for size TEST = \033[31mFAILED\r\n");
    return;
  }
  printf("\033[37mfopen for size TEST = \033[32mPASSED\r\n");

  int pos = fseek(file,0,SEEK_END);
  printf("\033[37mfseek SEEK_END TEST = %s\r\n", (pos == 0) ? "\033[32mPASSED" : "\033[31mFAILED");

  long size = ftell( file );
  printf("\033[37mftell size = %d\r\n", (int)size );
  printf("\033[37mftell size TEST = %s\r\n", (size == EXPECTED_FILE_SIZE) ? "\033[32mPASSED" : "\033[31mFAILED");

  int close_result = fclose( file );
  printf("\033[37mfclose for size TEST = %s\r\n", ( close_result == 0 ) ? "\033[32mPASSED" : "\033[31mFAILED" );
}


void test_read( void )
{
  FILE* file = fopen( TEST_FILE, "r+" );
  setvbuf( file, file_vbuf, _IOFBF, VBUF_SIZE );  
   
  if ( file == NULL )
  {
    printf("\033[37mfopen for read TEST = \033[31mFAILED\r\n");
    return;
  }
  printf("\033[37mfopen for read TEST = \033[32mPASSED\r\n");

  int pos = fseek(file,0,SEEK_END);
  printf("\033[37mfseek SEEK_END TEST = %s\r\n", (pos == 0) ? "\033[32mPASSED" : "\033[31mFAILED");

  long size = ftell( file );
  printf("\033[37mftell size = %d\r\n", (int)size );
  printf("\033[37mftell size TEST = %s\r\n", (size == EXPECTED_FILE_SIZE) ? "\033[32mPASSED" : "\033[31mFAILED");

  int reset_pos = fseek(file,0,SEEK_SET);
  printf("\033[37mfseek SEEK_SET TEST = %s\r\n", (reset_pos == 0) ? "\033[32mPASSED" : "\033[31mFAILED");

  char* buf = (char*)alloca( size );
  if ( buf == NULL )
  {
    printf("\033[31mERROR: Couldn't allocate %d bytes for read\r\n",(int)size);
    int close_result = fclose( file );
    printf("\033[31mClose result=%d\r\n",close_result);
    return;
  }
  
  int read_count = fread( buf, size, 1, file );
  printf("\033[37mfread count TEST = %s\r\n", (read_count == 1) ? "\033[32mPASSED" : "\033[31mFAILED");

  printf("\033[34m---- TEST FILE BEGIN ----\r\n");
  int write_count = fwrite( buf, size, 1, stdout );
  printf("\033[34m---- TEST FILE END ----\r\n");
  printf("\033[37mfwrite count TEST = %s\r\n", (write_count == 1) ? "\033[32mPASSED" : "\033[31mFAILED");

  int close_result = fclose( file );
  printf("\033[37mfclose for read TEST = %s\r\n", ( close_result == 0 ) ? "\033[32mPASSED" : "\033[31mFAILED" );
}

