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

void * createBuffer(size_t size);
int * createMasterQueues(int numberOfFiles, int * queueDescriptorArray);
void run(int argc, const char ** argv);
int slaveNumberCalculator(int argc);
void createSlaves(int numberOfSlaves, const char ** argv);

#endif //SO_TP1_MASTER_H
