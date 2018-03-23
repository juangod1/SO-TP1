//
// Created by juangod on 23/03/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "master.h"

// The first n (n = num_of_slaves) bytes in the buffer correspond to the semaphore structure.
// Each byte can be RED or GREEN, GREEN evaluates to true, while RED evaluates to false.
// The nth byte corresponds to the access state of the nth slave.
void * createBuffer(size_t size, int num_of_slaves){
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_ANONYMOUS | MAP_SHARED;
    void * buffer = mmap(NULL, size, protection, visibility, 0, 0); // MAN PAGE: If addr is NULL, then the kernel chooses the address at which to create the mapping.

    return buffer;
}

void * createSemaphore(void * buffer, int num_of_slaves){
    for(int i=0 ; i<num_of_slaves ; i++){
        * (char*)(buffer+i) = RED; // Semaphores start at red
    }
}