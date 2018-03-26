//
// Created by juangod on 23/03/18.
//
#include "messageQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#define QUEUE_OPTIONS 0666

mqd_t createQueue(const char* mqName, long messageSize, long maxMessages) {
    if(maxMessages>readSystemMaxMsg()){
        printf("----------------------------------------\nUnable to complete operation: Your max_msg system variable (%d) is smaller than number of files sent (%ld).\n You can modify this value in /proc/sys/fs/mqueue/msg_max\n----------------------------------------\nExiting the program...\n",readSystemMaxMsg(),maxMessages);
        exit(-1);
    }

    mqd_t queueDescriptor;
    struct mq_attr queueAttributes;
    int flags = O_CREAT|O_RDWR;

    queueAttributes.mq_flags = O_NONBLOCK;
    queueAttributes.mq_msgsize = messageSize;
    queueAttributes.mq_maxmsg = maxMessages;

    queueDescriptor = mq_open(mqName,flags,QUEUE_OPTIONS,&queueAttributes);
    if (queueDescriptor==-1){
        perror("mq_open() ERROR");
        exit(1);
    }

    return queueDescriptor;
}

ssize_t getMessage(mqd_t queueDescriptor, size_t bufferSize, char * buffer){
    ssize_t ret;
    if((ret = mq_receive(queueDescriptor,buffer,bufferSize,0))==-1){
        perror("mq_receive ERROR");
    }
    return ret;
}

void sendMessage(const char * msg, size_t msgLen,mqd_t queueDescriptor){
    if(mq_send(queueDescriptor,msg,msgLen,0)==-1)
        perror("mq_send ERROR");
}

void closeHashQueue(){
    if(mq_unlink("/hashQueue")==-1)
        perror("no /hashQueue previously left open... (mq_unlink) ");
}

void closeFileQueue(){
    if(mq_unlink("/fileQueue")==-1)
        perror("no /fileQueue previously left open... (mq_unlink) ");
}

int isEmpty(mqd_t queueID){
  return !numberOfMessages(queueID);
}

long numberOfMessages(mqd_t queueID){
    struct mq_attr attributes;
    if(mq_getattr(queueID, &attributes)==-1){
        perror("mq_getattr ERROR");
    }
    return attributes.mq_curmsgs;
}

int readSystemMaxMsg(){
    FILE *file = popen("cat /proc/sys/fs/mqueue/msg_max","r");
    char buffer[7];
    fgets(buffer,6,file);
    return (int)strtol(buffer,NULL,BASE10);
}