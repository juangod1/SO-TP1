//
// Created by juangod on 24/03/18.
//
#include "Tests/testlib.h"
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
#include <semaphore.h>

int convertParameterStringToInt(char * param);
char * createConnectionWithSharedMemory(key_t key);
void printSemaphores(char * address);
void openSemaphores(sem_t ** visualConnectedPointer, sem_t ** semaphoreStatusPointer);
void closeSemaphores(sem_t ** visualConnectedPointer, sem_t ** semaphoreStatusPointer);



//Receives PID through argument
int main(int argc, char ** argv)
{
  //If more than 1 argument, abort.
  if(argc != 2)
  { //Argument 0 is the program name.
    perror("Error: One and only argument must be provided.\nExiting program...\n");
    exit(-1);
  }

  char * parameter = argv[1];
  int connectionId = convertParameterStringToInt(parameter);
  char * readingAddress;
  //Starting buffer connection
  readingAddress = createConnectionWithSharedMemory(connectionId);

  sem_t * visSem;
  sem_t * semSem;
  openSemaphores(&visSem,&semSem);

  //Connect to master
  *((char *)readingAddress+1) = GREEN; // Second byte of buffer

  printf("This is the view for the hashing process with PID: %d\n", connectionId);
  while(*((char *)readingAddress+1))
  {
    int semaphoreState = *((char *)readingAddress+2); // Third byte of buffer

    switch(semaphoreState)
    {
        case GREEN:
            printf("%s\n",readingAddress+3);
            *((char *)readingAddress+2) = RED;
            sem_post(visSem);
            break;
        case RED:
            sem_wait(semSem);
            break;
        default:
            perror("Illegal semaphore state ERROR");
            exit(-1);
    }
    }
}

int convertParameterStringToInt(char * param)
{
  return atoi(param);
}

char * createConnectionWithSharedMemory(key_t key)
{
  int connectionId;
  char * readingAddress;
  //Attempting to locate the shared memory
  if((connectionId = shmget(key, BUFFER_SIZE, 0666)) < 0)
  {
    perror("Failed to locate shared memory.\n");
    exit(-1);
  }

  //Attempting to create a connection with data space
  if((readingAddress = shmat(connectionId, 0, 0)) == (char*) -1)
  {
    perror("Failed to connect with data space.\n");
    exit(-1);
  }

  //Checking safety code
  if(*readingAddress != 100)
  {
    perror("Connection with unknown source");
    exit(-1);
  }
  return readingAddress;
}

void printSemaphores(char * address)
{
  printf("Visual:%d, Semaphore:%d\n",(char)*((char *)address),(char)*((char *)address+1));
}

void openSemaphores(sem_t ** visualConnectedPointer, sem_t ** semaphoreStatusPointer)
{
    //Visual connection with initial value 0
    if((*visualConnectedPointer = sem_open("/visualConnected", O_CREAT, 0660, 0)) == SEM_FAILED)
    {
        perror("Failed to open visual semaphore\n");
        exit(-1);
    }
    //Semaphore state with initial value 0
    if((*semaphoreStatusPointer = sem_open("/semaphoreStatus", O_CREAT, 0660, 0)) == SEM_FAILED)
    {
        perror("Failed to open status semaphore\n");
        exit(-1);
    }
}

//Closes the semaphores. Handle errors
void closeSemaphores(sem_t ** visualConnectedPointer, sem_t ** semaphoreStatusPointer)
{
    sem_unlink("/visualConnected");
    sem_close(*visualConnectedPointer);
    sem_unlink("/semaphoreStatus");
    sem_close(*semaphoreStatusPointer);
}
