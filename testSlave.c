#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "testSlave.h"
#include "testLib.h"
#include "slave.h"
#include "messageQueue.h"

char testPath[MAX_PATH_LEN];
char buffer[MD5_LEN];
char testBuffer[5];

int testRun(mqd_t fileQueueDescriptor, mqd_t hashQueueDescriptor)
{
    testExistingFilePrint();
    testGettingItemFromFileQueue(fileQueueDescriptor);
    return 0;
}

void testGettingItemFromFileQueue(mqd_t fileQueueDescriptor){
  givenAnExistingTestMessage();
  whenReadingQueueDescriptor();
  thenFoundMessage();
}

void   givenAnExistingTestMessage(){

}

void whenReadingQueueDescriptor(mqd_t fileQueueDescriptor){
    getMessage(fileQueueDescriptor, 5, testBuffer);
}

void thenFoundMessage(){
  if(strcmp(testBuffer,"test")==0){
    Ok();
  }
  else{
    printf("Expected: \"test\" found %s: \n", testBuffer);
    Fail();
  }
}

void testExistingFilePrint(){
  printf("Testing Existing File Print...  \n");
  givenExistingFile();

  whenExecuting(buffer);

  thenValueIsNotNull(buffer);
}

void thenValueIsNotNull(char *buffer){
  if(buffer!=NULL){
    printf("%s\n",buffer);
    Ok();
    return;
  }
  Fail();
}

void givenExistingFile()
{
  sprintf(testPath, "messageQueue.c");
}

void whenExecuting(char * buffer)
{
  readMD5(testPath, buffer);
}
