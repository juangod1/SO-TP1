#include <stdlib.h>
#include <stdio.h>

#include "testlib.h"

void createBufferTest();
void readWriteBufferTest();

void givenAKey();
void givenABuffer();
void givenAStringToWrite();

void whenAllocatingBufferSpace();
void whenWritingStringToBuffer();
void whenReadingStringFromBuffer();

void thenStringsMatch();
void thenSomeBufferIsCreated();

void * buffer;
key_t key;
char * string;

int main()
{
    printf("Test create buffer: ");
    creteBufferTest();

    printf("Test read write buffer: ")
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
    //para que quede mas limpio se le tiene que pasar
    //el adress por parametro a createBufferAlternate
    createBufferAlternate(key, buffer);
}

void thenSomeBufferIsCreated()
{
    checkIsNotNull(buffer);
}

void givenABuffer()
{
    createBufferAlternate(key, buffer);
}

void givenAStringToWrite()
{
    writeString = "testString";
}

void whenWritingStringToBuffer()
{
    *buffer = writeString;
}

void whenReadingStringFromBuffer()
{
    readString = buffer;
}

void thenStringsMatch()
{
    checkStringsEqual(readstring, writeString);
}