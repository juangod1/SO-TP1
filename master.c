//
// Created by juangod on 23/03/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/limits.h>
#include "master.h"
#include "messaqueQueue.h"
#include <unistd.h>
#include <math.h>

void run(int argc, const char ** argv){
    // Creates shared memory buffer for view process and message queues for slave processes
    void * sharedBuffer = createBuffer(BUFFER_SIZE);
    int queueIDs[2]={0};
    createMasterQueues(argc,queueIDs);
    printf("%d %d\n",queueIDs[0],queueIDs[1]);
    // Launch slave processes
    createSlaves(slaveNumberCalculator(argc),argv);
}

void createSlaves(int numberOfSlaves, const char ** argv){
    int pid;
    for(int i=0;i<numberOfSlaves;i++){
        if( (pid=fork()) == 0){ // Child process
            if(execv("./Binaries/slave",(char * const *)argv) == -1)
                perror("execve ERROR");

            exit(-1);
        }
    }
}

// The first byte is the semaphore for the view process
// When RED the master process cleans the buffer, reads a hash, writes the hash to the file output and sets the semaphore GREEN
// When GREEN the view process reads from the buffer, displays the information and sets the semaphore RED
// GREEN evaluates to true, while RED evaluates to false
void * createBuffer(size_t size){
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    void * buffer = mmap(NULL, size, protection, visibility, 0, 0); // MAN PAGE: If addr is NULL, then the kernel chooses the address at which to create the mapping.

    *((char*)buffer)= RED; // Initializes semaphore as RED (Control to master proces)

    return buffer;
}

// Returns queueDescriptorArray with mqd_t of both queues
// int[0] is fileQueue, int[1] is hashQueue
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray){
    mqd_t fileQueue, hashQueue;

    fileQueue = createQueue("/fileQueue",PATH_MAX,numberOfFiles);
    hashQueue = createQueue("/hashQueue",HASH_SIZE,numberOfFiles);

    queueDescriptorArray[0] = fileQueue;
    queueDescriptorArray[1] = hashQueue;

    return queueDescriptorArray;
}

int slaveNumberCalculator(int argc){
    return (int)ceil((double)(argc-1)/PONDERATIVE_PROCESS_INDEX);
}

