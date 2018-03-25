//
// Created by juangod on 24/03/18.
//

#ifndef SO_TP1_SLAVE_H
#define SO_TP1_SLAVE_H

#define MD5_CMD_FMT "md5sum %s"
#define MAX_PATH_LEN 256
#define MD5_LEN 32
#define BASE10 10
#define IS_TEST_SLAVE 2
#define IS_NOT_TEST_SLAVE 1

int readMD5(const char* path, char* buffer);
int is_regular_file(const char *path);

#endif //SO_TP1_SLAVE_H
