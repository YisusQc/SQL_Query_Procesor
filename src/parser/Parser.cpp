#include "Parser.h"

#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
Parser::Parser(
  const std::vector<Token>& tokens,
  const ParsingTable& parsingTable,
  const Grammar& grammar
)
  : tokens(tokens),
    parsingTable(parsingTable),
    grammar(grammar) {
}

// Representacion de la pila
std::string Parser::stackToString(
  const std::vector<Symbol>& stack
) const {

  std::ostringstream out;
  out << "[ ";

  // La pila se muestra desde el fondo hasta la cima.
  for (const auto& symbol : stack) {
    out << toString(symbol) << " ";
  }

  out << "]";

  return out.str();
}

// Representacion de la entrada restante
std::string Parser::inputToString(size_t index) const {

  std::ostringstream out;
  out << "[ ";

  for (size_t i = index; i < tokens.size(); ++i) {
    out << toString(Symbol::T(tokens[i].type));

    if (!tokens[i].lexeme.empty()) {
      out << "('" << tokens[i].lexeme << "')";
    }
    out << " ";
  }

  out << "]";

  return out.str();
}

// Representacion de una produccion
std::string Parser::productionToString(
  const Production& production
) const {

  std::ostringstream out;

  out << toString(production.lhs) << " -> ";

  if (production.rhs.empty()) {
    out << "epsilon";
    return out.str();
  }

  for (const auto& symbol : production.rhs) {
    out << toString(symbol) << " ";
  }

  return out.str();
}

// Mostrar traza
void Parser::printTrace(
  size_t step,
  const std::vector<Symbol>& stack,
  size_t inputIndex,
  const std::string& action
) const {

  std::cout << std::left
    << std::setw(8) << step
    << std::setw(55) << stackToString(stack)
    << std::setw(65) << inputToString(inputIndex)
    << action
    << "\n";
}

// Detectar errores lexicos
ParseResult Parser::checkLexicalErrors() const {

  for (const Token& token : tokens) {
    if (token.type == TokenType::TOKEN_ERROR) {
      ParseResult result;

      result.accepted = false;
      result.lexicalError = true;
      result.found = token;
      result.message = "Se encontro un token no reconocido.";

      return result;
    }
  }

  ParseResult result;
  result.accepted = true;
  return result;
}

// Crear error sintactico
ParseResult Parser::makeSyntaxError(
  const Token& found,
  const std::vector<TokenType>& expected
) const {

  ParseResult result;

  result.accepted = false;
  result.lexicalError = false;
  result.found = found;
  result.expected = expected;
  result.message = "Error sintactico.";

  return result;
}

// Mostrar error
void Parser::printError(const ParseResult& result) const {

  if (result.lexicalError) {
    std::cout << "\n========== ERROR LEXICO ==========\n";
    std::cout << result.message << "\n";
    std::cout << "Linea: " << result.found.pos.line << ", columna: " << result.found.pos.column << "\n";
    std::cout << "Lexema: '" << result.found.lexeme << "'\n";

    return;
  }

  std::cout << "\n========== ERROR SINTACTICO ==========\n";
  std::cout << result.message << "\n";
  std::cout << "Linea: " << result.found.pos.line << ", columna: " << result.found.pos.column << "\n";
  std::cout << "Token encontrado: " << toString(Symbol::T(result.found.type));

  if (!result.found.lexeme.empty()) {
    std::cout << " ('" << result.found.lexeme << "')";
  }

  std::cout << "\n";
  std::cout << "Tokens esperados: ";

  if (result.expected.empty()) {
    std::cout << "ninguno";
  } else {
    for (size_t i = 0; i < result.expected.size(); ++i) {
      std::cout << toString(Symbol::T(result.expected[i]));

      if (i + 1 < result.expected.size()) {
        std::cout << ", ";
      }
    }
  }

  std::cout << "\n";
}

