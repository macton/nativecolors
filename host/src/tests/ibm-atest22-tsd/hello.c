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
Filename: ATEST22.QCSRC
The output of this example is as follows:
 Enter Testcase - LIBRARY/ATEST22
 Create/start threads
 Wait for the threads to complete, and release their resources
 Thread 00000000 00000036: Entered
 Thread 00000000 00000036: foo(), threadSpecific data=0 2
 Thread 00000000 00000036: bar(), threadSpecific data=0 2
 Thread 00000000 00000036: Free data
 Thread 00000000 00000037: Entered
 Thread 00000000 00000037: foo(), threadSpecific data=1 4
 Thread 00000000 00000037: bar(), threadSpecific data=1 4
 Thread 00000000 00000037: Free data
 Main completed
*/

#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
void foo(void);  /* Functions that use the threadSpecific data */
void bar(void);
void dataDestructor(void *data);
 
#define checkResults(string, val) {             \
 if (val) {                                     \
   printf("Failed with %d at %s", val, string); \
   exit(1);                                     \
 }                                              \
}
 
typedef struct {
  int          threadSpecific1;
  int          threadSpecific2;
} threadSpecific_data_t;
 
#define                 NUMTHREADS   2
pthread_key_t           threadSpecificKey;
 
 
void *theThread(void *parm)
{
   int               rc;
   threadSpecific_data_t    *gData;
   printf("Thread %u: Entered\n", pthread_self());
   gData = (threadSpecific_data_t *)parm;
   rc = pthread_setspecific(threadSpecificKey, gData);
   checkResults("pthread_setspecific()\n", rc);
   foo();
   return NULL;
}
 
void foo() {
   threadSpecific_data_t *gData = pthread_getspecific(threadSpecificKey);
   printf("Thread %u: foo(), threadSpecific data=%d %d\n",
          pthread_self(), gData->threadSpecific1, gData->threadSpecific2);
   bar();
}
 
void bar() {
   threadSpecific_data_t *gData = pthread_getspecific(threadSpecificKey);
   printf("Thread %u: bar(), threadSpecific data=%d %d\n",
          pthread_self(), gData->threadSpecific1, gData->threadSpecific2);
   return;
}
 
void dataDestructor(void *data) {
   printf("Thread %u: Free data\n", pthread_self());
   pthread_setspecific(threadSpecificKey, NULL);
   free(data);
}
 
 
int main(int argc, char **argv)
{
  pthread_t             thread[NUMTHREADS];
  int                   rc=0;
  int                   i;
  threadSpecific_data_t        *gData;
 
  printf("Enter Testcase - %s\n", argv[0]);
  rc = pthread_key_create(&threadSpecificKey, dataDestructor);
  checkResults("pthread_key_create()\n", rc);
 
  printf("Create/start threads\n");
  for (i=0; i <NUMTHREADS; i++) {
       /* Create per-thread threadSpecific data and pass it to the thread */
     gData = (threadSpecific_data_t *)malloc(sizeof(threadSpecific_data_t));
     gData->threadSpecific1 = i;
     gData->threadSpecific2 = (i+1)*2;
     rc = pthread_create(&thread[i], NULL, theThread, gData);
     checkResults("pthread_create()\n", rc);
  }
 
  printf("Wait for the threads to complete, and release their resources\n");
  for (i=0; i <NUMTHREADS; i++) {
  rc = pthread_join(thread[i], NULL);
     checkResults("pthread_join()\n", rc);
  }
 
  pthread_key_delete(threadSpecificKey);
  printf("Main completed\n");
  return 0;
}
