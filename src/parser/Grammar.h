#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "../common/Token.h"
#include <vector>
#include <string>

// 1. NO TERMINALES
enum class NonTerminal {
  QUERY,
  SELECT_CLAUSE,
  PROJECTION,
  COLUMN_LIST,
  COLUMN_TAIL,
  FROM_CLAUSE,
  WHERE_OPT,
  EXPRESSION,
  OR_TAIL,
  AND_EXPRESSION,
  AND_TAIL,
  CONDITION,
  RELATIONAL_OPERATOR,
  VALUE
};

// 2. TIPO DE SIMBOLO GRAMATICAL
enum class SymbolKind {
  TERMINAL,
  NON_TERMINAL
};

// 3. REPRESENTACION DE UN SIMBOLO
struct Symbol {
  SymbolKind kind;
  TokenType terminal = TokenType::TOKEN_ERROR;
  NonTerminal nonTerminal = NonTerminal::QUERY;

  static Symbol T(TokenType type) {
    return {
      SymbolKind::TERMINAL,
      type,
      NonTerminal::QUERY
    };
  }

  static Symbol N(NonTerminal type) {
    return {
      SymbolKind::NON_TERMINAL,
      TokenType::TOKEN_ERROR,
      type
    };
  }
};

// 4. REPRESENTACION DE UNA PRODUCCION
struct Production {
  int id;
  NonTerminal lhs;
  std::vector<Symbol> rhs;
};

// 5. CLASE GRAMMAR
class Grammar {
private:
  std::vector<Production> productions;

public:
  Grammar();

  const std::vector<Production>& getProductions() const;

  const Production* getProductionById(int id) const;
};

// 6. FUNCIONES AUXILIARES
std::string toString(NonTerminal nt);

std::string toString(const Symbol& symbol);

#endif