// Analisis sintactico LL(1)
ParseResult Parser::parse(bool showTrace) const {

  // 1. Comprobar errores lexicos antes del parsing.
  ParseResult lexicalResult = checkLexicalErrors();

  if (!lexicalResult.accepted) {
    printError(lexicalResult);
    return lexicalResult;
  }

  // 2. Verificar que la tabla sea valida.
  if (!parsingTable.isValid()) {

    ParseResult result;
    result.message = "La tabla predictiva contiene conflictos LL(1).";

    std::cout << result.message << "\n";

    return result;
  }

  // 3. Verificar que exista el marcador TOKEN_EOF.
  if (tokens.empty() || tokens.back().type != TokenType::TOKEN_EOF) {

    ParseResult result;

    result.message = "El vector de tokens no contiene TOKEN_EOF al final.";

    std::cout << result.message << "\n";

    return result;
  }

  // 4. Inicializar la pila.
  std::vector<Symbol> stack;

  stack.push_back(Symbol::T(TokenType::TOKEN_EOF));
  stack.push_back(Symbol::N(NonTerminal::QUERY));

  // Indice del token actual.
  size_t inputIndex = 0;

  // Contador de pasos.
  size_t step = 1;

  if (showTrace) {

    std::cout << "\n========== TRAZA DEL ANALISIS LL(1) ==========\n";

    std::cout << std::left
      << std::setw(8) << "PASO"
      << std::setw(55) << "PILA"
      << std::setw(65) << "ENTRADA"
      << "ACCION\n";

    std::cout << std::string(145, '-') << "\n";
  }

  // 5. Ejecutar mientras existan elementos en la pila.
  while (!stack.empty()) {
    // Proteccion ante una entrada incompleta.
    if (inputIndex >= tokens.size()) {
      ParseResult result;
      result.message = "Fin inesperado del vector de tokens.";
      std::cout << "\n" << result.message << "\n";
      return result;
    }

    Symbol top = stack.back();

    const Token& lookahead = tokens[inputIndex];

    // CASO A: la cima es un terminal.

    if (top.kind == SymbolKind::TERMINAL) {

      if (top.terminal == lookahead.type) {

        std::string action = "Coincidir terminal: " + toString(top);

        stack.pop_back();

        // Consumir el token.
        ++inputIndex;

        if (showTrace) {
          printTrace(
            step++, stack, inputIndex, action
          );
        }

        // EOF coincidio: verificar que la pila y la entrada hayan terminado.
        if (top.terminal == TokenType::TOKEN_EOF) {

          if (stack.empty() && inputIndex == tokens.size()) {

            ParseResult result;
            result.accepted = true;

            std::cout
              << "\n========== ANALISIS SINTACTICO ==========\n"
              << "Consulta aceptada correctamente.\n";

            return result;
          }

          ParseResult result;

          result.message = "Se encontraron elementos despues de TOKEN_EOF.";

          std::cout << result.message << "\n";

          return result;
        }

      } else {

        // Error: terminal esperado diferente.
        std::vector<TokenType> expected = {top.terminal};

        ParseResult result = makeSyntaxError(lookahead, expected);

        printError(result);

        return result;
      }
    }

    // CASO B: la cima es un no terminal.

    else {

      NonTerminal nt = top.nonTerminal;

      const Production* production = parsingTable.getProduction(nt, lookahead.type);

      if (production == nullptr) {

        // No existe produccion para ese lookahead.
        auto expectedSet = parsingTable.getExpectedTokens(nt);

        std::vector<TokenType> expected(expectedSet.begin(), expectedSet.end());

        ParseResult result = makeSyntaxError(lookahead, expected);

        printError(result);

        return result;
      }

      // Guardar la accion antes de modificar la pila.
      std::string action = "Aplicar P" + std::to_string(production->id) + ": " + productionToString(*production);

      // Retirar el no terminal.
      stack.pop_back();

      // Insertar RHS en orden inverso.
      for (auto it = production->rhs.rbegin(); it != production->rhs.rend(); ++it) {
        stack.push_back(*it);
      }

      if (showTrace) {
        printTrace(
          step++, stack, inputIndex, action
        );
      }
    }
  }

  // Si la pila termina sin consumir EOF, hubo un problema.
  ParseResult result;

  result.message = "La pila termino antes de consumir toda la entrada.";

  std::cout << result.message << "\n";

  return result;
}

