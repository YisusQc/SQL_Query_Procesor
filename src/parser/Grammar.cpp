#include "Grammar.h"

// CONSTRUCTOR: DEFINICION DE LA GRAMATICA
Grammar::Grammar() {
  productions = {
    // 1. CONSULTA PRINCIPAL
    {
      1,
      NonTerminal::QUERY,
      {
        Symbol::N(NonTerminal::SELECT_CLAUSE),
        Symbol::N(NonTerminal::FROM_CLAUSE),
        Symbol::N(NonTerminal::WHERE_OPT),
        Symbol::T(TokenType::SEMICOLON)
      }
    },
    // 2. SELECT
    {
      2,
      NonTerminal::SELECT_CLAUSE,
      {
        Symbol::T(TokenType::KEYWORD_SELECT),
        Symbol::N(NonTerminal::PROJECTION)
      }
    },
    // 3. PROJECTION
    {
      3,
      NonTerminal::PROJECTION,
      {
        Symbol::T(TokenType::STAR)
      }
    },

    {
      4,
      NonTerminal::PROJECTION,
      {
        Symbol::N(NonTerminal::COLUMN_LIST)
      }
    },
    // 4. LISTA DE COLUMNAS
    {
      5,
      NonTerminal::COLUMN_LIST,
      {
        Symbol::T(TokenType::IDENTIFIER),
        Symbol::N(NonTerminal::COLUMN_TAIL)
      }
    },

    {
      6,
      NonTerminal::COLUMN_TAIL,
      {
        Symbol::T(TokenType::COMMA),
        Symbol::T(TokenType::IDENTIFIER),
        Symbol::N(NonTerminal::COLUMN_TAIL)
      }
    },

    {
      7,
      NonTerminal::COLUMN_TAIL,
      {} // epsilon
    },
    // 5. FROM
    {
      8,
      NonTerminal::FROM_CLAUSE,
      {
        Symbol::T(TokenType::KEYWORD_FROM),
        Symbol::T(TokenType::IDENTIFIER)
      }
    },
    // 6. WHERE OPCIONAL
    {
      9,
      NonTerminal::WHERE_OPT,
      {
        Symbol::T(TokenType::KEYWORD_WHERE),
        Symbol::N(NonTerminal::EXPRESSION)
      }
    },

    {
      10,
      NonTerminal::WHERE_OPT,
      {} // epsilon
    },
    // 7. EXPRESIONES LOGICAS
    {
      11,
      NonTerminal::EXPRESSION,
      {
        Symbol::N(NonTerminal::AND_EXPRESSION),
        Symbol::N(NonTerminal::OR_TAIL)
      }
    },

    {
      12,
      NonTerminal::OR_TAIL,
      {
        Symbol::T(TokenType::KEYWORD_OR),
        Symbol::N(NonTerminal::AND_EXPRESSION),
        Symbol::N(NonTerminal::OR_TAIL)
      }
    },

    {
      13,
      NonTerminal::OR_TAIL,
      {} // epsilon
    },

    {
      14,
      NonTerminal::AND_EXPRESSION,
      {
        Symbol::N(NonTerminal::CONDITION),
        Symbol::N(NonTerminal::AND_TAIL)
      }
    },

    {
      15,
      NonTerminal::AND_TAIL,
      {
        Symbol::T(TokenType::KEYWORD_AND),
        Symbol::N(NonTerminal::CONDITION),
        Symbol::N(NonTerminal::AND_TAIL)
      }
    },

    {
      16,
      NonTerminal::AND_TAIL,
      {} // epsilon
    },
    // 8. CONDICIONES
    {
      17,
      NonTerminal::CONDITION,
      {
        Symbol::T(TokenType::IDENTIFIER),
        Symbol::N(NonTerminal::RELATIONAL_OPERATOR),
        Symbol::N(NonTerminal::VALUE)
      }
    },
    // 9. OPERADORES RELACIONALES
    {
      18,
      NonTerminal::RELATIONAL_OPERATOR,
      {
        Symbol::T(TokenType::OP_EQUAL)
      }
    },

    {
      19,
      NonTerminal::RELATIONAL_OPERATOR,
      {
        Symbol::T(TokenType::OP_NOT_EQUAL)
      }
    },

    {
      20,
      NonTerminal::RELATIONAL_OPERATOR,
      {
        Symbol::T(TokenType::OP_LESS)
      }
    },

    {
      21,
      NonTerminal::RELATIONAL_OPERATOR,
      {
        Symbol::T(TokenType::OP_GREATER)
      }
    },

    {
      22,
      NonTerminal::RELATIONAL_OPERATOR,
      {
        Symbol::T(TokenType::OP_LESS_EQUAL)
      }
    },

    {
      23,
      NonTerminal::RELATIONAL_OPERATOR,
      {
        Symbol::T(TokenType::OP_GREATER_EQUAL)
      }
    },
    // 10. VALORES
    {
      24,
      NonTerminal::VALUE,
      {
        Symbol::T(TokenType::IDENTIFIER)
      }
    },

    {
      25,
      NonTerminal::VALUE,
      {
        Symbol::T(TokenType::INTEGER_LITERAL)
      }
    },

    {
      26,
      NonTerminal::VALUE,
      {
        Symbol::T(TokenType::DECIMAL_LITERAL)
      }
    },

    {
      27,
      NonTerminal::VALUE,
      {
        Symbol::T(TokenType::STRING_LITERAL)
      }
    }
  };
}

