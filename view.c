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
#include "view.h"


void testPrintToTerminal();
void testPrintArgumentToTerminal();
void testPrintAfterChange();

void givenAString();
void givenAChange();

void whenPrintingToStandardOutput();

void setString(char * stringValue);

int convertParameterStringToInt(char * param);
char * createConnectionWithSharedMemory(key_t key);
void testConnection();
void testBufferConnection(key_t key);
void printSemaphores(char * address);



//Receives PID through argument
int main(int argc, char ** argv){
  //If more than 1 argument, abort.
  if(argc != 2){ //Argument 0 is the program name.
    perror("Error: One and only argument must be provided.\nExiting program...\n");
    exit(-1);
  }

  //Uncomment after testing and erase connectionId 1234
  //char * parameter = argv[1];
  //int connectionId = convertParameterStringToInt(parameter);
  int connectionId = 1234;
  char * readingAddress;
  //Starting buffer connection
  readingAddress = createConnectionWithSharedMemory(connectionId);

  // printf("Testing Print parameter to terminal...\n");
  // testPrintArgumentToTerminal();
  // testPrintToTerminal();

  // printf("Testing Print after a change of value...\n");
  // testPrintAfterChange();

  //Connect to master
  *((char *)readingAddress) = GREEN; // First byte of buffer
  int visualIsConnected = GREEN;
  
  printf("Now testing bufferConnection\n");
  while(visualIsConnected){

        visualIsConnected = *((char *)readingAddress); // First byte of buffer
        int semaphoreState = *((char *)readingAddress+1); // Second byte of buffer

        switch(semaphoreState){
            case GREEN:
                printf("Received message: %s\n",readingAddress+2);
                *((char *)readingAddress+1) = RED;
                break;
            case RED:
                 while(!(*((char *)readingAddress+1)) && (*((char *)readingAddress)));
                break;
            default:
                perror("Illegal semaphore state ERROR");
                exit(-1);
        }
    }

}

void testPrintAfterChange(){
  givenAString();
  givenAChange();

  whenPrintingToStandardOutput();

  thenSuccess();
}

void testPrintToTerminal(){
  givenAString();
  whenPrintingToStandardOutput();
  thenSuccess();
}

void testPrintArgumentToTerminal(){
  whenPrintingToStandardOutput();
  thenSuccess();
}

void givenAString(){
  setString("String");
}

void givenAChange(){
  setString("Change");
}

void setString(char * stringValue){
}

void whenPrintingToStandardOutput(){
  //printf("The parameter is: %s\n",parameter);
}

void testConnection(){
  //createConnectionWithSharedMemory(convertParameterStringToInt(parameter));
}

int convertParameterStringToInt(char * param){
  return atoi(param);
}

char * createConnectionWithSharedMemory(key_t key){
  int connectionId;
  char * readingAddress;
  //Attempting to locate the shared memory
  if((connectionId = shmget(key, BUFFER_SIZE, 0666)) < 0){
    perror("Failed to locate shared memory.\n");
    exit(1);
  }

  //Attempting to create a connection with data space
  if((readingAddress = shmat(connectionId, 0, 0)) == (char*) -1){
    perror("Failed to connect with data space.\n");
    exit(1);
  }
  return readingAddress;
}

void testBufferConnection(key_t key){
  createConnectionWithSharedMemory(key);
  //printf("Este es el dato que recibo en el buffer --> %s\n", readingAddress);
}

void printSemaphores(char * address){
  printf("Visual:%d, Semaphore:%d\n",(char)*((char *)address),(char)*((char *)address+1));
}