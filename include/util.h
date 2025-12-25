#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>

int count_bytes(FILE *file);
void file_to_char_arr(char *arr, FILE *file);
void usage(void);
void err_file(const char *file);

#endif

