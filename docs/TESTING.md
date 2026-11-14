# Testing

## Framework

Catch2 v3 is used for unit tests. It is fetched only when tests are enabled
and a suitable CMake package is not already installed.

## Run tests

```bash
cmake -S . -B build -DARYABHATTA_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Test expectations

Current test suites are split by concern:

- `test_glm_integration.cpp`: matrix layout, quaternion representation,
  coordinate handedness, transform composition, and interpolation basics.
- `test_math_helpers.cpp`: transform defaults, helper functions, vector
  interpolation, transform interpolation, and quaternion slerp edge cases.
- `test_math.cpp`: consolidated API regression coverage.

Tests should cover:

- Identity and default values.
- Matrix and transform composition.
- Coordinate-system conventions.
- Quaternion operations and normalization.
- Interpolation endpoints and intermediate values.
- Numerical boundaries and degenerate inputs as APIs are added.

Prefer assertions with tolerances for floating-point results. Exact equality
should be used only when the operation and representation make it appropriate.
