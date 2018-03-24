#include <stdio.h>
#include <stdlib.h>
#include "messaqueQueue.h"

int main(int argc, const char ** argv){
    if(argc==1){ // Argument zero is the program name
        perror("Error: 0 program arguments received.\nExiting program...\n");
        exit(-1);
    }


    // ejemplo de prueba del message queue
    const char * q_name = "/TESTasdf";
    mqd_t q_descriptor = createQueue(q_name,10,10);
    const char * message = "message";
    sendMessage(message,8,q_descriptor);
    char * a = malloc(11);
    getMessage(q_descriptor,11,a);
    printf("%s\n",a);
    free(a);
}
