//
// Created by juangod on 23/03/18.
//

#ifndef SO_TP1_MESSAQUEQUEUE_H
#define SO_TP1_MESSAQUEQUEUE_H

#include <mqueue.h>
#include <stddef.h>

mqd_t createQueue(const char* mqName, long messageSize, long maxMessages);
void getMessage(mqd_t queueDescriptor, size_t messageSize, char * buffer);
void sendMessage(const char * msg, size_t msgLen,mqd_t queueDescriptor);
void closeMessageQueue(mqd_t queueDescriptor);
long numberOfMessages(mqd_t queueID);

#endif //SO_TP1_MESSAQUEQUEUE_H
