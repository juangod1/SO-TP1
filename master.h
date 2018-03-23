//
// Created by juangod on 23/03/18.
//
#include <stdlib.h>

#ifndef SO_TP1_MASTER_H
#define SO_TP1_MASTER_H

#define RED 0
#define GREEN 1
void * createBuffer(size_t size, int num_of_slaves);
void * createSemaphore(void * buffer, int num_of_slaves);

#endif //SO_TP1_MASTER_H
