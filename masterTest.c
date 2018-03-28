#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <linux/limits.h>
#include "master.h"
#include "messageQueue.h"
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "masterTest.h"



void createTestQueue(int *queueIDs)
{
  const char * msg = "test";
  createMasterQueues(1,queueIDs);
  sendMessage(msg, sizeof(msg) ,FILEQ_ID);
  printf("Wrote \"test\" to queue\n");
}

void createTestSlave()
{
  createSlaves(1, 1);
}
