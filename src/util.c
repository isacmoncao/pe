#include "../include/util.h"
#include <stdio.h>
#include <stdlib.h>

int count_bytes(FILE *file) {
  fseek(file, 0, SEEK_SET);
  int bytes = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF)
    bytes++;
  return bytes;
}

void file_to_char_arr(char *arr, FILE *file) {
  fseek(file, 0, SEEK_SET);
  int i = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    arr[i] = ch;
    i++;
  }
  arr[i] = '\0';
}

void usage(void) {
    fprintf(stderr, "usage: %s <source_file>\n", getprogname());
    exit(EXIT_FAILURE);
}

void err_file(const char *file) {
    fprintf(stderr, "%s: error while opening %s\n"
            "Does the file exists or do you have reading permissions?\n",
            getprogname(), file);
    exit(EXIT_FAILURE);
}
