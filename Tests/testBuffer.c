#include <stdlib.h>
#include <stdio.h>

#include "testlib.h"
#include "testBuffer.h"


char * buffer;
char * readString;
key_t key;
char * string;
char * writeString;

int main()
{
    printf("Test create buffer: ");
    createBufferTest();

    printf("Test read write buffer: ");
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
    //createBufferAlternate(key, buffer); ESTA FUNCION NO EXISTE
}

void thenSomeBufferIsCreated()
{
    checkIsNotNull(buffer);
}

void givenABuffer()
{
    //createBufferAlternate(key, buffer); ESTA FUNCION NO EXISTE
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
