#ifndef PARSER_H
#define PARSER_H

#include "../common/Token.h"
#include "Grammar.h"
#include "ParsingTable.h"

#include <vector>
#include <string>

struct ParseResult {
  bool accepted = false;
  bool lexicalError = false;

  Token found{};
  std::vector<TokenType> expected;

  std::string message;
};

class Parser {
private:
  const std::vector<Token>& tokens;
  const ParsingTable& parsingTable;
  const Grammar& grammar;

  // Representacion textual de la pila y entrada.
  std::string stackToString(
    const std::vector<Symbol>& stack
  ) const;

  std::string inputToString(size_t index) const;

  std::string productionToString(
    const Production& production
  ) const;

  // Informacion de errores.
  ParseResult makeSyntaxError(
    const Token& found,
    const std::vector<TokenType>& expected
  ) const;

  ParseResult checkLexicalErrors() const;

  void printTrace(
    size_t step,
    const std::vector<Symbol>& stack,
    size_t inputIndex,
    const std::string& action
  ) const;

  void printError(const ParseResult& result) const;

public:
  Parser(
    const std::vector<Token>& tokens,
    const ParsingTable& parsingTable,
    const Grammar& grammar
  );

  ParseResult parse(bool showTrace = true, bool showDiagnostics = true) const;
};

#endif

