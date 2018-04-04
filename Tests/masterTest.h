#ifndef MASTERTEST_H
#define MASTERTEST_H


void createTestSlave();
void testMasterRun();
void whenCreatingQueue();
void thenQueueIsCreated();
void whenAddingItemToFileQueue();
void thenFileQueueIsNotEmpty();

#define MAX_PATH_LEN 4096

#endif
