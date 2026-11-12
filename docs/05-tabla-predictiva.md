# Tabla de análisis predictivo LL(1)

## 1. Objetivo

La tabla predictiva permite seleccionar una producción
utilizando el no terminal de la cima de la pila y el siguiente
token de entrada.

## 2. Representación

La tabla se representa mediante:

M[NonTerminal][TokenType] = production_id

Cada fila corresponde a un no terminal y cada columna
a un terminal de entrada.

## 3. Construcción

Para cada producción A -> alpha:

- Se inserta la producción en M[A, a] para cada
  terminal a perteneciente a FIRST(alpha).
- Si alpha puede derivar epsilon, se inserta la producción
  en M[A, b] para cada b perteneciente a FOLLOW(A).

## 4. Conflictos

Si una celda intenta recibir dos producciones diferentes,
se detecta un conflicto LL(1).

La tabla se considera válida cuando no existen conflictos.

## 5. Uso

El parser consulta la tabla mediante:

getProduction(nonTerminal, lookahead)

Si la celda está vacía, se informa un error sintáctico
y se obtienen los terminales esperados de esa fila.

