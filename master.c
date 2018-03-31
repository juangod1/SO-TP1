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
#include <limits.h>

void sigint(int);

//Var for shared memory space. Cuando todo este funcionando, se pone su .h correspondiente
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

int connectionId;

void openSemaphores(sem_t ** semaphoreStatusPointer, sem_t **visualConnectedPointer);
void closeSemaphores(sem_t ** visualConnectedPointer, sem_t **semaphoreStatusPointer);
void fetchSemaphoreValue(sem_t *semaphorePointer, int *semaphoreValue);
void createBufferConnection(key_t key, char ** asignedBufferAddress);
void cleanBufferConnections(key_t key);
//Testing Buffer ToDelete
void testBufferAlternate();

void run(int argc, const char ** argv, int testMode){
    //BufferAdress for shared memory 
    char * bufferAddress;

    //Uncomment when testing ends
    key_t uniqueKeyPid = 1234;
    //key_t uniqueKeyPid = getpid();//The view will know the PID and will use it as well
    cleanBufferConnections(uniqueKeyPid);
    createBufferConnection(uniqueKeyPid, &bufferAddress);
    
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
    createMasterQueues(numberOfFiles,queueIDs);

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

    //Process cycle
    *((char *)bufferAddress) = RED;
    *((char *)bufferAddress+1) = RED;
    //Wait 10 seconds to be able to run view
    sleep(10);

    while(hashCount != (numberOfFiles)){
        //This two variables can be moved to the beginning of "run".
        int visualIsConnected = *((char *)bufferAddress); // First byte of buffer
        int semaphoreState = *((char *)bufferAddress+1); // Second byte of buffer
        char hashBuffer[HASH_SIZE+1] = {0};

        switch(semaphoreState){
            case RED:
                cleanBuffer(bufferAddress,BUFFER_SIZE);//This clean might 
                if(isEmpty(HASHQ_ID)){
                  //printf("Queue is empty.... waiting\n");
                  break;
                }
                if (getMessage(HASHQ_ID,HASH_SIZE,hashBuffer)>0)
                    hashCount++;
                printf("Received message: %s\n",hashBuffer);
                memcpy(bufferAddress+2,hashBuffer,HASH_SIZE);
                //maybe we should integrate the hash format with the MD5_CMD_FMT form the salve.
                fprintf(fileToWrite,"file hash: %s \n",hashBuffer);
                // TODO: write hash to file on disc
                *((char *)bufferAddress+1) = GREEN;
                break;
            case GREEN:
                if(visualIsConnected){
                    while(*((char *)bufferAddress+1));
                }
                else
                    *((char *)bufferAddress+1) = RED;
                break;
            default:
                perror("Illegal semaphore state ERROR");
                exit(-1);
        }
    }
    fclose(fileToWrite);
    //Wait 2 seconds in case view did not finish printing and then disconnect
    if(*((char *)bufferAddress)){
        sleep(2);
        *((char *)bufferAddress) = RED;
    }
    void cleanBufferConnections(key_t key);
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


void cleanBufferConnections(key_t key){
    //Removes the shared memory asigned previously
    char buff[10+INT_MAX%10];
    sprintf(buff,"ipcrm -M %d", (int)key);
    system(buff);
}

//El key que recibe tiene que ser el PID del proceso actual.
void createBufferConnection(key_t key, char ** asignedBufferAddress){
    //Attempting to create the shared memory
    if((connectionId = shmget(key, BUFFER_SIZE, IPC_CREAT |0666)) < 0){
    perror("Failed to create shared memory.\n");
    exit(-1);
    }
    //Attempting to create a connection with data space
    if(((*asignedBufferAddress) = (char*)shmat(connectionId, 0, 0)) == (char*) -1){
    perror("Failed to connect with data space.\n");
    exit(-1);
    }

    //*(bufferAddress) = 0;
    //*(bufferAddress+1) = RED; // Initializes semaphore as RED (Control to master proces)
}

//Opens two semaphores to comunicate with the view.
void openSemaphores(sem_t ** visualConnectedPointer, sem_t ** semaphoreStatusPointer){
    //Visual connection with initial value 0
    if((*visualConnectedPointer = sem_open("/visualConnected", O_CREAT, 0660, 0)) == SEM_FAILED){
        perror("Failed to open visual semaphore\n");
        exit(-1);
    }
    //Semaphore state with initial value 0
    if((*semaphoreStatusPointer = sem_open("/semaphoreStatus", O_CREAT, 0660, 0)) == SEM_FAILED){
        perror("Failed to open status semaphore\n");
        exit(-1);
    }
}

//Closes the semaphores. Handle errors
void closeSemaphores(sem_t ** visualConnectedPointer, sem_t ** semaphoreStatusPointer){
    sem_unlink("/visualConnected");
    sem_close(*visualConnectedPointer);
    sem_unlink("/semaphoreStatus");
    sem_close(*semaphoreStatusPointer);
}

//Fetch semaphore value and deposit in pointer to int. Checking for error
void fetchSemaphoreValue(sem_t *semaphorePointer, int *semaphoreValue){
    if(sem_getvalue(semaphorePointer, semaphoreValue) == -1){
            perror("Failed to fetch status from semaphore\n");
    }
}

// Returns queueDescriptorArray with mqd_t of both queues
// int[0] is fileQueue, int[1] is hashQueue
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray){
    mqd_t fileQueue, hashQueue;

    closeFileQueue();
    closeHashQueue();

    fileQueue = createQueue("/fileQueue",PATH_MAX,numberOfFiles);
    hashQueue = createQueue("/hashQueue",HASH_SIZE,numberOfFiles);

    queueDescriptorArray[0] = fileQueue;
    queueDescriptorArray[1] = hashQueue;

    return queueDescriptorArray;
}

int slaveNumberCalculator(int numberOfFiles){
    return (int)ceil((double)(numberOfFiles)/PONDERATIVE_PROCESS_INDEX);
}

void cleanBuffer(void * buff, int buffSize){
    //Starts cleaning after the to semaphores
    for(int i=2; i<buffSize ; i++)
        *((char*)buff+i)=0;
}

void sigint(int signo)
{
    fflush(stdout);
    printf("Error, exiting\n");
    fflush(stdout);
    //exit(-1);
}

//Tests del Buffer Address ToDelete
// void testBufferAlternate(char * hashBufferTest){
//     key_t key = getpid();
//     createBufferAlternate(key);

//     memcpy(bufferAddress,hashBufferTest,BUFFER_SIZE);

//     printf("Este es el dato que guardo en el buffer --> %s\n", bufferAddress);
// }
