//
// Created by juangod on 24/03/18.
//

#include <stdio.h>
#include <unistd.h>

int main(){
    int pid = getpid();
    printf("HELLO I AM %d\n",pid);
}