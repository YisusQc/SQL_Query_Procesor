#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include "Grammar.h"
#include "FirstFollow.h"

#include <map>
#include <set>
#include <vector>
#include <string>

class ParsingTable {
private:
  const Grammar& grammar;
  const FirstFollow& firstFollow;

  // M[NoTerminal][Lookahead] = ID de produccion
  std::map<NonTerminal, std::map<TokenType, int>> table;

  // Terminales utilizados como columnas.
  std::set<TokenType> terminals;

  // Registrar si hubo conflicto
  bool valid = true;

  // Construccion interna.
  void initializeTable();
  void buildTable();

public:
  ParsingTable(
    const Grammar& grammar,
    const FirstFollow& firstFollow
  );

  // Consultar una celda. Devuelve nullptr si no existe una produccion.
  const Production* getProduction(
    NonTerminal nt,
    TokenType lookahead
  ) const;

  // Verificar si una celda tiene produccion.
  bool hasEntry(
    NonTerminal nt,
    TokenType lookahead
  ) const;

  // Obtener las columnas de la tabla.
  const std::set<TokenType>& getTerminals() const;

  // Mostrar la tabla.
  void printTable() const;

  // Verificar que todas las producciones aplicables hayan sido insertadas sin conflictos.
  bool isValid() const;

  // Consultar los tokens esperados
  std::set<TokenType> getExpectedTokens(NonTerminal nt) const;
};

#endif

