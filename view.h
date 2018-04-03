//
// Created by juangod on 24/03/18.
//

#ifndef SO_TP1_VIEW_H
#define SO_TP1_VIEW_H
#define HASH_SIZE 32
#define BUFFER_SIZE (HASH_SIZE + FILENAME_MAX)
#define GREEN 1
#define RED 0
#define VIEW_IS_CONNECTED_BYTE *((char *)readingAddress+1)
#define PROCESS_TURN_SEMAPHORE_BYTE *((char *)readingAddress+2)

int convertParameterStringToInt(char * param);
char * createConnectionWithSharedMemory(key_t key);
void printSemaphores(char * address);



#endif //SO_TP1_VIEW_H
