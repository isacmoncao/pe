/*
 * Copyright (c) 2025 José Isac Araujo Monção
 * All rights reserved.
 *
 * See LICENSE file for copyright and license details.
 */
#include <ctype.h>
#include "../include/lexer.h"

void lexer_init(lexer *lex, const char *source) {
  lex->source = source;
  lex->pos = 0;
  lex->line = 1;
  lex->col = 1;
}

static char lexer_advance(lexer *lex) {
  char c = lex->source[lex->pos++];
  if (c == '\n') {
    lex->line++;
    lex->col = 1;
  } else {
    lex->col++;
  }
  return c;
}

// Look the current char
static char lexer_peek(lexer *lex) {
  return lex->source[lex->pos];
}

static void lexer_skip_whitespace(lexer *lex) {
  while (isspace((unsigned char)lexer_peek(lex))) {
    lexer_advance(lex);
  }
}

static token lexer_read_identifier(lexer *lex) {
  token tkn;
  tkn.type = TOKEN_IDENTIFIER;
  tkn.line = lex->line;
  tkn.col = lex->col;
  
  int i = 0;
  while (isalnum((unsigned char)lexer_peek(lex)) || lexer_peek(lex) == '_') {
    if (i < 99)
      tkn.lexeme[i++] = lexer_advance(lex);
    else
     lexer_advance(lex);
  }
  
  tkn.lexeme[i] = '\0';
  return tkn;
}
