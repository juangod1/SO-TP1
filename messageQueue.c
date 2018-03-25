//
// Created by juangod on 23/03/18.
//
#include "messaqueQueue.h"
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
    mqd_t queueDescriptor;
    struct mq_attr queueAttributes;
    int flags = O_RDWR|O_CREAT;

    queueAttributes.mq_flags = O_NONBLOCK;
    queueAttributes.mq_msgsize = messageSize;
    queueAttributes.mq_maxmsg = maxMessages;

    queueDescriptor = mq_open(mqName,flags,QUEUE_OPTIONS,&queueAttributes);
    if (queueDescriptor==-1){
        perror("mq_open() ERROR");
    }

    return queueDescriptor;
}

void getMessage(mqd_t queueDescriptor, size_t bufferSize, char * buffer){
    if(mq_receive(queueDescriptor,buffer,bufferSize,0)==-1){
        perror("mq_receive ERROR");
    }
}

void sendMessage(const char * msg, size_t msgLen,mqd_t queueDescriptor){
    if(mq_send(queueDescriptor,msg,msgLen,0)==-1)
        perror("mq_send ERROR");
}

void closeMessageQueue(mqd_t queueDescriptor){
    mq_close(queueDescriptor);
}

int isEmpty(mqd_t queueID){
  return !numberOfMessages(queueID);
}

long numberOfMessages(mqd_t queueID){
    struct mq_attr attributes = {};
    mq_getattr(queueID, &attributes);
    return attributes.mq_curmsgs;
}
