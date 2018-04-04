#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testlib.h"

void givenNothing()
{
}

void checkIsNotNull(void * pointer)
{
    if(pointer == NULL)
    {
        fail("Expected: a non null pointer, recieved: a null pointer");
    }
    else
    {
        ok();
    }
}

void checkStringsEqual(char* str1, char* str2)
{
    if(strcmp(str1, str2) != 0)
    {
        fail("Expected: two equal strings, recieved: two different strings");
    }
    else
    {
        ok();
    }
}

void thenSuccess()
{
  ok();
}

void ok()
{
    printf("\tOk!\n");
}

void fail(char * errorMsg)
{
    printf("\tFailed\n");
    fprintf(stderr, "%s", errorMsg);
}

void notImplemented()
{
    fflush(stdout);
    fprintf(stderr, "Not implemented");
    exit(1);
}
