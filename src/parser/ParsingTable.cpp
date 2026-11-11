#include "ParsingTable.h"

#include <iostream>
#include <iomanip>

ParsingTable::ParsingTable(
  const Grammar& grammar,
  const FirstFollow& firstFollow
)
  : grammar(grammar), firstFollow(firstFollow), valid(true) {

  initializeTable();
  buildTable();
}

// Inicializacion
void ParsingTable::initializeTable() {

  // Crear una fila para cada no terminal.
  for (int i = 0; i < 14; ++i) {
    NonTerminal nt = static_cast<NonTerminal>(i);
    table[nt];
  }

  // Recopilar los terminales de las producciones.
  for (const auto& production : grammar.getProductions()) {
    for (const auto& symbol : production.rhs) {
      if (symbol.kind == SymbolKind::TERMINAL) {
        terminals.insert(symbol.terminal);
      }
    }
  }

  // Agregar los terminales presentes en FOLLOW.
  for (int i = 0; i < 14; ++i) {
    NonTerminal nt = static_cast<NonTerminal>(i);
    const auto& follow = firstFollow.getFollow(nt);
    terminals.insert(follow.begin(), follow.end());
  }
}

// Construccion de la tabla
void ParsingTable::buildTable() {

  for (const auto& production : grammar.getProductions()) {

    NonTerminal lhs = production.lhs;

    // FIRST del lado derecho.
    SequenceFirst sequenceFirst =
      firstFollow.firstOfSequence(production.rhs);

    // Insertar la produccion para cada terminal de FIRST.
    for (TokenType terminal : sequenceFirst.terminals) {

      auto& row = table[lhs];

      auto result = row.emplace(terminal, production.id);

      // Una celda ocupada por otra produccion representa un conflicto.
      if (!result.second && result.first->second != production.id) {
        valid = false;

        std::cerr << "Conflicto en M[" << toString(lhs) << ", " << toString(Symbol::T(terminal))
          << "] entre producciones " << result.first->second << " y " << production.id << "\n";
      }
    }

    // Si la produccion genera epsilon, insertar en las columnas de FOLLOW(lhs).
    if (sequenceFirst.derivesEpsilon) {

      const auto& follow = firstFollow.getFollow(lhs);

      for (TokenType terminal : follow) {

        auto& row = table[lhs];

        auto result = row.emplace(terminal, production.id);

        if (!result.second && result.first->second != production.id) {
          valid = false;

          std::cerr << "Conflicto en M[" << toString(lhs) << ", " << toString(Symbol::T(terminal))
            << "] entre producciones " << result.first->second << " y " << production.id << "\n";
        }
      }
    }
  }
}

// Consultas
const Production* ParsingTable::getProduction(
  NonTerminal nt,
  TokenType lookahead
) const {
  auto row = table.find(nt);

  if (row == table.end()) {
    return nullptr;
  }

  auto cell = row->second.find(lookahead);

  if (cell == row->second.end()) {
    return nullptr;
  }

  return grammar.getProductionById(cell->second);
}

bool ParsingTable::hasEntry(
  NonTerminal nt,
  TokenType lookahead
) const {
  return getProduction(nt, lookahead) != nullptr;
}

const std::set<TokenType>& ParsingTable::getTerminals() const {
  return terminals;
}

// Impresion
void ParsingTable::printTable() const {

  std::cout << "\n========== TABLA PREDICTIVA LL(1) ==========\n";

  constexpr int width = 19;

  // Encabezado.
  std::cout << std::left << std::setw(width) << "NO TERMINAL";

  for (TokenType terminal : terminals) {
    std::cout << std::setw(width) << toString(Symbol::T(terminal));
  }

  std::cout << "\n";

  // Separador.
  std::cout << std::string(width * (terminals.size() + 1), '-') << "\n";

  // Filas.
  for (const auto& [nt, row] : table) {
    std::cout << std::left << std::setw(width) << toString(nt);

    for (TokenType terminal : terminals) {
      auto cell = row.find(terminal);

      if (cell != row.end()) {
        std::cout << std::setw(width) << ("P" + std::to_string(cell->second));
      } else {
        std::cout << std::setw(width) << "-";
      }
    }

    std::cout << "\n";
  }
}

// Validacion
bool ParsingTable::isValid() const {
  return valid;
}

