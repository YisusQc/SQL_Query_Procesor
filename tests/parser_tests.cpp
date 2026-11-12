#include "TestSupport.h"

#include "../src/lexer/Lexer.h"
#include "../src/parser/Grammar.h"
#include "../src/parser/FirstFollow.h"
#include "../src/parser/ParsingTable.h"
#include "../src/parser/Parser.h"

#include <string>

using namespace TestSupport;

bool parseQuery(
  const std::string& query,
  const Grammar& grammar,
  const ParsingTable& table
) {
  Lexer lexer(query);
  auto tokens = lexer.tokenize();

  Parser parser(tokens, table, grammar);

  // Sin traza durante las pruebas automatizadas.
  ParseResult result = parser.parse(false, false);

  return result.accepted;
}

int main() {
  std::cout << "\n========== PRUEBAS DEL PARSER ==========\n";

  Grammar grammar;
  FirstFollow firstFollow(grammar);
  ParsingTable table(grammar, firstFollow);

  // Consultas validas

  check(
    parseQuery(
      "SELECT * FROM estudiantes;",
      grammar, table
    ),
    "Acepta SELECT con proyeccion STAR"
  );

  check(
    parseQuery(
      "SELECT nombre FROM estudiantes;",
      grammar, table
    ),
    "Acepta una columna"
  );

  check(
    parseQuery(
      "SELECT nombre, edad FROM estudiantes;",
      grammar, table
    ),
    "Acepta multiples columnas"
  );

  check(
    parseQuery(
      "SELECT nombre FROM estudiantes WHERE edad = 18;",
      grammar, table
    ),
    "Acepta una condicion simple"
  );

  check(
    parseQuery(
      "SELECT nombre FROM estudiantes WHERE edad > 18 AND edad < 30;",
      grammar, table
    ),
    "Acepta condiciones AND"
  );

  check(
    parseQuery(
      "SELECT nombre FROM estudiantes WHERE edad < 18 OR edad > 60;",
      grammar, table
    ),
    "Acepta condiciones OR"
  );

  check(
    parseQuery(
      "SELECT nombre FROM estudiantes WHERE edad >= 18 AND carrera = 'Sistemas' OR edad < 15;",
      grammar, table
    ),
    "Acepta expresiones combinadas AND y OR"
  );

  check(
    parseQuery(
      "select nombre from estudiantes;",
      grammar, table
    ),
    "Acepta keywords en minusculas"
  );

  // Consultas invalidas

  check(
    !parseQuery(
      "SELECT FROM estudiantes;",
      grammar, table
    ),
    "Rechaza una proyeccion vacia"
  );

  check(
    !parseQuery(
      "SELECT nombre estudiantes;",
      grammar, table
    ),
    "Rechaza la ausencia de FROM"
  );

  check(
    !parseQuery(
      "SELECT nombre FROM;",
      grammar, table
    ),
    "Rechaza la ausencia de tabla"
  );

  check(
    !parseQuery(
      "SELECT nombre FROM estudiantes WHERE;",
      grammar, table
    ),
    "Rechaza WHERE sin expresion"
  );

  check(
    !parseQuery(
      "SELECT nombre FROM estudiantes WHERE edad >;",
      grammar, table
    ),
    "Rechaza una condicion sin valor"
  );

  check(
    !parseQuery(
      "SELECT nombre, FROM estudiantes;",
      grammar, table
    ),
    "Rechaza una lista de columnas incompleta"
  );

  check(
    !parseQuery(
      "SELECT nombre FROM estudiantes",
      grammar, table
    ),
    "Rechaza la ausencia de punto y coma"
  );

  check(
    !parseQuery(
      "SELECT nombre FROM estudiantes;;",
      grammar, table
    ),
    "Rechaza tokens sobrantes"
  );

  // Errores lexicos

  check(
    !parseQuery(
      "SELECT @ FROM estudiantes;",
      grammar, table
    ),
    "Rechaza consultas con errores lexicos"
  );

  summary();

  return failed == 0 ? 0 : 1;
}

