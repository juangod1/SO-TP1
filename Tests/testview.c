#include "testlib.h"
#include "../messageQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include "../view.h"
#include "testview.h"

void testPrintAfterChange()
{
  givenAString();
  givenAChange();

  whenPrintingToStandardOutput();

  thenSuccess();
}

void testPrintToTerminal()
{
  givenAString();
  whenPrintingToStandardOutput();
  thenSuccess();
}

void testPrintArgumentToTerminal()
{
  whenPrintingToStandardOutput();
  thenSuccess();
}

void givenAString()
{
  setString("String");
}

void givenAChange()
{
  setString("Change");
}

void setString(char * stringValue)
{
}

void whenPrintingToStandardOutput()
{
}

void testConnection()
{
}


void testBufferConnection(key_t key)
{
  createConnectionWithSharedMemory(key);
}
