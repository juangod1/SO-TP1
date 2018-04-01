#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messageQueue.h"
#include "master.h"

int main(int argc, const char ** argv){
    if(argc==1) // Argument zero is the program name
    {
        perror("Error: 0 program arguments received.\nExiting program...\n");
        exit(-1);
    }
    if(argc == 2 && strcmp(argv[1], "test")==0) //test Mode check
    {
      printf("\n_____________________________\n\n.....Entering test mode.....\n\n_____________________________\n\n");
      run(argc,argv,1);
    }
    else
    {
      run(argc,argv,0);
    }
}
