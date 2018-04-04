#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#include "testlib.h"
#include "testBuffer.h"
#include "../master.h"

void createTestBuffer();
void createBufferTest();
void readWriteBufferTest();

void givenAKey();
void givenABuffer();
void givenAStringToWrite();

void whenAllocatingBufferSpace();
void whenWritingStringToBuffer();
void whenReadingStringFromBuffer();

void thenSomeBufferIsCreated();
void thenStringsMatch();

void createBufferConnection(key_t key, char ** buffer);

char * buffer;
char * readString;
key_t key;
char * string;
char * writeString;


void createTestBuffer()
{
    printf("Test create buffer:");
    createBufferTest();

    printf("Test read write buffer:");
    readWriteBufferTest();
}

void createBufferTest()
{
  givenAKey();
  whenAllocatingBufferSpace();
  thenSomeBufferIsCreated();
}

void readWriteBufferTest()
{
  givenABuffer();
  givenAStringToWrite();
  whenWritingStringToBuffer();
  whenReadingStringFromBuffer();
  thenStringsMatch();
}

void givenAKey()
{
    key = 1234;
}

void whenAllocatingBufferSpace()
{
    createBufferConnection(key, &buffer);
}

void thenSomeBufferIsCreated()
{
    checkIsNotNull(buffer);
}

void givenABuffer()
{
    createBufferConnection(key, &buffer);
}

void givenAStringToWrite()
{
    writeString = "testString";
}

void whenWritingStringToBuffer()
{
    buffer = (char*)writeString;
}

void whenReadingStringFromBuffer()
{
    readString = buffer;
}

void thenStringsMatch()
{
    checkStringsEqual(readString, writeString);
}
