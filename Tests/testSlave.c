#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "testSlave.h"
#include "testlib.h"
#include "../slave.h"
#include "../messageQueue.h"

char testPath[MAX_PATH_LEN];
char buffer[MD5_LEN];
char testBuffer[MAX_PATH_LEN];

int testRun(mqd_t fileQueueDescriptor, mqd_t hashQueueDescriptor)
{
    testExistingFilePrint();
    testGettingItemFromFileQueue(fileQueueDescriptor);
    return 0;
}

void testGettingItemFromFileQueue(mqd_t fileQueueDescriptor)
{
  printf("Testing getting an existing item from the queue...\n");
  printf("Amount of elements in queue: %ld\n", numberOfMessages(fileQueueDescriptor));
  givenAnExistingTestMessage();
  whenReadingQueueDescriptor(fileQueueDescriptor);
  thenFoundMessage();
}

void   givenAnExistingTestMessage()
{
}

void whenReadingQueueDescriptor(mqd_t fileQueueDescriptor)
{
    getMessage(fileQueueDescriptor, MAX_PATH_LEN, testBuffer);
}

void thenFoundMessage()
{
  if(strcmp(testBuffer,"test")==0)
  {
    ok();
  }
  else
  {
    char errormsg[50]={0};
    sprintf(errormsg,"Expected: \"test\" found %s: \n", testBuffer);
    fail(errormsg);
  }
}

void testExistingFilePrint()
{
  printf("Testing Existing File Print...  \n");
  givenExistingFile();

  whenExecuting(buffer);

  thenValueIsNotNull(buffer);
}

void thenValueIsNotNull(char *buffer)
{
  if(buffer!=NULL){
    printf("%s\n",buffer);
    ok();
    return;
  }
  fail("Value is null");
}

void givenExistingFile()
{
  sprintf(testPath, "messageQueue.c");
}

void whenExecuting(char * buffer)
{
  readMD5(testPath, buffer);
}
