/*
 * Copyright (c) 2025 José Isac Araujo Monção
 *
 * See LICENSE file for copyright and license details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "../include/lexer.h"
#include "../include/util.h"

static lexer lex;
static char *code;
static FILE *source_file;
static token tkn;

int main(int argc, char *argv[]) {
  setprogname(argv[0]);

  if (argc != 2)
    usage();
  
  source_file = fopen(argv[1], "r");
  if ( source_file == NULL)
    err_file(argv[1]);

  code = malloc(count_bytes(source_file) * sizeof(char));
  file_to_char_arr(code, source_file);
  lexer_init(&lex, code);

  do {
    tkn = lexer_next_token(&lex);
    token_print(tkn);
  } while (tkn.type != TOKEN_EOF);

  free(code);
  fclose(source_file);
  return EXIT_SUCCESS;
}
