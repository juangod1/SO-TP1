//
// Created by juangod on 23/03/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "master.h"

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
