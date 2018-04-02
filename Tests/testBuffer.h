#ifndef TESTBUFFER_H
#define TESTBUFFER_H


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

#endif
