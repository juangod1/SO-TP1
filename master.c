//
// Created by juangod on 23/03/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <linux/limits.h>
#include "master.h"
#include "messageQueue.h"
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "masterTest.h"

void sigint(int);

//Var for shared memory space. Cuando todo este funcionando, se pone su .h correspondiente
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
int connectionId;
char * bufferAddress;
void createBufferAlternate(key_t key);
void testBufferAlternate();

void run(int argc, const char ** argv, int testMode){

    int queueIDs[2]={0};

    if(testMode){
      int * status;
      createTestQueue(queueIDs);
      createTestSlave();
      wait(status);
      fflush(stdout);
      closeFileQueue();
      closeHashQueue();
      exit(1);
    }

    int parametersOffset = (testMode ? 2 : 1 );// Due to testing flag existing or not existing
    int hashCount = 0;
    int numberOfFiles = argc - parametersOffset;
    FILE *fileToWrite;
    fileToWrite =  fopen("hashDump.txt","a");

    // Creates shared memory buffer for view process and message queues for slave processes
    void * sharedBuffer = createBuffer(BUFFER_SIZE);
    createMasterQueues(numberOfFiles,queueIDs);

    //Testing alternateBuffer
    testBufferAlternate();

    // Queue files for slaves to poll
    for(int i=0; i<numberOfFiles; i++) {

        sendMessage(argv[i+parametersOffset], strlen(argv[i+parametersOffset]), FILEQ_ID);
    }

    // Set the signal listener
    struct sigaction sigact;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = sigint;
    if (sigaction(SIGHUP, &sigact, NULL) < 0) {
        perror("sigaction()");
        exit(-1);
    }
    // Launch slave processes
    createSlaves(numberOfFiles,testMode);
    // Process cycle
    while(hashCount != (numberOfFiles)){
        int visualIsConnected = *((char *)sharedBuffer); // First byte of buffer
        int semaphoreState = *((char *)sharedBuffer+1); // Second byte of buffer
        char hashBuffer[HASH_SIZE+1] = {0};

        switch(semaphoreState){
            case RED:
                cleanBuffer(sharedBuffer,BUFFER_SIZE);
                if(isEmpty(HASHQ_ID)){
                  //printf("Queue is empty.... waiting\n");
                  break;
                }
                if (getMessage(HASHQ_ID,HASH_SIZE,hashBuffer)>0)
                    hashCount++;
                printf("Received message: %s\n",hashBuffer);
                memcpy(sharedBuffer+2,hashBuffer,HASH_SIZE);
                //maybe we should integrate the hash format with the MD5_CMD_FMT form the salve.
                fprintf(fileToWrite,"file hash: %s \n",hashBuffer);
                // TODO: write hash to file on disc

                *((char *)sharedBuffer+1) = GREEN;
                break;
            case GREEN:
                if(visualIsConnected){
                    while(*((char *)sharedBuffer+1));// TODO: espera no activa
                }
                else
                    *((char *)sharedBuffer+1) = RED;
                break;
            default:
                perror("Illegal semaphore state ERROR");
                exit(-1);
        }
    }
    fclose(fileToWrite);
}

void  createSlaves(int numberOfFiles, int testMode){
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

    for(int i=0;i<numberOfSlaves;i++){
        if( (pid=fork()) == 0){ // Child process
            if(execv("./Binaries/slave", parameters) == -1)
                perror("execv ERROR");

            exit(-1);
        }
    }
}

// The first byte of the buffer is a 1 if the view process is connected, 0 if it is not
// The second byte is the semaphore for the view process
// When RED the master process cleans the buffer, reads a hash, writes the hash to the file output and sets the semaphore GREEN
// When GREEN the view process reads from the buffer, displays the information and sets the semaphore RED
// GREEN evaluates to true, while RED evaluates to false
void * createBuffer(size_t size){
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    void * buffer = mmap(NULL, size, protection, visibility, 0, 0); // MAN PAGE: If addr is NULL, then the kernel chooses the address at which to create the mapping.

    *((char*)buffer) = 0;
    *((char*)buffer+1) = RED; // Initializes semaphore as RED (Control to master proces)

    return buffer;
}


//El key que recibe tiene que ser el PID del proceso actual.
void createBufferAlternate(key_t key){

    //Attempting to create the shared memory
  if((connectionId = shmget(key, BUFFER_SIZE, IPC_CREAT |0666)) < 0){
    perror("Failed to create shared memory.\n");
    exit(1);
  }
  printf("%d\n", connectionId);

  //Attempting to create a connection with data space
  if((bufferAddress = (char*)shmat(connectionId, 0, 0)) == (char*) -1){
    perror("Failed to connect with data space.\n");
    exit(1);
  }

  printf("%p\n", bufferAddress);

    //*(bufferAddress) = 0;
    //*(bufferAddress+1) = RED; // Initializes semaphore as RED (Control to master proces)
}

//Test para bufferAlternate
void testBufferAlternate(){
    key_t key = 1234;
    createBufferAlternate(key);
    *bufferAddress = 'r';
    printf("%p\n", bufferAddress);
    printf("Este es el dato que guardo en el buffer --> %c\n", *bufferAddress);
}

// Returns queueDescriptorArray with mqd_t of both queues
// int[0] is fileQueue, int[1] is hashQueue
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray){
    mqd_t fileQueue, hashQueue;

    closeFileQueue();
    closeHashQueue();

    fileQueue = createQueue("/fileQueue",PATH_MAX,numberOfFiles,O_NONBLOCK);
    hashQueue = createQueue("/hashQueue",HASH_SIZE,numberOfFiles,0);

    queueDescriptorArray[0] = fileQueue;
    queueDescriptorArray[1] = hashQueue;

    return queueDescriptorArray;
}

int slaveNumberCalculator(int numberOfFiles){
    return (int)ceil((double)(numberOfFiles)/PONDERATIVE_PROCESS_INDEX);
}

void cleanBuffer(void * buff, int buffSize){
    for(int i=0; i<buffSize ; i++)
        *((char*)buff+i)=0;
}

void sigint(int signo)
{
    fflush(stdout);
    printf("Error, exiting\n");
    fflush(stdout);
    //exit(-1);
}
