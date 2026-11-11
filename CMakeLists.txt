cmake_minimum_required(VERSION 3.16)

project(AnalizadorLexicoSQL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sql_processor
  src/app/main.cpp
  # Lexer
  src/lexer/Lexer.cpp
  # Parser
  src/parser/Grammar.cpp
  src/parser/FirstFollow.cpp
)

target_include_directories(sql_processor PRIVATE
  src/common
  src/lexer
  src/parser
)

