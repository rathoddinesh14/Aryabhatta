# Architecture

## Purpose

Aryabhatta is a modular C++ mathematics library. It provides reusable
mathematical types, conventions, and algorithms through stable public APIs.

## Architectural principles

1. **Mathematical focus:** Modules implement mathematical concepts rather than
   application workflows.
2. **Dependency discipline:** Dependencies are introduced only when they
   provide clear value and are isolated behind public APIs where practical.
3. **Explicit conventions:** Coordinate systems, units, precision, and
   numerical assumptions are documented.
4. **Testability:** Algorithms are validated with deterministic unit tests
   and numerical edge cases.
5. **Modularity:** Consumers can use the public modules without depending on
   unrelated application or platform infrastructure.
6. **Stable interfaces:** Public headers live under `include/aryabhatta/`.

## Initial module layout

- `aryabhatta::math`: foundational vectors, matrices, quaternions,
  transformations, and interpolation.
- `aryabhatta::geometry`: planned geometric primitives and predicates.
- `aryabhatta::numerical`: planned numerical algorithms.
- `aryabhatta::computational_geometry`: planned spatial and mesh algorithms.

The initial release implements the foundational math module. Other modules
will be introduced when their API boundaries and tests are sufficiently
well-defined.

## Dependency direction

The library is a foundational component. Its modules must not depend on
application code, rendering engines, window systems, or platform-specific
frameworks.
