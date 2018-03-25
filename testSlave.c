#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "testSlave.h"
#include "testLib.h"
#include "slave.h"


char * testPath;

int testRun(int argc, const char ** argv)
{
    testExistingFilePrint();
    return 0;
}

void testExistingFilePrint(){
  printf("Testing Existing File Print...  \n");

  testPath=malloc(30);
  givenExistingFile();

  char* buffer= malloc(MD5_LEN);
  whenExecuting(buffer);

  thenValueIsNotNull(buffer);

  free(testPath);
  free(buffer);
}

void thenValueIsNotNull(char *buffer){
  if(buffer!=NULL){
    printf("%s",buffer);
    Ok();
    return;
  }
  Fail();
}

void givenExistingFile()
{
  sprintf(testPath, "slave");
}

void whenExecuting(char * buffer)
{
  readMD5(testPath, buffer);
}
