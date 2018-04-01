//
// Created by juangod on 24/03/18.
//
#include "testLib.h"
#include "messageQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>

#define BUFFER_SIZE 32



void testPrintToTerminal();
void testPrintArgumentToTerminal();
void testPrintAfterChange();

void givenAString();
void givenAChange();

void whenPrintingToStandardOutput();

void setString(char * stringValue);

int convertParameterStringToInt(char * param);
void createConnectionWithSharedMemory(key_t key);
void testConnection();
void testBufferConnection(key_t key);

char * parameter;
int connectionId;
char * readingAddress;

//Receives PID through argument
int main(int argc, char ** argv)
{
  //If more than 1 argument, abort.
  if(argc != 2){ //Argument 0 is the program name.
    perror("Error: One and only argument must be provided.\nExiting program...\n");
    exit(-1);
  }

  setString(argv[1]);
  //testConnection();

  printf("Testing Print parameter to terminal...\n");
  testPrintArgumentToTerminal();
  //testPrintToTerminal();

  printf("Testing Print after a change of value...\n");
  testPrintAfterChange();

  printf("Now testing bufferConnection\n");
  testBufferConnection(1234);
}

void testPrintAfterChange()
{
  givenAString();
  givenAChange();

  whenPrintingToStandardOutput();

  thenSuccess();
}

void testPrintToTerminal()
{
  givenAString();
  whenPrintingToStandardOutput();
  thenSuccess();
}

void testPrintArgumentToTerminal()
{
  whenPrintingToStandardOutput();
  thenSuccess();
}

void givenAString(){
  setString("String");
}

void givenAChange(){
  setString("Change");
}

void setString(char * stringValue)
{
  parameter=stringValue;
}

void whenPrintingToStandardOutput()
{
  printf("The parameter is: %s\n",parameter);
}

void testConnection()
{
  createConnectionWithSharedMemory(convertParameterStringToInt(parameter));
}

int convertParameterStringToInt(char * param)
{
  return atoi(param);
}

void createConnectionWithSharedMemory(key_t key)
{
  //Attempting to locate the shared memory
  if((connectionId = shmget(key, BUFFER_SIZE, 0666)) < 0)
  {
    perror("Failed to locate shared memory.\n");
    exit(1);
  }
  printf("%d\n", connectionId);

  //Attempting to create a connection with data space
  if((readingAddress = shmat(connectionId, 0, 0)) == (char*) -1)
  {
    perror("Failed to connect with data space.\n");
    exit(1);
  }

  printf("%p\n", readingAddress);
}

void testBufferConnection(key_t key)
{
  createConnectionWithSharedMemory(key);
  printf("%p\n", readingAddress);
  printf("Este es el dato que recibo en el buffer --> %c\n", *readingAddress);
}
