//
// Created by juangod on 23/03/18.
//

#ifndef SO_TP1_MESSAQUEQUEUE_H
#define SO_TP1_MESSAQUEQUEUE_H

struct message {
    long messageNumber;
    char hash[32];
} messageCDT;

typedef struct messageCDT * message;

#endif //SO_TP1_MESSAQUEQUEUE_H
