# Public API Design

## Namespace

Public symbols are defined under `aryabhatta`. The initial math API uses
`aryabhatta::math`.

## Header layout

Public headers are installed beneath `include/aryabhatta/`. Include the
foundational math API with:

```cpp
#include <aryabhatta/math/math.hpp>
```

## Naming

- Types: `PascalCase`
- Functions and variables: `snake_case`
- Constants: `UPPER_SNAKE_CASE`
- Namespaces: lowercase
- Header filenames: `snake_case.hpp`

## API requirements

- Prefer value semantics for small mathematical types.
- Document units and numerical assumptions.
- Avoid hidden global state.
- Mark non-mutating utility functions `[[nodiscard]]` where useful.
- Add tests for normal, boundary, and degenerate inputs.
- Avoid exposing implementation details that are not part of the supported API.
