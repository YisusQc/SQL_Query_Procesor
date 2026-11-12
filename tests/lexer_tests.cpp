#include "TestSupport.h"
#include "../src/lexer/Lexer.h"

#include <string>
#include <vector>

using namespace TestSupport;

void testValidQuery() {
  Lexer lexer("SELECT nombre FROM estudiantes;");
  auto tokens = lexer.tokenize();

  check(
    tokens.size() == 6,
    "Lexer reconoce una consulta SELECT sencilla"
  );

  check(
    tokens[0].type == TokenType::KEYWORD_SELECT,
    "Reconoce SELECT"
  );

  check(
    tokens[1].type == TokenType::IDENTIFIER,
    "Reconoce nombre como IDENTIFIER"
  );

  check(
    tokens[2].type == TokenType::KEYWORD_FROM,
    "Reconoce FROM"
  );

  check(
    tokens[3].type == TokenType::IDENTIFIER,
    "Reconoce el nombre de la tabla"
  );

  check(
    tokens[4].type == TokenType::SEMICOLON,
    "Reconoce punto y coma"
  );

  check(
    tokens[5].type == TokenType::TOKEN_EOF,
    "Agrega TOKEN_EOF al final"
  );
}

void testCaseInsensitiveKeywords() {
  Lexer lexer("select nombre from estudiantes;");
  auto tokens = lexer.tokenize();

  check(
    tokens[0].type == TokenType::KEYWORD_SELECT,
    "Reconoce SELECT en minusculas"
  );

  check(
    tokens[2].type == TokenType::KEYWORD_FROM,
    "Reconoce FROM en minusculas"
  );
}

void testOperators() {
  Lexer lexer("SELECT edad FROM estudiantes WHERE edad >= 18;");
  auto tokens = lexer.tokenize();

  bool foundGreaterEqual = false;

  for (const auto& token : tokens) {
    if (token.type == TokenType::OP_GREATER_EQUAL) {
      foundGreaterEqual = true;
      break;
    }
  }

  check(
    foundGreaterEqual,
    "Reconoce el operador >="
  );
}

void testStringLiteral() {
  Lexer lexer("SELECT nombre FROM estudiantes WHERE carrera = 'Sistemas';");
  auto tokens = lexer.tokenize();

  bool foundString = false;

  for (const auto& token : tokens) {
    if (token.type == TokenType::STRING_LITERAL) {
      foundString = true;
      break;
    }
  }

  check(
    foundString,
    "Reconoce literales de cadena"
  );
}

void testInvalidCharacter() {
  Lexer lexer("SELECT @ FROM estudiantes;");
  auto tokens = lexer.tokenize();

  bool foundError = false;

  for (const auto& token : tokens) {
    if (token.type == TokenType::TOKEN_ERROR) {
      foundError = true;
      break;
    }
  }

  check(
    foundError,
    "Detecta caracteres no reconocidos"
  );
}

int main() {
  std::cout << "\n========== PRUEBAS DEL LEXER ==========\n";

  testValidQuery();
  testCaseInsensitiveKeywords();
  testOperators();
  testStringLiteral();
  testInvalidCharacter();

  summary();

  return failed == 0 ? 0 : 1;
}

