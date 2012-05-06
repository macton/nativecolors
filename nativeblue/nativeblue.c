#define _MULTI_THREADED

#include <nativeblack/nativeblack.h>
#include "nativeblue.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <alloca.h>
#include <ctype.h>

// fileno() is missing from <stdio.h>
int fileno( FILE* stream );

// warning: pthread_kill is not implemented and will always fail
// warning: pthread_cancel is not implemented and will always fail

enum
{
  kMainStatusStarting,
  kMainStatusRunning,
  kMainStatusComplete,
  kMainStatusClosed,
  kMainStatusError,
  kMainStatusUnknown
};

struct MainThreadStatus 
{
  int m_Status;
  int m_Result;
};

typedef struct MainThreadStatus MainThreadStatus ;

pthread_t         g_MainThread;
MainThreadStatus  g_MainThreadStatus;

int    nablue_main(int argc, char **argv);
void*  main_entry(void *parm);

int    __real_write(int, void *, size_t);
int    __wrap_write(int, void *, size_t);

// --------------------------------------------------------------------------
// #HACK Locked posts from whatever thread.
// #HACK Nothing about this is good. 
// ...just for a test at the moment. But I'm cringing just looking at it.
//   #todo Use a real buffer. Argh! allocations everywhere! :(
//   #todo All this locking isn't needed. Fix that.
// --------------------------------------------------------------------------

typedef struct WriteCommand WriteCommand;
struct WriteCommand
{
  int           fd;
  size_t        count;
  WriteCommand* next;
  char          buf[0];
};


pthread_mutex_t   g_WriteMutex        = PTHREAD_MUTEX_INITIALIZER;
WriteCommand*     g_WriteCommandFirst = NULL;
WriteCommand*     g_WriteCommandLast  = NULL;

void FlushWrite();

// --------------------------------------------------------------------------
// Required entry point functions -------------------------------------------
// --------------------------------------------------------------------------

PP_Bool NaBlackHandleInputEvent( PP_Resource input_event )
{
  return PP_TRUE;
}

PP_Bool NaBlackInstanceCreate( uint32_t argc, const char* argn[], const char* argv[] )
{
  int main_start_result;

  // printf("Instance_DidCreate\n");

  g_MainThreadStatus.m_Status = kMainStatusStarting;
  main_start_result           = pthread_create(&g_MainThread, NULL, main_entry, NULL);
  if ( main_start_result != 0 )
  {
    g_MainThreadStatus.m_Status = kMainStatusError;
    fprintf(stderr,"ERROR: main thread not started.\n");
  }

  return PP_TRUE;
}

void NaBlackInstanceDestroy()
{
  // assumes you can't get a destroy call while you're inside a create call
  if ( ( g_MainThreadStatus.m_Status == kMainStatusStarting ) ||
       ( g_MainThreadStatus.m_Status == kMainStatusRunning  ) ||
       ( g_MainThreadStatus.m_Status == kMainStatusComplete ) )
  {
    (void)pthread_join( g_MainThread, NULL );
  }
}

void NaBlackDidChangeView( PP_Resource view )
{
  // printf("Instance_DidChangeView\n");
}

void  NaBlackDidChangeFocus( PP_Bool has_focus )
{
  // printf("Instance_DidChangeFocus\n");
}

PP_Bool NaBlackHandleDocumentLoad( PP_Resource url_loader )
{
  return PP_TRUE;
}

void NaBlackHandleMessage( PP_Var message )
{
}

void NaBlackMouseLockLost()
{
}

void NaBlackRenderFrame()
{
  // check if main thread still running
  if ( g_MainThreadStatus.m_Status == kMainStatusComplete )
  {
    // pthread_join( g_MainThread, NULL );
    g_MainThreadStatus.m_Status = kMainStatusClosed;
  }

  FlushWrite();
}

// --------------------------------------------------------------------------
// Utility functions --------------------------------------------------------
// --------------------------------------------------------------------------

