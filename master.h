//
// Created by juangod on 23/03/18.
//
#include <stdlib.h>

#ifndef SO_TP1_MASTER_H
#define SO_TP1_MASTER_H

#define PONDERATIVE_PROCESS_INDEX 4
#define RED 0
#define GREEN 1
#define HASH_SIZE 32
#define BUFFER_SIZE 32
#define FILEQ_ID queueIDs[0]
#define HASHQ_ID queueIDs[1]
#define QID_MAX_DIGITS 5
#define IS_TEST_SLAVE 2
#define IS_NOT_TEST_SLAVE 1

void * createBuffer(size_t size);
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray);
int slaveNumberCalculator(int numberOfFiles);
void  createSlaves(int numberOfFiles, int testMode);
void cleanBuffer(void * buff, int buffSize);
void run(int argc, const char ** argv, int testMode);

#endif //SO_TP1_MASTER_H
