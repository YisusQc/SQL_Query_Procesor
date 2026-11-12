# Analizador Léxico y Sintáctico SQL

Procesador básico de consultas SQL desarrollado en C++17.

El proyecto implementa un analizador léxico y un analizador
sintáctico predictivo no recursivo LL(1).

## Características

- Reconocimiento de tokens SQL.
- Identificación de palabras reservadas sin distinguir mayúsculas.
- Seguimiento de línea y columna.
- Gramática libre de contexto para consultas SELECT.
- Cálculo automático de FIRST y FOLLOW.
- Validación de conflictos LL(1).
- Construcción de tabla predictiva.
- Parser iterativo basado en pila explícita.
- Diagnóstico de errores léxicos y sintácticos.
- Traza paso a paso del análisis.
- Pruebas automatizadas mediante CTest.

## Requisitos

- C++17.
- CMake 3.16 o superior.
- Compilador compatible con C++17.

## Compilación

```bash
cmake -S . -B build
cmake --build build
```

## Ejecutar el procesador

Desde la entrada estándar:

```bash
./build/sql_processor
```

Desde un archivo:

```bash
./build/sql_processor query.sql
```

## Ejecutar pruebas

```bash
ctest --test-dir build --output-on-failure
```

## Documentación

La documentación técnica se encuentra en `docs/`.

Consulta `docs/01-arquitectura.md` para conocer la organización
general y los documentos especializados para cada componente.

## Alcance

La gramática implementada reconoce un subconjunto de SQL
centrado en consultas SELECT, proyecciones, FROM y WHERE.

No representa la totalidad del lenguaje SQL.

