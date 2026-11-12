#include "TestSupport.h"
#include "../src/parser/Grammar.h"
#include "../src/parser/FirstFollow.h"
#include "../src/parser/ParsingTable.h"

using namespace TestSupport;

int main() {

  Grammar grammar;

  std::cout << "\nProducciones de la gramatica LL(1):\n";

  for (const auto& production : grammar.getProductions()) {
    std::cout << "(" << production.id << ") "
              << toString(production.lhs)
              << " -> ";

    if (production.rhs.empty()) {
      std::cout << "epsilon";
    } else {
      for (const auto& symbol : production.rhs) {
        std::cout << toString(symbol) << " ";
      }
    }

    std::cout << "\n";
  }

  FirstFollow firstFollow(grammar);

  firstFollow.printFirstSets();
  firstFollow.printFollowSets();
  firstFollow.printLL1Conflicts();

  std::cout << "\n\n========== PRUEBAS DE GRAMATICA ==========\n";

  check(
    grammar.getProductions().size() == 27,
    "La gramatica contiene 27 producciones"
  );

  check(
    firstFollow.getFirst(NonTerminal::QUERY).count(
      TokenType::KEYWORD_SELECT
    ) == 1,
    "FIRST(QUERY) contiene KEYWORD_SELECT"
  );

  check(
    firstFollow.isNullable(NonTerminal::WHERE_OPT),
    "WHERE_OPT puede producir epsilon"
  );

  check(
    firstFollow.getFollow(NonTerminal::QUERY).count(
      TokenType::TOKEN_EOF
    ) == 1,
    "FOLLOW(QUERY) contiene TOKEN_EOF"
  );

  check(
    firstFollow.getFollow(NonTerminal::AND_TAIL).count(
      TokenType::KEYWORD_OR
    ) == 1,
    "FOLLOW(AND_TAIL) contiene KEYWORD_OR"
  );

  check(
    firstFollow.isLL1(),
    "La gramatica no presenta conflictos LL(1)"
  );

  ParsingTable table(grammar, firstFollow);

  check(
    table.isValid(),
    "La tabla predictiva se construye sin conflictos"
  );

  check(
    table.getProduction(
      NonTerminal::QUERY,
      TokenType::KEYWORD_SELECT
    ) != nullptr,
    "M[QUERY, KEYWORD_SELECT] tiene produccion"
  );

  check(
    table.getProduction(
      NonTerminal::WHERE_OPT,
      TokenType::SEMICOLON
    ) != nullptr,
    "M[WHERE_OPT, SEMICOLON] tiene produccion epsilon"
  );

  summary();

  return failed == 0 ? 0 : 1;
}

