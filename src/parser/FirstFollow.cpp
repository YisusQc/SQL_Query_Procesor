#include "FirstFollow.h"

#include <iostream>
#include <algorithm>

FirstFollow::FirstFollow(const Grammar& grammar)
  : grammar(grammar) {

  initializeSets();
  computeFirst();
  computeFollow();
}

// Inicializacion
void FirstFollow::initializeSets() {

  for (const auto& production : grammar.getProductions()) {
    firstSets[production.lhs];
    followSets[production.lhs];
  }

  // Todos los no terminales de la gramatica.
  for (int i = 0; i < 14; ++i) {
    NonTerminal nt = static_cast<NonTerminal>(i);
    firstSets[nt];
    followSets[nt];
  }
}

// FIRST de una secuencia de simbolos
SequenceFirst FirstFollow::firstOfSequence(
  const std::vector<Symbol>& sequence
) const {

  SequenceFirst result;

  // Una secuencia vacia produce epsilon.
  if (sequence.empty()) {
    result.derivesEpsilon = true;
    return result;
  }

  bool allNullable = true;

  for (const auto& symbol : sequence) {
    // Si es terminal, se agrega y se detiene.
    if (symbol.kind == SymbolKind::TERMINAL) {
      result.terminals.insert(symbol.terminal);
      allNullable = false;
      break;
    }

    // Si es no terminal, agregamos su FIRST.
    auto it = firstSets.find(symbol.nonTerminal);

    if (it != firstSets.end()) {
      result.terminals.insert(it->second.begin(), it->second.end());
    }

    // Si no produce epsilon, no continuamos.
    if (!isNullable(symbol.nonTerminal)) {
      allNullable = false;
      break;
    }
  }

  result.derivesEpsilon = allNullable;
  return result;
}

// Cálculo de FIRST
void FirstFollow::computeFirst() {

  bool changed;

  do {
    changed = false;

    for (const auto& production : grammar.getProductions()) {

      SequenceFirst sequenceFirst = firstOfSequence(production.rhs);

      auto& currentFirst = firstSets[production.lhs];

      // Agregar los terminales encontrados.
      for (TokenType terminal : sequenceFirst.terminals) {
        if (currentFirst.insert(terminal).second) {
          changed = true;
        }
      }

      // Si toda la secuencia produce epsilon.
      if (sequenceFirst.derivesEpsilon) {
        if (nullable.insert(production.lhs).second) {
          changed = true;
        }
      }
    }

  } while (changed);
}

// Calculo de FOLLOW
void FirstFollow::computeFollow() {

  // QUERY es el simbolo inicial.
  followSets[NonTerminal::QUERY].insert(TokenType::TOKEN_EOF);

  bool changed;

  do {
    changed = false;

    for (const auto& production : grammar.getProductions()) {

      const auto& rhs = production.rhs;

      for (size_t i = 0; i < rhs.size(); ++i) {

        const Symbol& current = rhs[i];

        if (current.kind != SymbolKind::NON_TERMINAL) {
          continue;
        }

        // Obtener la secuencia que viene despues.
        std::vector<Symbol> beta(rhs.begin() + i + 1, rhs.end());

        SequenceFirst betaFirst = firstOfSequence(beta);

        auto& currentFollow = followSets[current.nonTerminal];

        // Agregar FIRST(beta), sin epsilon.
        for (TokenType terminal : betaFirst.terminals) {
          if (currentFollow.insert(terminal).second) {
            changed = true;
          }
        }

        // Si beta produce epsilon, agregar FOLLOW(lhs).
        if (betaFirst.derivesEpsilon) {

          const auto& lhsFollow = followSets[production.lhs];

          for (TokenType terminal : lhsFollow) {
            if (currentFollow.insert(terminal).second) {
              changed = true;
            }
          }
        }
      }
    }

  } while (changed);
}

// Consultas
const TerminalSet& FirstFollow::getFirst(NonTerminal nt) const {
  return firstSets.at(nt);
}

