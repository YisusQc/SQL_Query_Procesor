#include "Lexer.h"
#include <cctype>

char Lexer::peek() {
  if (currentPos >= input.length()) return '\0';
  return input[currentPos];
}

char Lexer::advance() {
  if (currentPos >= input.length()) return '\0';
  char c = input[currentPos++];
  
  if (c == '\n') {
    currentLine++;
    currentCol = 1;
  } else {
    currentCol++;
  }
  return c;
}

void Lexer::skipWhitespace() {
  while (std::isspace(peek())) {
    advance();
  }
}

Token Lexer::getNextToken() {
  skipWhitespace();

  Token token;
  token.pos = {currentLine, currentCol};

  // Fin del archivo
  if (currentPos >= input.length()) {
    token.type = TokenType::TOKEN_EOF;
    token.lexeme = "EOF";
    return token;
  }

  char c = peek();

  // 1. IDENTIFICADORES Y KEYWORDS
  if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {

    token.lexeme += advance();

    while (
      std::isalnum(static_cast<unsigned char>(peek())) ||
      peek() == '_'
    ) {
      token.lexeme += advance();
    }

    if (!keywords.isKeyword(token.lexeme, token.type)) {
      token.type = TokenType::IDENTIFIER;
    }

    return token;
  }

  // 2. LITERALES NUMÉRICOS
  if (std::isdigit(static_cast<unsigned char>(c))) {

    token.lexeme += advance();

    while (std::isdigit(static_cast<unsigned char>(peek()))) {
      token.lexeme += advance();
    }

    // Número decimal
    if (peek() == '.') {

      token.lexeme += advance();

      // Debe haber al menos un dígito después del punto
      if (!std::isdigit(static_cast<unsigned char>(peek()))) {
        token.type = TokenType::TOKEN_ERROR;
        return token;
      }

      while (std::isdigit(static_cast<unsigned char>(peek()))) {
        token.lexeme += advance();
      }

      token.type = TokenType::DECIMAL_LITERAL;
    }
    // Número entero
    else {
      token.type = TokenType::INTEGER_LITERAL;
    }

    return token;
  }

  // 3. CADENAS DE TEXTO
  if (c == '\'') {

    token.lexeme += advance();

    while (peek() != '\'' && peek() != '\0' && peek() != '\n') {
      token.lexeme += advance();
    }

    if (peek() == '\'') {
      token.lexeme += advance();
      token.type = TokenType::STRING_LITERAL;
    } else {
      token.type = TokenType::TOKEN_ERROR;
    }

    return token;
  }

  // 4. OPERADORES RELACIONALES
  if (c == '=') {
    token.lexeme += advance();
    token.type = TokenType::OP_EQUAL;
    return token;
  }

  if (c == '<') {

    token.lexeme += advance();

    if (peek() == '=') {
      token.lexeme += advance();
      token.type = TokenType::OP_LESS_EQUAL;
    }
    else if (peek() == '>') {
      token.lexeme += advance();
      token.type = TokenType::OP_NOT_EQUAL;
    }
    else {
      token.type = TokenType::OP_LESS;
    }

    return token;
  }

  if (c == '>') {

    token.lexeme += advance();

    if (peek() == '=') {
      token.lexeme += advance();
      token.type = TokenType::OP_GREATER_EQUAL;
    }
    else {
      token.type = TokenType::OP_GREATER;
    }

    return token;
  }

  if (c == '!') {

    token.lexeme += advance();

    if (peek() == '=') {
      token.lexeme += advance();
      token.type = TokenType::OP_NOT_EQUAL;
    }
    else {
      token.type = TokenType::TOKEN_ERROR;
    }

    return token;
  }

  // 5. SIGNOS DE PUNTUACIÓN
  if (c == ',') {
    token.lexeme += advance();
    token.type = TokenType::COMMA;
    return token;
  }

  if (c == ';') {
    token.lexeme += advance();
    token.type = TokenType::SEMICOLON;
    return token;
  }

  if (c == '*') {
    token.lexeme += advance();
    token.type = TokenType::STAR;
    return token;
  }

  // 6. CARÁCTER NO RECONOCIDO
  token.lexeme += advance();
  token.type = TokenType::TOKEN_ERROR;

  return token;
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  Token currentToken = getNextToken();
  
  while (currentToken.type != TokenType::TOKEN_EOF) {
    tokens.push_back(currentToken);
    currentToken = getNextToken();
  }
  
  tokens.push_back(currentToken);
  
  return tokens;
}

