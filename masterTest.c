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

  queueIDs=createMasterQueues(1,queueIDs);
  if(sendMessage(msg, sizeof(msg) ,FILEQ_ID)==0 && !isEmpty(FILEQ_ID))
  {
    printf("Wrote \"test\" to queue\n");
  }
  else
  {
    printf("Failed to write \"test\" to queue\n");
  }
}

void createTestSlave()
{
  createSlaves(1, 1);
}
