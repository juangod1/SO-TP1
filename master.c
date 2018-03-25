//
// Created by juangod on 23/03/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/limits.h>
#include "master.h"
#include "messageQueue.h"
#include <unistd.h>
#include <math.h>
#include <string.h>

void run(int argc, const char ** argv){
    int hashCount = 0;

    // Creates shared memory buffer for view process and message queues for slave processes
    void * sharedBuffer = createBuffer(BUFFER_SIZE);
    int queueIDs[2]={0};
    createMasterQueues(argc,queueIDs);

    // Queue files for slaves to poll
    for(int i=1; i<argc; i++) {
        sendMessage(argv[i], strlen(argv[i]), FILEQ_ID);
    }
    printf("messages %d\n",(int)numberOfMessages(FILEQ_ID));fflush(stdout);
    // TODO:if is test argument launch test slave

    // Launch slave processes
    createSlaves(slaveNumberCalculator(argc),queueIDs);

    // Process cycle
    while(hashCount != (argc-1)){
        printf("HASH COUNT: %d\n",hashCount);fflush(stdout);
        int visualIsConnected = *((char *)sharedBuffer); // First byte of buffer
        int semaphoreState = *((char *)sharedBuffer+1); // Second byte of buffer
        char hashBuffer[HASH_SIZE] = {0};

        switch(semaphoreState){
            case RED:
                cleanBuffer(sharedBuffer,BUFFER_SIZE);
                // TODO: if received message
                getMessage(HASHQ_ID,HASH_SIZE,hashBuffer);
                hashCount++;
                memcpy(sharedBuffer+2,hashBuffer,HASH_SIZE);
                // TODO: write hash to file on disc
                *((char *)sharedBuffer+1) = GREEN;
                break;
            case GREEN:
                if(visualIsConnected){
                    while(*((char *)sharedBuffer+1));
                }
                else
                    *((char *)sharedBuffer+1) = RED;
                break;
            default:
                perror("Illegal semaphore state ERROR");
                exit(-1);
        }
    }
    printf("Done!\n");
}

void createTestSlave(){

}

void createSlaves(int numberOfSlaves, int queueIDs[2]){
    int pid;
    char slaveFile[2] = {};
    char slaveHash[2] = {};
    char slaveTest[2] = {};

    sprintf(slaveFile, "%d", FILEQ_ID);
    sprintf(slaveHash, "%d", HASHQ_ID);
    sprintf(slaveTest, "%d", IS_NOT_TEST_SLAVE);

    char * parameters[5];

    parameters[0] = "./Binaries/slave";
    parameters[1] = slaveFile;
    parameters[2] = slaveHash;
    parameters[3] = slaveTest;
    parameters[4] = (char *) NULL;

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

// Returns queueDescriptorArray with mqd_t of both queues
// int[0] is fileQueue, int[1] is hashQueue
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray){
    mqd_t fileQueue, hashQueue;

    fileQueue = createQueue("/fileQueue",PATH_MAX,numberOfFiles);
    hashQueue = createQueue("/hashQueue",HASH_SIZE,numberOfFiles);

    closeFileQueue();
    closeHashQueue();

    fileQueue = createQueue("/fileQueue",PATH_MAX,numberOfFiles);
    hashQueue = createQueue("/hashQueue",HASH_SIZE,numberOfFiles);

    queueDescriptorArray[0] = fileQueue;
    queueDescriptorArray[1] = hashQueue;

    return queueDescriptorArray;
}

int slaveNumberCalculator(int argc){
    return (int)ceil((double)(argc-1)/PONDERATIVE_PROCESS_INDEX);
}

void cleanBuffer(void * buff, int buffSize){
    for(int i=0; i<buffSize ; i++)
        *((char*)buff+i)=0;
}
