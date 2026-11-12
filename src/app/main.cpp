#include "../lexer/Lexer.h"
#include "../parser/Grammar.h"
#include "../parser/FirstFollow.h"
#include "../parser/ParsingTable.h"
#include "../parser/Parser.h"

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

int main(int argc, char* argv[]) {

  std::string query;

  // 1. Obtener la consulta

  if (argc > 1) {

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
      std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";

      return 1;
    }

    query.assign(
      std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>()
    );

  } else {

    std::cout << "Ingrese una consulta SQL:\n";

    query.assign(
      std::istreambuf_iterator<char>(std::cin),
      std::istreambuf_iterator<char>()
    );
  }

  if (query.empty()) {
    std::cerr << "No se proporcionó ninguna consulta.\n";
    return 1;
  }

  // 2. Analisis lexico

  Lexer lexer(query);
  auto tokens = lexer.tokenize();

  // 3. Preparar la gramatica LL(1)

  Grammar grammar;

  FirstFollow firstFollow(grammar);

  if (!firstFollow.isLL1()) {
    std::cerr << "Error: la gramática presenta conflictos LL(1).\n";
    return 1;
  }

  // 4. Construir la tabla predictiva

  ParsingTable parsingTable(grammar, firstFollow);

  if (!parsingTable.isValid()) {
    std::cerr << "Error: la tabla predictiva no es válida.\n";
    return 1;
  }

  // 5. Analisis sintáctico

  Parser parser(tokens, parsingTable, grammar);

  ParseResult result = parser.parse(true, true);

  // 6. Resultado

  if (result.accepted) {
    std::cout << "\nConsulta SQL sintácticamente válida.\n";
    return 0;
  }

  std::cout << "\nConsulta SQL rechazada.\n";

  return 1;
}

