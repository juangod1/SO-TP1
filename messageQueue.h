//
// Created by juangod on 23/03/18.
//

#ifndef SO_TP1_MESSAQUEQUEUE_H
#define SO_TP1_MESSAQUEQUEUE_H

#include <mqueue.h>
#include <stddef.h>

mqd_t createQueue(const char* mqName, long messageSize, long maxMessages, long mqFlags);
ssize_t getMessage(mqd_t queueDescriptor, size_t bufferSize, char * buffer);
int sendMessage(const char * msg, size_t msgLen,mqd_t queueDescriptor);
void closeFileQueue();
void closeHashQueue();
long numberOfMessages(mqd_t queueID);
int isEmpty(mqd_t queueID);
int readSystemMaxMsg();

#define BASE10 10
#define QUEUE_OPTIONS 0666

#endif //SO_TP1_MESSAQUEQUEUE_H
