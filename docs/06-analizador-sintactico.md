# Analizador sintáctico LL(1)

## 1. Objetivo

Verificar que la secuencia de tokens generada por el lexer
cumpla la gramática definida.

Se utiliza un analizador predictivo no recursivo.

## 2. Estructuras

El parser utiliza:

- Una pila explícita de símbolos.
- Un vector constante de tokens.
- Un índice para recorrer la entrada.
- La tabla predictiva LL(1).

## 3. Inicialización

La pila comienza con:

TOKEN_EOF
QUERY

QUERY queda en la cima y es el primer símbolo procesado.

## 4. Algoritmo

En cada iteración se consulta:

- El símbolo de la cima.
- El token actual de entrada.

Si la cima es un terminal:

- Si coincide con el token actual, ambos avanzan.
- Si no coincide, se genera un error sintáctico.

Si la cima es un no terminal:

- Se consulta la tabla predictiva.
- Si existe una producción, se retira el no terminal.
- Se insertan los símbolos del lado derecho en orden inverso.

Si coinciden los marcadores TOKEN_EOF, el análisis termina
correctamente.

## 5. Manejo de errores

Los errores léxicos se detectan antes de iniciar el análisis
sintáctico.

Los errores sintácticos incluyen:

- Línea y columna.
- Token encontrado.
- Tokens esperados.

El parser se detiene ante el primer error.

## 6. Complejidad

Sea n la cantidad de tokens de entrada.

El análisis predictivo consume cada token una vez y aplica
las producciones mediante una pila.

Para esta gramática, cuyo tamaño es fijo, el análisis tiene
comportamiento lineal respecto a la longitud de la entrada:
O(n).

El almacenamiento auxiliar de la pila depende de la cantidad
de símbolos pendientes de procesar.

