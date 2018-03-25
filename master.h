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
#define PID_MAX_DIGITS 5

void * createBuffer(size_t size);
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray);
void run(int argc, const char ** argv);
int slaveNumberCalculator(int argc);
void createSlaves(int numberOfSlaves, int queueIDs[2]);
void cleanBuffer(void * buff, int buffSize);

#endif //SO_TP1_MASTER_H
