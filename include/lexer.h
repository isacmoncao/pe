/*
 * Copyright (c) 2025 José Isac Araujo Monção
 *
 * See LICENSE file for copyright and license details.
 */
#ifndef LEXER_H
#define LEXER_H

typedef enum {
  TOKEN_EOF,
  TOKEN_ALGORITHM,
  TOKEN_ENDPROGRAM,
  TOKEN_BEGIN,
  TOKEN_VARIABLES,
  TOKEN_INT_TYPE,
  TOKEN_REAL_TYPE,
  TOKEN_BOOL_TYPE,
  TOKEN_STR_TYPE,
  TOKEN_CHAR_TYPE,
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_ENDIF,
  TOKEN_SWITCH,
  TOKEN_CASE,
  TOKEN_ENDSWITCH,
  TOKEN_WHILE,
  TOKEN_DO,
  TOKEN_ENDWHILE,
  TOKEN_FOR,
  TOKEN_ENDFOR,
  TOKEN_ASSIGN,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIVIDE,
  TOKEN_MOD,
  TOKEN_RAD,
  TOKEN_EQUAL,
  TOKEN_DIFF,
  TOKEN_GREATER,
  TOKEN_LESSER,
  TOKEN_GEQUAL,
  TOKEN_LEQUAL,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_READ,
  TOKEN_WRITE,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_CHAR,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_DOT,
  TOKEN_COMMA,
  TOKEN_COLON,
  TOKEN_SEMICOLON,
  TOKEN_ERROR
} token_t;

typedef struct {
  token_t type;
  char lexeme[100];
  int line;
  int col;
} token;

typedef struct {
  const char* source;
  int pos;
  int line;
  int col;
} lexer;

void lexer_init(lexer *lex, const char *source);
token lexer_next_token(lexer *lex);
void token_print(const token tkn);

#endif
