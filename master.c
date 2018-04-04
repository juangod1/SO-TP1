//
// Created by juangod on 23/03/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <linux/limits.h>
#include "master.h"
#include "messageQueue.h"
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "Tests/masterTest.h"
#include "Tests/testBuffer.h"
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int connectionId;


void run(int argc, const char ** argv, int mode)
{
  if(mode==TEST)
  {
    testModeRun();
    exit(1);
  }
  //BufferAdress for shared memory
  char * bufferAddress;

  key_t uniqueKeyPid = getpid();//The view will know the PID and will use it as well
  cleanBufferConnections(uniqueKeyPid);
  createBufferConnection(uniqueKeyPid, &bufferAddress);

  sem_t * visSem;
  sem_t * semSem;

  int parametersOffset = ((mode==WAIT) ? 2 : 1 );// Due to flag existing or not existing
  int hashCount = 0;
  int numberOfFiles = argc - parametersOffset;
  FILE *fileToWrite;

  int queueIDs[2]={0};
  createMasterQueues(numberOfFiles,queueIDs);
  fileToWrite =  fopen("HashDump/hashDump.txt","a");

  numberOfFiles=sendFilesToQueue(numberOfFiles,argv,argc,parametersOffset, queueIDs);
  putchar('\n');
  if(numberOfFiles<=0)
  {
      printf("Error: no programs are hashable\nExiting program...\n");
      exit(-1);
  }

  createSlaves(numberOfFiles,0);

  openSemaphores(&visSem,&semSem);
  semaphoreInitiation(bufferAddress);

  if(mode==WAIT){
    waitForViewSystem(uniqueKeyPid, bufferAddress);
  }

  while(hashCount < numberOfFiles)
  {
      char hashBuffer[PATH_MAX+HASH_SIZE+1] = {0};
      switch(*((char *)bufferAddress+2))
      {
          case RED:
              cleanBuffer(bufferAddress,BUFFER_SIZE);
              if(isEmpty(HASHQ_ID)){
                break;
              }
              if (getMessage(HASHQ_ID,HASH_SIZE+PATH_MAX,hashBuffer)>0){
                  hashCount++;
              }
              memcpy(bufferAddress+3,hashBuffer,HASH_SIZE+PATH_MAX);
              fprintf(fileToWrite,"%s\n",hashBuffer);
              PROCESS_TURN_SEMAPHORE_BYTE = GREEN;
              sem_post(semSem);
              break;
          case GREEN:
              if(VIEW_IS_CONNECTED_BYTE){
                  sem_wait(visSem);
              }
              else
                  PROCESS_TURN_SEMAPHORE_BYTE = RED;
              break;
          default:
              perror("Illegal semaphore state ERROR");
              exit(-1);
      }
  }
  fclose(fileToWrite);
  printf("Hashes written to \'HashDump/hashDump.txt\'\n");

  closeProgramConnections(semSem, visSem, uniqueKeyPid, bufferAddress);
}

void semaphoreInitiation(char * bufferAddress)
{
  *((char *)bufferAddress) = 100; //Safety code
  VIEW_IS_CONNECTED_BYTE = RED;
  PROCESS_TURN_SEMAPHORE_BYTE = RED;
}


void closeProgramConnections(sem_t * semSem, sem_t * visSem, key_t uniqueKeyPid, char* bufferAddress)
{
  if(VIEW_IS_CONNECTED_BYTE)
  {
      *((char *)bufferAddress+1) = RED;
      *((char *)bufferAddress+2) = RED;
      sem_post(semSem);
  }
  cleanBufferConnections(uniqueKeyPid);
  closeSemaphores(&visSem, &semSem);
}

void testModeRun()
{
  int * status=malloc(4); pid_t wpid;
  createTestBuffer();
  testMasterRun();
  while ((wpid = wait(status)) > 0); //wait for all child processes
  fflush(stdout);
  closeFileQueue();
  closeHashQueue();
  free(status);
}

