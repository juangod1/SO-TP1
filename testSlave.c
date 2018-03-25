#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "testSlave.h"
#include "testLib.h"
#include "slave.h"


char testPath[MAX_PATH_LEN];
char buffer[MD5_LEN];

int testRun()
{
    testExistingFilePrint();
    return 0;
}

void testExistingFilePrint(){
  printf("Testing Existing File Print...  \n");
  givenExistingFile();

  whenExecuting(buffer);

  thenValueIsNotNull(buffer);
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
