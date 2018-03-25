//
// Created by juangod on 24/03/18.
//
#include "testLib.h"
#include "messageQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

void testPrintToTerminal();
void testPrintAfterChange();

void givenAString();
void givenAChange();

void whenPrintingToStandardOutput();

void setString(char * stringValue);

char * parameter;

int main(){
    printf("Testing Print parameter to terminal...\n");
    testPrintToTerminal();

    printf("Testing Print after a change of value...\n");
    testPrintAfterChange();
}

void testPrintAfterChange(){
  givenAString();
  givenAChange();

  whenPrintingToStandardOutput();

  thenSuccess();
}

void testPrintToTerminal(){
  givenAString();
  whenPrintingToStandardOutput();
  thenSuccess();
}

void givenAString(){
  setString("String");
}

void givenAChange(){
  setString("Change");
}

void setString(char * stringValue){
  parameter=stringValue;
}

void whenPrintingToStandardOutput(){
  printf("%s\n",parameter);
}
