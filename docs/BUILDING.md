# Building

## Requirements

- CMake 3.20 or newer
- A C++20-compatible compiler
- Git, when dependencies must be fetched
- GLM 1.0.1, available as a CMake package or fetched automatically

## Configure and build

```bash
cmake -S . -B build -DARYABHATTA_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Use an installed GLM package

```bash
cmake -S . -B build \
  -DARYABHATTA_FETCH_GLM=OFF \
  -DARYABHATTA_BUILD_TESTS=ON
```

## Install

```bash
cmake --install build --prefix install
```

The installed package exports the `Aryabhatta::Math` CMake target.
