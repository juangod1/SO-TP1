#ifndef TESTLIB_H
#define TESTLIB_H

void checkIsNotNull(void * pointer);
void ok();
void fail(char * errorMsg);
void notImplemented();
void thenSuccess();
void checkStringsEqual(char* str1, char* str2);
void givenNothing();


#endif
