#include "messageQueue.h"

#ifndef TEST_SLAVE_H
#define TEST_SLAVE_H

void givenExistingFile();
void whenExecuting();
void thenValueIsNotNull();
void testExistingFilePrint();
void testGettingItemFromFileQueue(mqd_t fileQueueDescriptor);
void givenAnExistingTestMessage();
void whenReadingQueueDescriptor();
void thenFoundMessage();
int testRun();


#endif
