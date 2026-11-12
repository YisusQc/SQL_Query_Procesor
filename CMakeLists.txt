cmake_minimum_required(VERSION 3.16)

project(AnalizadorLexicoSQL
  VERSION 2.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Biblioteca principal

add_library(sql_core
  src/lexer/Lexer.cpp
  src/parser/Grammar.cpp
  src/parser/FirstFollow.cpp
  src/parser/ParsingTable.cpp
  src/parser/Parser.cpp
)

target_include_directories(sql_core PUBLIC
  src/common
  src/lexer
  src/parser
)

# Aplicacion

add_executable(sql_processor
  src/app/main.cpp
)

target_link_libraries(sql_processor PRIVATE sql_core)

# Pruebas automatizadas

enable_testing()

add_executable(lexer_tests
  tests/lexer_tests.cpp
)

target_link_libraries(lexer_tests PRIVATE sql_core)
target_include_directories(lexer_tests PRIVATE tests)

add_test(
  NAME LexerTests
  COMMAND lexer_tests
)

add_executable(grammar_tests
  tests/grammar_tests.cpp
)

target_link_libraries(grammar_tests PRIVATE sql_core)
target_include_directories(grammar_tests PRIVATE tests)

add_test(
  NAME GrammarTests
  COMMAND grammar_tests
)

add_executable(parser_tests
  tests/parser_tests.cpp
)

target_link_libraries(parser_tests PRIVATE sql_core)
target_include_directories(parser_tests PRIVATE tests)

add_test(
  NAME ParserTests
  COMMAND parser_tests
)

