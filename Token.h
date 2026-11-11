#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
  KEYWORD_SELECT, KEYWORD_FROM, KEYWORD_WHERE, KEYWORD_AND, KEYWORD_OR,
  IDENTIFIER,
  INTEGER_LITERAL, DECIMAL_LITERAL,
  STRING_LITERAL,
  OP_EQUAL, OP_NOT_EQUAL, OP_LESS, OP_GREATER, OP_LESS_EQUAL, OP_GREATER_EQUAL,
  COMMA, SEMICOLON, STAR,
  TOKEN_EOF,
  TOKEN_ERROR
};

struct Position {
  int line;
  int column;
};

struct Token {
  TokenType type;
  std::string lexeme;
  Position pos;
};

#endif