int sendFilesToQueue(int numberOfFiles, const char** argv, int argc, int parametersOffset, int *queueIDs)
{
  int numberOfFilesCopy=numberOfFiles;
  for(int i=parametersOffset; i<argc; i++)
  {
      if(is_regular_file(argv[i]))
      {
        sendMessage(argv[i], strlen(argv[i]), FILEQ_ID);
      }
      else
      {
        printf("\'%s\' Ignored. Not a regular file\n",argv[i]);
        numberOfFilesCopy--;
      }
  }
  return numberOfFilesCopy;

}

void  createSlaves(int numberOfFiles, int testMode)
{
    int numberOfSlaves = slaveNumberCalculator(numberOfFiles);
    int pid;
    char numberOfFilesArr[ARG_MAX%10+1] = {};
    char testModeArr[2] = {};

    sprintf(numberOfFilesArr, "%d", numberOfFiles);
    sprintf(testModeArr, "%d", testMode);

    char * parameters[4];

    parameters[0] = "./Binaries/slave";
    parameters[1] = numberOfFilesArr;
    parameters[2] = testModeArr;
    parameters[3] = (char *) NULL;

    for(int i=0;i<numberOfSlaves;i++)
    {
        if( (pid=fork()) == 0)
        {
            if(execv("./Binaries/slave", parameters) == -1)
                perror("execv ERROR");
            exit(-1);
        }
    }
}


void cleanBufferConnections(key_t key)
{
    char buff[10+INT_MAX%10];
    sprintf(buff,"ipcrm -M %d", (int)key);
    system(buff);
}


void createBufferConnection(key_t key, char ** asignedBufferAddress)
{
    //Attempting to create the shared memory
    if((connectionId = shmget(key, BUFFER_SIZE, IPC_CREAT |0666)) < 0)
    {
      perror("Failed to create shared memory.\n");
      exit(-1);
    }
    //Attempting to create a connection with data space
    if(((*asignedBufferAddress) = (char*)shmat(connectionId, 0, 0)) == (char*) -1)
    {
      perror("Failed to connect with data space.\n");
      exit(-1);
    }

    //*(bufferAddress) = 0;
    //*(bufferAddress+1) = RED; // Initializes semaphore as RED (Control to master proces)
}

//Opens two semaphores to comunicate with the view.
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

void waitForViewSystem(key_t uniqueKeyPid, char * bufferAddress)
{
  printf("Waiting for connection with semaphore system. PID: %d\n", uniqueKeyPid);
  for (int i=0; i<60 && VIEW_IS_CONNECTED_BYTE==0; i++)
  {
    if(i%20==0){
      putchar('\n');
    }
    sleep(1);
    putchar('.');
    fflush(stdout);

  }

  putchar('\n');
  putchar('\n');

  if(!VIEW_IS_CONNECTED_BYTE)
  {
    printf("Connection timedout.\nExiting program...\n");
    exit(-1);
  }
}

//Fetch semaphore value and deposit in pointer to int. Checking for error
void fetchSemaphoreValue(sem_t *semaphorePointer, int *semaphoreValue)
{
    if(sem_getvalue(semaphorePointer, semaphoreValue) == -1)
    {
            perror("Failed to fetch status from semaphore\n");
    }
}

// Returns queueDescriptorArray with mqd_t of both queues
// int[0] is fileQueue, int[1] is hashQueue
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray)
{
    mqd_t fileQueue, hashQueue;

    closeFileQueue();
    closeHashQueue();

    fileQueue = createQueue("/fileQueue",PATH_MAX,numberOfFiles,O_NONBLOCK);
    hashQueue = createQueue("/hashQueue",PATH_MAX+HASH_SIZE,numberOfFiles,0);

    queueDescriptorArray[0] = fileQueue;
    queueDescriptorArray[1] = hashQueue;

    return queueDescriptorArray;
}

int slaveNumberCalculator(int numberOfFiles)
{
    return (int)ceil((double)(numberOfFiles)/PONDERATIVE_PROCESS_INDEX);
}

void cleanBuffer(void * buff, int buffSize){
    //Starts cleaning after the to semaphores
    for(int i=3; i<buffSize ; i++)
        *((char*)buff+i)=0;
}
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
