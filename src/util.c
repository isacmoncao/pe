#include "../include/util.h"
#include <stdio.h>

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
