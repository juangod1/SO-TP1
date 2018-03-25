#include "testLib.h"
#include <stdio.h>

void Ok(){
  printf("  Ok!\n");
}

void thenSuccess(){
  Ok();
}

void Fail(){
  printf("  Failed\n");
}
