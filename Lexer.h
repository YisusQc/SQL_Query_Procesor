#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include "Keywords.h"
#include <string>
#include <vector>

class Lexer {
private:
  std::string input;
  int currentPos;
  int currentLine;
  int currentCol;
  Keywords keywords;

  char peek();
  char advance();
  void skipWhitespace();

public:
  Lexer(const std::string& source) : input(source), currentPos(0), currentLine(1), currentCol(1) {}
    
  std::vector<Token> tokenize();
  
  Token getNextToken(); 
};

#endif

