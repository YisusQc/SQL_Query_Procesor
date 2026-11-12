# Conjuntos FIRST y FOLLOW

## 1. FIRST

FIRST(A) contiene los terminales que pueden aparecer al inicio
de alguna cadena derivada del no terminal A.

Si una producción puede generar la cadena vacía, se indica
que el no terminal es nullable.

## 2. FOLLOW

FOLLOW(A) contiene los terminales que pueden aparecer
inmediatamente después del no terminal A en alguna derivación.

Para el símbolo inicial QUERY se agrega TOKEN_EOF.

## 3. Representación

El proyecto utiliza:

- std::set<TokenType> para los terminales.
- std::set<NonTerminal> para los no terminales nullable.
- Un bool independiente para indicar si una secuencia deriva epsilon.

Epsilon no se representa como TokenType porque no es un token
que aparezca en la entrada.

## 4. Algoritmo

Los conjuntos se calculan mediante iteraciones de punto fijo.

Se recorren las producciones y se agregan los terminales que
pueden deducirse. El proceso continúa hasta que ninguna
iteración modifica los conjuntos.

## 5. Validación LL(1)

Para cada no terminal con alternativas:

1. Se comparan los FIRST de sus producciones.
2. Se comprueba que las alternativas no compartan terminales.
3. Si una alternativa produce epsilon, se compara FIRST
   de las demás alternativas con FOLLOW del no terminal.

Si se cumplen estas condiciones, no existen conflictos
predictivos para la gramática.

