//
// Created by juangod on 24/03/18.
//https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
//https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "testSlave.h"
#include "messaqueQueue.h"
#include "slave.h"
#include "testLib.h"


int main(int argc, const char ** argv)
{
  if(argc!=4)
  {
    perror("Illegal argument exception.");
    exit(-1);
  }
  mqd_t fileQueueDescriptor = (mqd_t) strtol(argv[1],NULL, BASE10);
  mqd_t hashQueueDescriptor = (mqd_t) strtol(argv[2],NULL, BASE10);
  int isTest = strtol(argv[3],NULL,BASE10);

  if(isTest)
  {
    testRun();
    exit(1);
  }

  char path[MAX_PATH_LEN];
  char buffer[MD5_LEN];

  while(!isEmpty(fileQueueDescriptor))
  {
    getMessage(fileQueueDescriptor, 256, path);
    if(path==NULL)
    {
      printf("Null message\n");
    }
    else
    {
      readMD5(path,buffer);
      sendMessage(buffer, 32, hashQueueDescriptor);
    }
  }
  return 0;
}


int readMD5(const char* path, char* buffer)
{
  if(!is_regular_file(path))
  {
    printf("ERROR: \"%s\" ",path);
    fflush(stdout);
    perror("IS NOT A REGULAR FILE");
    exit(-1);
  }
  char cmd[sizeof(MD5_CMD_FMT)+ MAX_PATH_LEN];
  sprintf(cmd, MD5_CMD_FMT, path);
  FILE *p = popen(cmd, "r");
  if(p==NULL)
  {
    perror("UNRESOLVABLE POPEN ERROR : CODE FF517FDA1");
    exit(-1);
  }
  int i; char ch;
  for (i = 0; i < MD5_LEN; i++)
  {
    ch = fgetc(p);
    if(!isxdigit(ch))
    {
      perror("UNRESOLVABLE MD5SUM ERROR : CODE FF517FDA1");
      exit(-1);
    }
    *(buffer++) = ch;
  }
  *buffer='\0';
  pclose(p);
  return 0;
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
