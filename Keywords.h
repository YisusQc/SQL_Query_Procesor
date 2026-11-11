#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "Token.h"
#include <unordered_map>
#include <string>
#include <algorithm>

class Keywords {
private:
  std::unordered_map<std::string, TokenType> reservedWords;

  std::string toUpper(const std::string& word) {
    std::string result = word;

    std::transform(
      result.begin(),
      result.end(),
      result.begin(),
      [](unsigned char c) {
        return std::toupper(c);
      }
    );

    return result;
  }
public:
  Keywords() {
    reservedWords["SELECT"] = TokenType::KEYWORD_SELECT;
    reservedWords["FROM"] = TokenType::KEYWORD_FROM;
    reservedWords["WHERE"] = TokenType::KEYWORD_WHERE;
    reservedWords["AND"] = TokenType::KEYWORD_AND;
    reservedWords["OR"] = TokenType::KEYWORD_OR;
  }
  
  bool isKeyword(const std::string& lexeme, TokenType& outType) {
    std::string upperLexeme = toUpper(lexeme);
    auto it = reservedWords.find(upperLexeme);
    if (it != reservedWords.end()) {
      outType = it->second;
      return true;
    }
    return false;
  }
};

#endif

