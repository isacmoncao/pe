/*
 * Copyright (c) 2025 José Isac Araujo Monção
 *
 * See LICENSE file for copyright and license details.
 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
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

static token lexer_read_number(lexer *lex) {
  token tkn;
  tkn.type = TOKEN_NUMBER;
  tkn.line = lex->line;
  tkn.col = lex->col;

  int i = 0;
  while (isdigit((unsigned int)lexer_peek(lex))) {
    if (i < 99)
      tkn.lexeme[i++] = lexer_advance(lex);
    else
     lexer_advance(lex);
  }

  tkn.lexeme[i] = '\0';
  return tkn;
}

static token lexer_read_string(lexer *lex) {
  token tkn;
  tkn.type = TOKEN_STRING;
  tkn.line = lex->line;
  tkn.col = lex->col;
  
  // Consume opening string
  lexer_advance(lex);

  int i = 0;
  char c;
  while ((c = lexer_peek(lex)) != '"' && c != '\0') {
    if (i < 99)
      tkn.lexeme[i++] = lexer_advance(lex);
    else
      lexer_advance(lex);
  }

  if (c == '\0') {
    tkn.type = TOKEN_ERROR;
    strcpy(tkn.lexeme, "Unterminated string");
    return tkn;
  } 

  // Consume close string
  lexer_advance(lex);
  tkn.lexeme[i] = '\0';
  return tkn;
}

static token lexer_read_char(lexer *lex) {
  token tkn;
  tkn.type = TOKEN_CHAR;
  tkn.line = lex->line;
  tkn.col = lex->col;

  lexer_advance(lex);

  int i = 0;
  char c = lexer_peek(lex);
  if (c == '\0') {
    tkn.type = TOKEN_ERROR;
    strcpy(tkn.lexeme, "Unterminated char");
    return tkn;
  }

  // Handle escape sequences
  if (c == '\\') {
    if (i < 2)
      tkn.lexeme[i++] = lexer_advance(lex);

    char next = lexer_peek(lex);

    if (next != '\0' && i < 2)
      tkn.lexeme[i++] = lexer_advance(lex);

  } else {
    if (i < 2)
      tkn.lexeme[i++] = lexer_advance(lex);
  }

  c = lexer_peek(lex);
  if (c != '\'') {
    tkn.type = TOKEN_ERROR;
    strcpy(tkn.lexeme, "Char too long or missing closing quote");
    return tkn;
  }

  lexer_advance(lex);
  tkn.lexeme[i] = '\0';
  if (i > 2) {
    tkn.type = TOKEN_ERROR;
    strcpy(tkn.lexeme, "Invalid char literal");
  }
  return tkn;
}

static token_t check_keyword(const char* lexeme) {
  if (strcmp(lexeme, "ALGORITMO") == 0) return TOKEN_ALGORITHM;
  if (strcmp(lexeme, "VARIAVEIS") == 0) return TOKEN_VARIABLES;
  if (strcmp(lexeme, "INICIO") == 0) return TOKEN_BEGIN;
  if (strcmp(lexeme, "se") == 0) return TOKEN_IF;
  if (strcmp(lexeme, "entao") == 0) return TOKEN_THEN;
  if (strcmp(lexeme, "senao") == 0) return TOKEN_ELSE;
  if (strcmp(lexeme, "fimse") == 0) return TOKEN_ENDIF;
  if (strcmp(lexeme, "faca") == 0) return TOKEN_DO;
  if (strcmp(lexeme, "para") == 0) return TOKEN_FOR;
  if (strcmp(lexeme, "fimpara") == 0) return TOKEN_ENDFOR;
  if (strcmp(lexeme, "enquanto") == 0) return TOKEN_WHILE;
  if (strcmp(lexeme, "fimenquanto") == 0) return TOKEN_ENDWHILE;
  if (strcmp(lexeme, "escolha") == 0) return TOKEN_SWITCH;
  if (strcmp(lexeme, "caso") == 0) return TOKEN_CASE;
  if (strcmp(lexeme, "fimescolha") == 0) return TOKEN_ENDSWITCH;
  if (strcmp(lexeme, "FIM") == 0) return TOKEN_ENDPROGRAM;
  if (strcmp(lexeme, "inteiro") == 0) return TOKEN_INT_TYPE;
  if (strcmp(lexeme, "logico") == 0) return TOKEN_BOOL_TYPE;
  if (strcmp(lexeme, "real") == 0) return TOKEN_REAL_TYPE;
  if (strcmp(lexeme, "caractere") == 0) return TOKEN_CHAR_TYPE;
  if (strcmp(lexeme, "string") == 0) return TOKEN_STR_TYPE;
  if (strcmp(lexeme, "leia") == 0) return TOKEN_READ;
  if (strcmp(lexeme, "escreva") == 0) return TOKEN_WRITE;
  if (strcmp(lexeme, "rad") == 0) return TOKEN_RAD;
  if (strcmp(lexeme, "mod") == 0) return TOKEN_MOD;
  if (strcmp(lexeme, "ou") == 0) return TOKEN_OR;
  if (strcmp(lexeme, "nao") == 0) return TOKEN_NOT;
  return TOKEN_IDENTIFIER;
}

static token read_identifier_basic(lexer *lex) {
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

static token lexer_read_identifier(lexer *lex) {
  token tkn = read_identifier_basic(lex);
  tkn.type = check_keyword(tkn.lexeme);
  return tkn;
}

token lexer_next_token(lexer *lex) {
  lexer_skip_whitespace(lex);
  char current = lexer_peek(lex);
  
  if (current == '\0') {
    token tkn;
    tkn.type = TOKEN_EOF;
    strcpy(tkn.lexeme, "EOF");
    tkn.line = lex->line;
    tkn.col = lex->col;
    return tkn;
  }

  if (isdigit(current))
    return lexer_read_number(lex);

  if (isalpha(current) || current == '_')
    return lexer_read_identifier(lex);

  if (current == '"')
    return lexer_read_string(lex);

  if (current == '\'')
    return lexer_read_char(lex);

  token tkn;
  tkn.line = lex->line;
  tkn.col = lex->col;
  
  switch (current) {
    case '+':
      tkn.type = TOKEN_PLUS;
      strcpy(tkn.lexeme, "+");
      lexer_advance(lex);
      break;
    case '-':
      tkn.type = TOKEN_MINUS;
      strcpy(tkn.lexeme, "-");
      lexer_advance(lex);
      break;
    case '*':
      tkn.type = TOKEN_MULTIPLY;
      strcpy(tkn.lexeme, "*");
      lexer_advance(lex);
      break;
    case '/':
      tkn.type = TOKEN_DIVIDE;
      strcpy(tkn.lexeme, "/");
      lexer_advance(lex);
      break;
    case '.':
      tkn.type = TOKEN_DOT;
      strcpy(tkn.lexeme, ".");
      lexer_advance(lex);
      break;
    case '<':
      lexer_advance(lex);
      if (lexer_peek(lex) == '-') 
      {
        tkn.type = TOKEN_ASSIGN;
        strcpy(tkn.lexeme, "<-");
      } else if (lexer_peek(lex) == '=') 
      {
        tkn.type = TOKEN_LEQUAL;
        strcpy(tkn.lexeme, "<=");
      } else if (lexer_peek(lex) == '>') 
      {
        tkn.type = TOKEN_DIFF;
        strcpy(tkn.lexeme, "<>");
      } else 
      {
        tkn.type = TOKEN_LESSER;
        strcpy(tkn.lexeme, "<");
      }
      lexer_advance(lex);
      break;
    case '>':
      lexer_advance(lex);
      if (lexer_peek(lex) == '=') {
        tkn.type = TOKEN_GEQUAL;
        strcpy(tkn.lexeme, ">=");
      } else {
        tkn.type = TOKEN_GREATER;
        strcpy(tkn.lexeme, ">");
      }
      lexer_advance(lex);
      break;
    case '=':
      tkn.type = TOKEN_EQUAL;
      strcpy(tkn.lexeme, "=");
      lexer_advance(lex);
      break;
    case 'e':
      tkn.type = TOKEN_AND;
      strcpy(tkn.lexeme, "e");
      lexer_advance(lex);
      break;
    case '(':
      tkn.type = TOKEN_LPAREN;
      strcpy(tkn.lexeme, "(");
      lexer_advance(lex);
      break;
    case ')':
      tkn.type = TOKEN_RPAREN;
      strcpy(tkn.lexeme, ")");
      lexer_advance(lex);
      break;
    case ',':
      tkn.type = TOKEN_COMMA;
      strcpy(tkn.lexeme, ",");
      lexer_advance(lex);
      break;
    case ':':
      tkn.type = TOKEN_COLON;
      strcpy(tkn.lexeme, ":");
      lexer_advance(lex);
      break;
    case ';':
      tkn.type = TOKEN_SEMICOLON;
      strcpy(tkn.lexeme, ";");
      lexer_advance(lex);
      break;
    default:
      tkn.type = TOKEN_ERROR;
      tkn.lexeme[0] = lexer_advance(lex);
      tkn.lexeme[1] = '\0';
      break;
  }

  return tkn;
}

void token_print(const token tkn) {
  const char *type_names[] = { "TOKEN_EOF",
    "TOKEN_ALGORITHM",
    "TOKEN_ENDPROGRAM",
    "TOKEN_BEGIN",
    "TOKEN_VARIABLES",
    "TOKEN_INT_TYPE",
    "TOKEN_REAL_TYPE",
    "TOKEN_BOOL_TYPE",
    "TOKEN_STR_TYPE",
    "TOKEN_CHAR_TYPE",
    "TOKEN_IF",
    "TOKEN_THEN",
    "TOKEN_ELSE",
    "TOKEN_ENDIF",
    "TOKEN_SWITCH",
    "TOKEN_CASE",
    "TOKEN_ENDSWITCH",
    "TOKEN_WHILE",
    "TOKEN_DO",
    "TOKEN_ENDWHILE",
    "TOKEN_FOR",
    "TOKEN_ENDFOR",
    "TOKEN_ASSIGN",
    "TOKEN_PLUS",
    "TOKEN_MINUS",
    "TOKEN_MULTIPLY",
    "TOKEN_DIVIDE",
    "TOKEN_MOD",
    "TOKEN_RAD",
    "TOKEN_EQUAL",
    "TOKEN_DIFF",
    "TOKEN_GREATER",
    "TOKEN_LESSER",
    "TOKEN_GEQUAL",
    "TOKEN_LEQUAL",
    "TOKEN_AND",
    "TOKEN_OR",
    "TOKEN_NOT",
    "TOKEN_READ",
    "TOKEN_WRITE",
    "TOKEN_IDENTIFIER",
    "TOKEN_NUMBER",
    "TOKEN_STRING",
    "TOKEN_CHAR",
    "TOKEN_LPAREN",
    "TOKEN_RPAREN",
    "TOKEN_DOT",
    "TOKEN_COMMA",
    "TOKEN_COLON",
    "TOKEN_SEMICOLON",
    "TOKEN_ERROR"
  };
  printf("Token[%s] = '%s' at (%d:%d)\n", type_names[tkn.type], tkn.lexeme, tkn.line, tkn.col);
}
