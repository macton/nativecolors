// IBM grants you a nonexclusive copyright license to use all programming code examples from which you can 
// generate similar function tailored to your own specific needs.
// 
// SUBJECT TO ANY STATUTORY WARRANTIES WHICH CANNOT BE EXCLUDED, IBM, ITS PROGRAM DEVELOPERS AND SUPPLIERS 
// MAKE NO WARRANTIES OR CONDITIONS EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OR CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT, 
// REGARDING THE PROGRAM OR TECHNICAL SUPPORT, IF ANY.
// 
// UNDER NO CIRCUMSTANCES IS IBM, ITS PROGRAM DEVELOPERS OR SUPPLIERS LIABLE FOR ANY OF THE FOLLOWING, 
// EVEN IF INFORMED OF THEIR POSSIBILITY:
// 
// LOSS OF, OR DAMAGE TO, DATA;
// DIRECT, SPECIAL, INCIDENTAL, OR INDIRECT DAMAGES, OR FOR ANY ECONOMIC CONSEQUENTIAL DAMAGES; OR
// LOST PROFITS, BUSINESS, REVENUE, GOODWILL, OR ANTICIPATED SAVINGS.
// SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OR LIMITATION OF DIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, 
// SO SOME OR ALL OF THE ABOVE LIMITATIONS OR EXCLUSIONS MAY NOT APPLY TO YOU.

/*
Filename: ATEST22TLS.QCSRC
The output of this example is as follows:
 Enter Testcase - LIBRARY/ATEST22TLS
 Create/start threads
 Wait for the threads to complete, and release their resources
 Thread 0000000000000036: Entered
 Thread 0000000000000037: Entered
 Thread 0000000000000036: foo(), TLS data=0 2
 Thread 0000000000000036: bar(), TLS data=0 2
 Thread 0000000000000037: foo(), TLS data=1 4
 Thread 0000000000000037: bar(), TLS data=1 4
 Main completed
*/
#define _MULTI_THREADED
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void foo(void);  /* Functions that use the TLS data */
void bar(void);
 
#define checkResults(string, val) {             \
 if (val) {                                     \
   printf("Failed with %d at %s", val, string); \
   exit(1);                                     \
 }                                              \
}
 
__thread int TLS_data1;
__thread int TLS_data2;
 
#define                 NUMTHREADS   2 

typedef struct {
   int   data1;
   int   data2;
} threadparm_t; 

void *theThread(void *parm)
{
   threadparm_t     *gData;

   printf("Thread %u: Entered\n", pthread_self());

   gData = (threadparm_t *)parm;

   TLS_data1 = gData->data1;
   TLS_data2 = gData->data2;

   foo();
   return NULL;
}
 
void foo() {
   printf("Thread %u: foo(), TLS data=%d %d\n",
          pthread_self(), TLS_data1, TLS_data2);
   bar();
}
 
void bar() {
   printf("Thread %u: bar(), TLS data=%d %d\n",
          pthread_self(), TLS_data1, TLS_data2);
   return;
}
 

int main(int argc, char **argv)
{
  pthread_t             thread[NUMTHREADS];
  int                   rc=0;
  int                   i;
  threadparm_t          gData[NUMTHREADS];
 
  printf("Enter Testcase - %s\n", argv[0]);
 
  printf("Create/start threads\n");
  for (i=0; i < NUMTHREADS; i++) { 
     /* Create per-thread TLS data and pass it to the thread */
     gData[i].data1 = i;
     gData[i].data2 = (i+1)*2;
     rc = pthread_create(&thread[i], NULL, theThread, &gData[i]);
     checkResults("pthread_create()\n", rc);
  }
 
  printf("Wait for the threads to complete, and release their resources\n");
  for (i=0; i < NUMTHREADS; i++) {
     rc = pthread_join(thread[i], NULL);
     checkResults("pthread_join()\n", rc);
  }

  printf("Main completed\n");
  return 0;
}
