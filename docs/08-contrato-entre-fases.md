# Contrato entre fases

## 1. Objetivo

Definir las estructuras de datos que permiten conectar
las diferentes etapas del procesador SQL.

## 2. Salida del lexer

El lexer produce:

std::vector<Token>

Cada token conserva:

- Tipo.
- Lexema.
- Posición en el código fuente.

El vector termina con TOKEN_EOF.

El lexer no asigna tipos semánticos ni interpreta el significado
de los identificadores.

## 3. Entrada del parser

El parser recibe el vector de tokens mediante una referencia
constante.

No modifica ni elimina los tokens originales.

## 4. Salida del parser

El parser produce ParseResult, que indica:

- Si la consulta fue aceptada.
- Si ocurrió un error léxico.
- El token encontrado.
- Los tokens esperados.
- El mensaje del diagnóstico.

## 5. Posibles fases posteriores

Una futura etapa podrá incorporar:

- Construcción de un árbol sintáctico o AST.
- Validación de tablas y columnas.
- Tabla de símbolos.
- Comprobación de tipos.
- Resolución de identificadores.
- Traducción a un plan de ejecución.

## 6. Extensibilidad

Las fases deben mantener interfaces independientes.
Las nuevas etapas podrán utilizar los tokens y resultados
producidos por las fases anteriores sin modificar su lógica
interna innecesariamente.

