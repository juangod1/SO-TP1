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
  if(sendMessage(msg, sizeof(msg) ,FILEQ_ID)==0 && !isEmpty(FILEQ_ID))
  {
    printf("Wrote \"test\" to queue,%d\nqueue ID: %d\n",numberOfMessages(FILEQ_ID),FILEQ_ID);
    char buffer[10]={0};
    getMessage(FILEQ_ID, 10, buffer);
    printf("%s\n", buffer);
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