const TerminalSet& FirstFollow::getFollow(NonTerminal nt) const {
  return followSets.at(nt);
}

bool FirstFollow::isNullable(NonTerminal nt) const {
  return nullable.count(nt) > 0;
}

// Deteccion de conflictos LL(1)
std::vector<LL1Conflict> FirstFollow::findLL1Conflicts() const {

  std::vector<LL1Conflict> conflicts;

  const auto& productions = grammar.getProductions();

  for (size_t i = 0; i < productions.size(); ++i) {

    for (size_t j = i + 1; j < productions.size(); ++j) {

      const auto& p1 = productions[i];
      const auto& p2 = productions[j];

      // Solo comparamos alternativas del mismo no terminal.
      if (p1.lhs != p2.lhs) {
        continue;
      }

      SequenceFirst first1 = firstOfSequence(p1.rhs);
      SequenceFirst first2 = firstOfSequence(p2.rhs);

      // Interseccion FIRST(p1) y FIRST(p2).
      for (TokenType terminal : first1.terminals) {
        if (first2.terminals.count(terminal)) {
          conflicts.push_back({p1.lhs, terminal, p1.id, p2.id});
        }
      }

      // Si p1 produce epsilon, FIRST(p2) debe ser disjunto de FOLLOW(lhs).
      if (first1.derivesEpsilon) {
        for (TokenType terminal : first2.terminals) {
          if (followSets.at(p1.lhs).count(terminal)) {
            conflicts.push_back({
              p1.lhs, terminal, p1.id, p2.id
            });
          }
        }
      }

      // Si p2 produce epsilon, FIRST(p1) debe ser disjunto de FOLLOW(lhs).
      if (first2.derivesEpsilon) {
        for (TokenType terminal : first1.terminals) {
          if (followSets.at(p2.lhs).count(terminal)) {
            conflicts.push_back({p1.lhs, terminal, p1.id, p2.id});
          }
        }
      }

      // Si ambas producen epsilon, tambien existe conflicto si FOLLOW(lhs) no es vacio.
      if (first1.derivesEpsilon && first2.derivesEpsilon) {
        for (TokenType terminal : followSets.at(p1.lhs)) {
          conflicts.push_back({
            p1.lhs, terminal, p1.id, p2.id
          });
        }
      }
    }
  }

  return conflicts;
}

bool FirstFollow::isLL1() const {
  return findLL1Conflicts().empty();
}

// Impresion de conjuntos
void FirstFollow::printFirstSets() const {

  std::cout << "\n========== CONJUNTOS FIRST ==========\n";

  for (const auto& [nt, terminals] : firstSets) {

    std::cout << "FIRST(" << toString(nt) << ") = { ";

    for (TokenType terminal : terminals) {
      std::cout << toString(Symbol::T(terminal)) << " ";
    }

    if (isNullable(nt)) {
      std::cout << "epsilon ";
    }

    std::cout << "}\n";
  }
}

void FirstFollow::printFollowSets() const {

  std::cout << "\n========== CONJUNTOS FOLLOW ==========\n";

  for (const auto& [nt, terminals] : followSets) {

    std::cout << "FOLLOW(" << toString(nt) << ") = { ";

    for (TokenType terminal : terminals) {
      std::cout << toString(Symbol::T(terminal)) << " ";
    }

    std::cout << "}\n";
  }
}

void FirstFollow::printLL1Conflicts() const {

  auto conflicts = findLL1Conflicts();

  std::cout << "\n========== VALIDACION LL(1) ==========\n";

  if (conflicts.empty()) {
    std::cout << "La gramatica NO presenta conflictos LL(1).\n";
    std::cout << "La gramatica es LL(1).\n";
    return;
  }

  std::cout << "Se encontraron " << conflicts.size() << " conflictos:\n";

  for (const auto& conflict : conflicts) {

    std::cout << "Conflicto en " << toString(conflict.nonTerminal)
              << " con lookahead " << toString(Symbol::T(conflict.lookahead))
              << " entre producciones " << conflict.production1
              << " y " << conflict.production2 << "\n";
  }
}

