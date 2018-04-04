#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <linux/limits.h>
#include "../master.h"
#include "../messageQueue.h"
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "testlib.h"
#include "masterTest.h"

int queueIdentificators[2]={0};

void testMasterRun()
{
  printf("Testing queue creation:");
  givenNothing();
  whenCreatingQueue();
  thenQueueIsCreated();
  printf("Testing item addition:");
  givenNothing();
  whenAddingItemToFileQueue();
  thenFileQueueIsNotEmpty();
  createTestSlave();
}

void whenCreatingQueue()
{
  createMasterQueues(1, queueIdentificators);
}

void thenQueueIsCreated()
{
  if(queueIdentificators[0]!=0 || queueIdentificators[1]!=0)
  {
    ok();
  }
  else
  {
    fail("Expected: non-0 queueIdentificators array, recieved: value 0 queueIdentificators");
  }
}

void whenAddingItemToFileQueue()
{
  const char * msg = "test";
  sendMessage(msg, sizeof(msg) ,queueIdentificators[0]);
}

void thenFileQueueIsNotEmpty()
{
  char destinationOfMessage[MAX_PATH_LEN];
  getMessage(queueIdentificators[0], MAX_PATH_LEN, destinationOfMessage);
  if(destinationOfMessage!=NULL)
  {
    ok();
  }
  else
  {
    fail("Expected: non-null buffer, received: null buffer");
  }
}

void createTestSlave()
{
  createSlaves(1, 1);
}
