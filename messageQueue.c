//
// Created by juangod on 23/03/18.
//
#include "messaqueQueue.h
#include <sys/ipc.h>
#include <sys/msg.h>
#include <wchar.h>

int createQueue() {
    key_t key = ftok("messageQueue",69);
    int msgid = msgget(key, 0666|IPC_CREAT); // second argument sets read-write permissions for everyone

    return msgid;
}

message getMessage(int queueID){
    message received={0};
    msgrcv(queueID, received,sizeof(message), 1, 0);
    return received;
}

void destroyMessageQueue(int queueID){
    msgctl(queueID, IPC_RMID, NULL);
}