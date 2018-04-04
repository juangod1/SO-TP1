//
// Created by juangod on 23/03/18.
//
#include <stdlib.h>
#include <semaphore.h>


#ifndef SO_TP1_MASTER_H
#define SO_TP1_MASTER_H

#define PONDERATIVE_PROCESS_INDEX 4
#define RED 0
#define GREEN 1
#define HASH_SIZE 32
#define BUFFER_SIZE (HASH_SIZE + FILENAME_MAX)
#define FILEQ_ID queueIDs[0]
#define HASHQ_ID queueIDs[1]
#define QID_MAX_DIGITS 5
#define IS_TEST_SLAVE 2
#define IS_NOT_TEST_SLAVE 1
#define VIEW_IS_CONNECTED_BYTE *((char *)bufferAddress+1)
#define PROCESS_TURN_SEMAPHORE_BYTE *((char *)bufferAddress+2)
#define WAIT 1
#define TEST 2


void * createBuffer(size_t size);
int is_regular_file(const char *path);
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray);
int slaveNumberCalculator(int numberOfFiles);
void  createSlaves(int numberOfFiles, int testMode);
void cleanBuffer(void * buff, int buffSize);

void openSemaphores(sem_t ** semaphoreStatusPointer, sem_t **visualConnectedPointer);
int sendFilesToQueue(int numberOfFiles, const char** argv, int argc, int parametersOffset, int *queueIDs);
void waitForViewSystem(key_t uniqueKeyPid, char* bufferAddress);
void closeSemaphores(sem_t ** visualConnectedPointer, sem_t **semaphoreStatusPointer);
void fetchSemaphoreValue(sem_t *semaphorePointer, int *semaphoreValue);
void createBufferConnection(key_t key, char ** asignedBufferAddress);
void cleanBufferConnections(key_t key);
void semaphoreInitiation(char * bufferAddress);
void closeProgramConnections(sem_t * semSem, sem_t * visSem, key_t uniqueKeyPid, char* bufferAddress);

void testModeRun();

void run(int argc, const char ** argv, int testMode);

#endif //SO_TP1_MASTER_H
