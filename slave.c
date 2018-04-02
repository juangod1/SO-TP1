//
// Created by juangod on 24/03/18.
//https://stackoverflow.com/questions/3395690/md5sum-of-file-in-linux-c
//https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "testSlave.h"
#include "messageQueue.h"
#include "slave.h"
#include "testLib.h"

// Receives three arguments, program name, number of files and isTest
int main(int argc, const char ** argv)
{
  if(argc!=3)
  {
    perror("Illegal argument exception.");
    exit(-1);
  }
  mqd_t fileQueueDescriptor, hashQueueDescriptor;
  long numberOfFiles = strtol(argv[1],NULL,BASE10);
  long isTest = strtol(argv[2],NULL,BASE10);

  fileQueueDescriptor = createQueue("/fileQueue",MAX_PATH_LEN,numberOfFiles,O_NONBLOCK);
  hashQueueDescriptor = createQueue("/hashQueue",HASH_SIZE+MAX_PATH_LEN,numberOfFiles,0);

  if(isTest)
  {
    testRun(fileQueueDescriptor,hashQueueDescriptor);
    fflush(stdout);
    exit(1);
  }

  char path[MAX_PATH_LEN];
  char buffer[MD5_LEN + MAX_PATH_LEN];

  while(!isEmpty(fileQueueDescriptor))
  {
    //cleaning buffer.
    for(int i=0; i<MAX_PATH_LEN ; i++)
    {
        path[i]=0;
    }

    getMessage(fileQueueDescriptor, MAX_PATH_LEN, path);
    if(path==NULL)
    {
      printf("Null message\n");
    }
    else
    {
      readMD5(path,buffer);
      sendMessage(buffer, MAX_PATH_LEN+HASH_SIZE, hashQueueDescriptor);
    }
  }
    exit(0);
}


int readMD5(const char* path, char* buffer)
{
  if(!is_regular_file(path))
  {
    printf("ERROR: \"%s\" ",path);
    fflush(stdout);
    perror("IS NOT A REGULAR FILE");
    buffer = "Undetermined Hash due to error";
    return -1;
  }
  char cmd[sizeof(MD5_CMD_FMT)+ MAX_PATH_LEN];
  sprintf(cmd, MD5_CMD_FMT, path);
  FILE *p = popen(cmd, "r");
  if(p==NULL)
  {
    perror("UNRESOLVABLE POPEN ERROR : CODE FF517FDA1");
    pclose(p);
    return -1;
  }
  int i; char ch;
  for (i = 1; i < MAX_PATH_LEN && path[i] != '\0'; i++)
  {
    *(buffer++) = path[i];
  }
  *(buffer++) = ':';
  *(buffer++) = ' ';
  for (i = 0; i < MD5_LEN; i++)
  {
    ch = fgetc(p);
    if(!isxdigit(ch))
    {
      perror("UNRESOLVABLE MD5SUM ERROR : CODE FF517FDA1");
        pclose(p);

      return -1;
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
