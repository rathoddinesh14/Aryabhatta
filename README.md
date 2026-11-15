# Aryabhatta

**A modular, reusable C++ mathematics library.**

Aryabhatta provides foundational mathematical abstractions and algorithms for
software involving graphics, geometry, simulation, computational mathematics,
and scientific computing.

The project aims to offer consistent mathematical conventions, well-tested
implementations, and a modular public API for modern C++ applications.

## Goals

- Provide consistent mathematical abstractions.
- Maintain a modular and extensible architecture.
- Make coordinate, unit, and numerical conventions explicit.
- Support modern C++ development.
- Maintain comprehensive unit tests.
- Minimize unnecessary dependencies.
- Provide documented and stable public APIs.

## Current capabilities

The initial release provides:

- 2D, 3D, and 4D vector aliases.
- 3x3 and 4x4 matrix aliases.
- Quaternion aliases and identity constants.
- TRS transformations and matrix conversion.
- Translation, scale, rotation, and angle-axis helpers.
- Vector and transform linear interpolation.
- Quaternion spherical linear interpolation.
- Generic transform and helper implementations for float and double.

## Precision support

Aryabhatta provides two explicit precision families.

| Float API | Double API |
|---|---|
| `Vec2` | `DVec2` |
| `Vec3` | `DVec3` |
| `Vec4` | `DVec4` |
| `Mat3` | `DMat3` |
| `Mat4` | `DMat4` |
| `Quat` | `DQuat` |
| `Transform` | `DTransform` |

The float API remains the default and retains its existing names.

The double API uses the corresponding GLM double-precision types.

Example:

```cpp
#include <aryabhatta/math/math.hpp>

using namespace aryabhatta::math;

int main() {
    Transform float_transform;
    float_transform.translation = Vec3{1.0f, 2.0f, 3.0f};

    DTransform double_transform;
    double_transform.translation = DVec3{1.0, 2.0, 3.0};

    const Mat4 float_matrix = float_transform.to_mat4();
    const DMat4 double_matrix = double_transform.to_mat4();
}
```

## Planned modules

- **Geometry:** points, lines, rays, planes, bounding volumes, and intersections.
- **Numerical mathematics:** linear algebra utilities and numerical methods.
- **Computational geometry:** triangulation, spatial structures, and mesh algorithms.

Planned modules are not represented as implemented functionality until their
APIs and tests are added.

## Technology

- **Language:** C++20
- **Build system:** CMake 3.20+
- **Math backend:** GLM 1.0.1
- **Testing:** Catch2 v3
- **License:** MIT

## Mathematical conventions

- Right-handed 3D basis.
- Positive Y is the nominal up direction.
- `cross(+X, +Y) = +Z`.
- Column-major matrices and column-vector multiplication (`M * v`).
- TRS composition: `T * R * S`.
- Quaternion component order: `(w, x, y, z)`.
- Angles are expressed in radians.
- No global graphics API clip-space depth convention is imposed by the core API.

See [Mathematical Conventions](docs/MATH_CONVENTIONS.md) for details.

## Build

### Requirements

- CMake 3.20 or later
- C++20 compatible compiler (GCC 10 or later recommended)
- Git
- GLM 1.0.1 (managed through the project's CMake dependency configuration)

### Configure

```bash
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=gcc-10 \
    -DCMAKE_CXX_COMPILER=g++-10 \
    -DARYABHATTA_BUILD_TESTS=ON
```

### Build

```bash
cmake --build build --parallel 4
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

### Install

```bash
cmake --install build --prefix "$PWD/build/install"
```

### Package Consumer Validation

Aryabhatta exports the `Aryabhatta::Math` CMake target. The following steps validate that an external CMake project can discover and consume the installed library.

```bash
cmake -S tests/package-consumer \
    -B build/consumer \
    -DCMAKE_PREFIX_PATH="$PWD/build/install" \
    -DCMAKE_CXX_COMPILER=g++-10

cmake --build build/consumer --parallel 4

ctest --test-dir build/consumer --output-on-failure
```

### Clean Build

To start from a clean build directory:

```bash
rm -rf build

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=gcc-10 \
    -DCMAKE_CXX_COMPILER=g++-10 \
    -DARYABHATTA_BUILD_TESTS=ON

cmake --build build --parallel 4

ctest --test-dir build --output-on-failure
```

### Using Aryabhatta in Another CMake Project

```cmake
find_package(Aryabhatta CONFIG REQUIRED)

add_executable(my_application main.cpp)

target_link_libraries(
    my_application
    PRIVATE
        Aryabhatta::Math
)
```

The exported target propagates the required C++20 compile feature to consuming targets.

## Use from C++

```cpp
#include <aryabhatta/math/math.hpp>

int main() {
    aryabhatta::math::Transform transform;
    transform.translation = {1.0f, 2.0f, 3.0f};

    const aryabhatta::math::Mat4 matrix = transform.to_mat4();
}
```

## Consume through CMake

As a subdirectory:

```cmake
add_subdirectory(path/to/Aryabhatta)
target_link_libraries(your_target PRIVATE Aryabhatta::Math)
```

Or use the installed package:

```cmake
find_package(Aryabhatta CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE Aryabhatta::Math)
```

## Dependency

Aryabhatta uses [GLM](https://github.com/g-truc/glm) as its
mathematics backend.

GLM is a public dependency because Aryabhatta exposes GLM types
through its public API.

### Building Aryabhatta

CMake first searches for an installed GLM package.

If GLM is not found, Aryabhatta can fetch the pinned GLM version
when `ARYABHATTA_FETCH_GLM` is enabled.

### Using an installed Aryabhatta package

Projects consuming an installed Aryabhatta package must also have
GLM installed and discoverable by CMake.

The GLM dependency is resolved through `find_dependency(glm CONFIG)`
in the installed Aryabhatta package configuration.

Fetching GLM while building Aryabhatta does not automatically install
GLM for downstream projects.

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [Mathematical Conventions](docs/MATH_CONVENTIONS.md)
- [API Design](docs/API_DESIGN.md)
- [Dependencies](docs/DEPENDENCIES.md)
- [Building](docs/BUILDING.md)
- [Testing](docs/TESTING.md)

## License

MIT License. See [LICENSE](LICENSE).
