#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>

#include "testSlave.h"
#include "testlib.h"
#include "../slave.h"
#include "../messageQueue.h"

void testMD5Hashing();

void givenExistingFile();

void whenCalculatingHash();

void thenReturnValueIsNotNull();

char testPath[MAX_PATH_LEN];
char buffer[MD5_LEN];
char testBuffer[MAX_PATH_LEN];

int testRun(mqd_t fileQueueDescriptor, mqd_t hashQueueDescriptor)
{
    printf("Test MD5 Hashing:");
    testMD5Hashing();
    //printf("Test getting item from file queue:");
    //testGettingItemFromFileQueue(fileQueueDescriptor);
    return 0;
}
//TODO: esto es testeo de posix?
// void testGettingItemFromFileQueue(mqd_t fileQueueDescriptor)
// {
//   givenAnExistingTestMessage();
//   whenReadingQueueDescriptor(fileQueueDescriptor);
//   thenFoundMessage();
// }

// void   givenAnExistingTestMessage()
// {

// }

// void whenReadingQueueDescriptor(mqd_t fileQueueDescriptor)
// {
//     getMessage(fileQueueDescriptor, MAX_PATH_LEN, testBuffer);
// }

// void thenFoundMessage()
// {
//   checkStringsEqual(testBuffer, "test");
// }

//TODO: agregar al informe que esto es meritorio
//por que esta nuestra funcion en el medio.
void testMD5Hashing()
{
  givenExistingFile();
  whenCalculatingHash();
  thenReturnValueIsNotNull();
}

void givenExistingFile()
{
  sprintf(testPath, "messageQueue.c");
}

void thenReturnValueIsNotNull()
{
  checkIsNotNull(buffer);
}

void whenCalculatingHash()
{
  readMD5(testPath, buffer);
}
