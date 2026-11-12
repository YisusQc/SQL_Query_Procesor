# Gramática SQL

## 1. Alcance

La gramática reconoce un subconjunto de consultas SELECT.

Incluye:

- Proyección de columnas.
- Proyección mediante *.
- Una tabla en FROM.
- Condiciones en WHERE.
- Operadores relacionales.
- Operadores lógicos AND y OR.

No incluye subconsultas, JOIN, alias, funciones, GROUP BY,
ORDER BY, paréntesis ni otras construcciones SQL avanzadas.

## 2. Gramática libre de contexto

```bnf
QUERY              -> SELECT_CLAUSE FROM_CLAUSE WHERE_OPT SEMICOLON

SELECT_CLAUSE      -> KEYWORD_SELECT PROJECTION

PROJECTION         -> STAR
                    | COLUMN_LIST

COLUMN_LIST        -> IDENTIFIER COLUMN_TAIL

COLUMN_TAIL        -> COMMA IDENTIFIER COLUMN_TAIL
                    | epsilon

FROM_CLAUSE        -> KEYWORD_FROM IDENTIFIER

WHERE_OPT          -> KEYWORD_WHERE EXPRESSION
                    | epsilon

EXPRESSION         -> AND_EXPRESSION OR_TAIL

OR_TAIL            -> KEYWORD_OR AND_EXPRESSION OR_TAIL
                    | epsilon

AND_EXPRESSION     -> CONDITION AND_TAIL

AND_TAIL           -> KEYWORD_AND CONDITION AND_TAIL
                    | epsilon

CONDITION          -> IDENTIFIER RELATIONAL_OPERATOR VALUE

RELATIONAL_OPERATOR -> OP_EQUAL
                     | OP_NOT_EQUAL
                     | OP_LESS
                     | OP_GREATER
                     | OP_LESS_EQUAL
                     | OP_GREATER_EQUAL

VALUE              -> IDENTIFIER
                    | INTEGER_LITERAL
                    | DECIMAL_LITERAL
                    | STRING_LITERAL
```

## 3. Precedencia lógica

La gramática establece que AND tiene mayor precedencia que OR.

Esto se consigue mediante la separación de:

- EXPRESSION
- AND_EXPRESSION
- CONDITION

Por ejemplo:

```sql
SELECT nombre
FROM estudiantes
WHERE edad >= 18 AND carrera = 'Sistemas' OR edad < 15;
```

Se interpreta conceptualmente como:

```text
(edad >= 18 AND carrera = 'Sistemas') OR edad < 15
```

## 4. Propiedades

- No presenta recursión izquierda.
- Está factorizada para el análisis predictivo.
- Permite producciones epsilon.
- Es compatible con el análisis LL(1), según la validación automática.

## 5. Limitaciones

Esta gramática representa un subconjunto educativo de SQL.
No pretende reconocer la totalidad del lenguaje SQL.