// OBTENER TODAS LAS PRODUCCIONES
const std::vector<Production>& Grammar::getProductions() const {
  return productions;
}

// BUSCAR UNA PRODUCCION POR ID
const Production* Grammar::getProductionById(int id) const {
  for (const auto& production : productions) {
    if (production.id == id) {
      return &production;
    }
  }

  return nullptr;
}

// CONVERTIR NO TERMINAL A TEXTO
std::string toString(NonTerminal nt) {
  switch (nt) {
    case NonTerminal::QUERY:
      return "QUERY";

    case NonTerminal::SELECT_CLAUSE:
      return "SELECT_CLAUSE";

    case NonTerminal::PROJECTION:
      return "PROJECTION";

    case NonTerminal::COLUMN_LIST:
      return "COLUMN_LIST";

    case NonTerminal::COLUMN_TAIL:
      return "COLUMN_TAIL";

    case NonTerminal::FROM_CLAUSE:
      return "FROM_CLAUSE";

    case NonTerminal::WHERE_OPT:
      return "WHERE_OPT";

    case NonTerminal::EXPRESSION:
      return "EXPRESSION";

    case NonTerminal::OR_TAIL:
      return "OR_TAIL";

    case NonTerminal::AND_EXPRESSION:
      return "AND_EXPRESSION";

    case NonTerminal::AND_TAIL:
      return "AND_TAIL";

    case NonTerminal::CONDITION:
      return "CONDITION";

    case NonTerminal::RELATIONAL_OPERATOR:
      return "RELATIONAL_OPERATOR";

    case NonTerminal::VALUE:
      return "VALUE";
  }

  return "UNKNOWN_NON_TERMINAL";
}

// CONVERTIR SIMBOLO A TEXTO
std::string toString(const Symbol& symbol) {
  if (symbol.kind == SymbolKind::NON_TERMINAL) {
    return toString(symbol.nonTerminal);
  }

  switch (symbol.terminal) {
    case TokenType::KEYWORD_SELECT:
      return "KEYWORD_SELECT";

    case TokenType::KEYWORD_FROM:
      return "KEYWORD_FROM";

    case TokenType::KEYWORD_WHERE:
      return "KEYWORD_WHERE";

    case TokenType::KEYWORD_AND:
      return "KEYWORD_AND";

    case TokenType::KEYWORD_OR:
      return "KEYWORD_OR";

    case TokenType::IDENTIFIER:
      return "IDENTIFIER";

    case TokenType::INTEGER_LITERAL:
      return "INTEGER_LITERAL";

    case TokenType::DECIMAL_LITERAL:
      return "DECIMAL_LITERAL";

    case TokenType::STRING_LITERAL:
      return "STRING_LITERAL";

    case TokenType::OP_EQUAL:
      return "OP_EQUAL";

    case TokenType::OP_NOT_EQUAL:
      return "OP_NOT_EQUAL";

    case TokenType::OP_LESS:
      return "OP_LESS";

    case TokenType::OP_GREATER:
      return "OP_GREATER";

    case TokenType::OP_LESS_EQUAL:
      return "OP_LESS_EQUAL";

    case TokenType::OP_GREATER_EQUAL:
      return "OP_GREATER_EQUAL";

    case TokenType::COMMA:
      return "COMMA";

    case TokenType::SEMICOLON:
      return "SEMICOLON";

    case TokenType::STAR:
      return "STAR";

    case TokenType::TOKEN_EOF:
      return "TOKEN_EOF";

    case TokenType::TOKEN_ERROR:
      return "TOKEN_ERROR";
  }

  return "UNKNOWN_TERMINAL";
}

