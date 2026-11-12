# Arquitectura general

## 1. Descripción

El proyecto implementa un procesador básico de consultas SQL en C++17.
Su arquitectura está organizada en fases independientes, donde cada
fase recibe una estructura de datos producida por la anterior.

## 2. Flujo de procesamiento

```text
Consulta SQL
     |
     v
Analizador lexico (Lexer)
     |
     v
Vector de tokens
     |
     v
Analizador sintactico (Parser LL(1))
     |
     v
Resultado sintactico
     |
     v
Futuras fases de procesamiento
```

## 3. Módulos

### common

Contiene estructuras compartidas, principalmente Token y Position.

### lexer

Transforma el texto SQL en una secuencia de tokens.

### parser

Contiene la gramática, los algoritmos FIRST y FOLLOW,
la tabla predictiva y el analizador sintáctico LL(1).

### app

Contiene el punto de entrada del programa y coordina las fases.

### tests

Contiene pruebas automatizadas independientes de la aplicación.

### docs

Contiene la documentación técnica y teórica.

## 4. Principios de diseño

- Separación de responsabilidades.
- Reutilización de componentes.
- Independencia entre fases.
- Validación mediante pruebas automatizadas.
- Extensibilidad para futuras etapas.

