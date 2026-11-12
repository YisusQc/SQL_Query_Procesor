# Pruebas automatizadas

## 1. Objetivo

Verificar de forma repetible el comportamiento del lexer,
la gramática, la tabla predictiva y el parser.

## 2. Organización

- lexer_tests: reconocimiento de tokens y errores léxicos.
- grammar_tests: producciones, FIRST, FOLLOW y LL(1).
- parser_tests: aceptación y rechazo de consultas.

## 3. Compilación y ejecución

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Para ejecutar un grupo individual:

```bash
ctest --test-dir build -R LexerTests --output-on-failure
ctest --test-dir build -R GrammarTests --output-on-failure
ctest --test-dir build -R ParserTests --output-on-failure
```

Para ejecutar las comprobaciones individuales:

```bash
./build/lexer_tests
./build/grammar_tests
./build/parser_tests
```

## 4. Casos de prueba

### Lexer

- Palabras reservadas.
- Identificadores.
- Operadores.
- Literales enteros, decimales y cadenas.
- Caracteres no reconocidos.
- Posiciones de tokens.

### Gramática

- Cantidad de producciones.
- Conjuntos FIRST.
- Conjuntos FOLLOW.
- Producciones nullable.
- Ausencia de conflictos LL(1).
- Entradas relevantes de la tabla.

### Parser

- Proyección con asterisco.
- Una o varias columnas.
- Condiciones relacionales.
- Expresiones AND y OR.
- Consultas inválidas.
- Errores léxicos.

## 5. Criterio de éxito

Todas las pruebas deben finalizar correctamente.
CTest devuelve un estado distinto de cero cuando falla
alguno de los ejecutables de pruebas.

