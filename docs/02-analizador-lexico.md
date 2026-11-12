# Analizador léxico

## 1. Objetivo

El analizador léxico transforma una consulta SQL escrita como
cadena de caracteres en una secuencia ordenada de tokens.

Cada token representa una unidad léxica reconocida por el lenguaje.

## 2. Estructura Token

La estructura Token contiene:

- type: categoría léxica.
- lexeme: texto original reconocido.
- pos: posición del token en el código fuente.

Position contiene:

- line: número de línea.
- column: número de columna.

## 3. Categorías léxicas

| Categoría | Ejemplos |
|---|---|
| Palabras reservadas | SELECT, FROM, WHERE, AND, OR |
| Identificadores | nombre, estudiantes, edad |
| Enteros | 18, 100 |
| Decimales | 10.50 |
| Cadenas | 'Sistemas' |
| Operadores | =, <>, <, >, <=, >= |
| Separadores | , ; |
| Comodín | * |
| Error | Caracteres no reconocidos |

## 4. Tabla de palabras reservadas

Keywords.h mantiene una estructura de búsqueda que relaciona
las palabras reservadas con sus respectivos TokenType.

Las palabras clave se reconocen sin distinguir entre mayúsculas
y minúsculas.

Ejemplos:

- SELECT -> KEYWORD_SELECT
- select -> KEYWORD_SELECT
- FROM -> KEYWORD_FROM
- where -> KEYWORD_WHERE

Los identificadores que no coinciden con palabras reservadas
se clasifican como IDENTIFIER.

## 5. Resultado del lexer

El método tokenize() produce un vector de Token.

La secuencia termina con TOKEN_EOF, que indica el fin de la entrada.

Este vector constituye la interfaz de salida del analizador léxico
y la entrada del analizador sintáctico.

## 6. Manejo de errores

Cuando se encuentra una secuencia no reconocida, el lexer genera
TOKEN_ERROR, conservando el lexema y su posición para permitir
el diagnóstico.

