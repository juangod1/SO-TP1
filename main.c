#include <stdio.h>
#include <stdlib.h>
#include "messageQueue.h"
#include "master.h"

int main(int argc, const char ** argv){
    if(argc==1){ // Argument zero is the program name
        perror("Error: 0 program arguments received.\nExiting program...\n");
        exit(-1);
    }

    //TODO: RECEIVE OPTION TEST

    run(argc,argv,0);
}
