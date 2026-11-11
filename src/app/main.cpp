#include <iostream>
#include <vector>
#include "../lexer/Lexer.h"
#include "../parser/Grammar.h"
#include "../parser/FirstFollow.h"

#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"
#define MAGENTA_COLOR "\033[35m"
#define RESET_COLOR "\033[0m"

std::unordered_map<TokenType, std::string> tokenTypes =
{
  {TokenType::KEYWORD_SELECT, "KEYWORD_SELECT"},
  {TokenType::KEYWORD_FROM, "KEYWORD_FROM"},
  {TokenType::KEYWORD_WHERE, "KEYWORD_WHERE"},
  {TokenType::KEYWORD_AND, "KEYWORD_AND"},
  {TokenType::KEYWORD_OR, "KEYWORD_OR"},
  {TokenType::IDENTIFIER, "IDENTIFIER"},
  {TokenType::INTEGER_LITERAL, "INTEGER_LITERAL"},
  {TokenType::DECIMAL_LITERAL, "DECIMAL_LITERAL"},
  {TokenType::STRING_LITERAL, "STRING_LITERAL"},
  {TokenType::OP_EQUAL, "OP_EQUAL"},
  {TokenType::OP_NOT_EQUAL, "OP_NOT_EQUAL"},
  {TokenType::OP_LESS, "OP_LESS"},
  {TokenType::OP_GREATER, "OP_GREATER"},
  {TokenType::OP_LESS_EQUAL, "OP_LESS_EQUAL"},
  {TokenType::OP_GREATER_EQUAL, "OP_GREATER_EQUAL"},
  {TokenType::COMMA, "COMMA"},
  {TokenType::SEMICOLON, "SEMICOLON"},
  {TokenType::STAR, "STAR"},
  {TokenType::TOKEN_EOF, "TOKEN_EOF"},
  {TokenType::TOKEN_ERROR, "TOKEN_ERROR"},
};

std::string printTokenType(TokenType type, const std::unordered_map<TokenType, std::string>& tokenTypes)
{
  auto it = tokenTypes.find(type);
  if (it != tokenTypes.end())
    return it->second;
  return "UNKNOWN_TOKEN";
}

void printAnalisis(const std::vector<Token>& tokens)
{
  for (const auto& token : tokens) {
    std::cout << token.lexeme << " | " 
    << printTokenType(token.type, tokenTypes) << " | " 
    << token.pos.line << " | " 
    << token.pos.column << "\n";
  }
}

int main() {

  // PRUEBA DE LEXER
  
  std::string sqlQuery1 = "SELECT nombre, edad FROM estudiantes WHERE edad >= 18;";
  std::string sqlQuery2 = "SelEcT * FroM estudiantes wHErE nombre <> 'Jesus AnD edad <= 23;";
  std::string sqlQuery3 = "select nombre, score from \nestudiantes where nombre <> 'Jesus' \nand score <= 15.234 or ID # '4321;";

  Lexer lexer1(sqlQuery1);
  Lexer lexer2(sqlQuery2);
  Lexer lexer3(sqlQuery3);
  
  std::vector<Token> tokens1 = lexer1.tokenize();
  std::vector<Token> tokens2 = lexer2.tokenize();
  std::vector<Token> tokens3 = lexer3.tokenize();
  
  std::cout << BLUE_COLOR << "\n[sqlQuery1]: " << GREEN_COLOR << sqlQuery1 << RESET_COLOR << "\n";
  std::cout << MAGENTA_COLOR << "\nAnálisis Léxico: [Lexema | Tipo de token | Línea | Columna]" << RESET_COLOR << "\n";
  printAnalisis(tokens1);

  std::cout << BLUE_COLOR << "\n[sqlQuery2]: " << GREEN_COLOR << sqlQuery2 << RESET_COLOR << "\n";
  std::cout << MAGENTA_COLOR << "\nAnálisis Léxico: [Lexema | Tipo de token | Línea | Columna]" << RESET_COLOR "\n";
  printAnalisis(tokens2);

  std::cout << BLUE_COLOR << "\n[sqlQuery3]: " << GREEN_COLOR << sqlQuery3 << RESET_COLOR << "\n";
  std::cout << MAGENTA_COLOR << "\nAnálisis Léxico: [Lexema | Tipo de token | Línea | Columna]" << RESET_COLOR << "\n";
  printAnalisis(tokens3);


  // PRUEBA DE LA GRAMATICA

  std::cout << "\n\nProducciones de la gramatica LL(1):\n";

  Grammar grammar;

  for (const auto& production : grammar.getProductions()) {
    std::cout << "(" << production.id << ") " << toString(production.lhs) << " -> ";

    if (production.rhs.empty()) {
      std::cout << "epsilon";
    } else {
      for (const auto& symbol : production.rhs) {
        std::cout << toString(symbol) << " ";
      }
    }

    std::cout << "\n";
  }

  std::cout << "\nTotal de producciones: " << grammar.getProductions().size() << "\n";


  // Calculo de FIRST y FOLLOW

  FirstFollow firstFollow(grammar);

  firstFollow.printFirstSets();
  firstFollow.printFollowSets();
  firstFollow.printLL1Conflicts();
 
  return 0;
}