/* Converts a hex character to its integer value */
static char from_hex(char ch) {
  return isdigit((int)ch) ? ch - '0' : tolower((int)ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
static char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

/* Returns a url-encoded version of str */
int strnurlencode( char* dest, const char* str, size_t max_len ) 
{
  const char* pstr = str;
  char        buf[ max_len * 3 + 1 ];
  char*       pbuf = buf;
  int         len;

  while ((*pstr) && ( (uintptr_t)(pstr) - (uintptr_t)(str) < (max_len-1) ))
  {
    if (isalnum((int)*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
    {
      *pbuf++ = *pstr;
    }
    else 
    {
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    }
    pstr++;
  }
  *pbuf = '\0';

  len = (uintptr_t)(pbuf)-(uintptr_t)buf;

  if ( dest )
  {
    strncpy( dest, buf, (len<max_len)?len:max_len );
  }

  return len;
}

/* Returns a url-decoded version of str */
int strnurldecode( char* dest, const char* str, size_t max_len ) 
{
  const char* pstr = str;
  char        buf[ max_len + 1 ];
  char*       pbuf = buf;
  int         len;

  while ((*pstr) && ( (uintptr_t)(pstr) - (uintptr_t)(str) < (max_len-1) ))
  {
    if (*pstr == '%') 
    {
      if (pstr[1] && pstr[2]) 
      {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } 
    else 
    {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';

  len = (uintptr_t)(pbuf)-(uintptr_t)buf;

  if ( dest )
  {
    strncpy( dest, buf, (len<max_len)?len:max_len );
  }

  return len;
}

// --------------------------------------------------------------------------
// Required entry point functions -------------------------------------------
// --------------------------------------------------------------------------

int __wrap_write(int fd, void* buf, size_t count)
{
  int           buffer_size = sizeof( WriteCommand ) + count;
  WriteCommand* buffer      = (WriteCommand*)malloc( buffer_size );


  buffer->fd    = fd;
  buffer->count = count;
  buffer->next  = NULL;
  memcpy( buffer->buf, buf, count );

  pthread_mutex_lock(&g_WriteMutex);

  if ( g_WriteCommandLast )
  {
    g_WriteCommandLast->next = buffer;
    g_WriteCommandLast       = buffer;
  }
  else
  {
    g_WriteCommandFirst      = buffer;
    g_WriteCommandLast       = buffer;
  }

  pthread_mutex_unlock(&g_WriteMutex);

  return ( count );
}

void FlushWrite()
{
  pthread_mutex_lock(&g_WriteMutex);

  WriteCommand* command = g_WriteCommandFirst;
  WriteCommand* next;
  
  while ( command )
  {
    next = command->next;

    int   fd    = command->fd;
    int   count = command->count;
    char* buf   = command->buf;

    int  encoded_len = strnurlencode( NULL, buf, count );
    char message[ encoded_len+1 ];

    strnurlencode( message, buf, encoded_len+1 );
    message[ encoded_len ] = 0;

    if ( fd == fileno(stdout) )
    {
      NaBlackMessagingPostPrintf( "{ \"func\": \"write\", \"dev\": \"/dev/stdout\", \"data\": \"%s\", \"len\": %d }", message, encoded_len );
    } 
    else if ( fd == fileno(stderr) )
    {
      NaBlackMessagingPostPrintf( "{ \"func\": \"write\", \"dev\": \"/dev/stderr\", \"data\": \"%s\", \"len\": %d }", message, encoded_len );
    }
    else /* fd should have come from the browser, so it's aware of it */
    {
      NaBlackMessagingPostPrintf( "{ \"func\": \"write\", \"dev\": \"/dev/fd/%d\", \"data\": \"%s\", \"len\": %d }", fd, message, encoded_len );
    }

    free(command);
    command = next;
  }

  g_WriteCommandFirst = NULL;
  g_WriteCommandLast  = NULL;

  pthread_mutex_unlock(&g_WriteMutex);
}

void* main_entry(void* parm)
{
  (void)parm;

  g_MainThreadStatus.m_Status = kMainStatusRunning;
  g_MainThreadStatus.m_Result = nablue_main( 0, NULL );
  g_MainThreadStatus.m_Status = kMainStatusComplete;

  return NULL;
}
