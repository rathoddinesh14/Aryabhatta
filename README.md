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

```bash
cmake -S . -B build -DARYABHATTA_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

GLM is discovered as a system CMake package when available. Otherwise CMake
fetches the pinned version. Catch2 is fetched only when tests are enabled and
a suitable package is unavailable.

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

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [Mathematical Conventions](docs/MATH_CONVENTIONS.md)
- [API Design](docs/API_DESIGN.md)
- [Dependencies](docs/DEPENDENCIES.md)
- [Building](docs/BUILDING.md)
- [Testing](docs/TESTING.md)

## License

MIT License. See [LICENSE](LICENSE).
