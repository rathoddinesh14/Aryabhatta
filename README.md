# Aryabhatta

**A shared, reusable C++ mathematics foundation for Dinesh's graphics, simulation, imaging, and engineering projects.**

Aryabhatta centralizes mathematical types, conventions, and algorithms so that consuming repositories do not maintain slightly different copies of vector, matrix, quaternion, and transformation utilities.

## Initial scope

This first extraction establishes a small GLM-backed foundation:

- Common aliases for 2D/3D/4D vectors, 3x3/4x4 matrices, and quaternions.
- Shared coordinate, matrix, angle, and quaternion conventions.
- TRS `Transform` with matrix conversion.
- Translation, scale, rotation, angle-axis, vector interpolation, quaternion interpolation, and transform interpolation helpers.
- Catch2 tests for the extracted behaviour.

This is intentionally a starting point, not a claim that every mathematical domain belongs in one module. Future modules can add geometry, linear algebra utilities, numerical methods, coordinate systems, computational geometry, and domain-specific algorithms without introducing graphics-engine dependencies into the core.

## Dependency and conventions

- **Language:** C++20.
- **Math backend:** GLM 1.0.1 (header-only).
- **Namespace:** `aryabhatta::math`.
- **Public include:** `<aryabhatta/math/math.hpp>`.
- **Coordinate system:** right-handed, Y-up; world-space +Z is the declared forward direction for the initial 3D convention.
- **Matrices:** GLM column-major storage; multiply column vectors using `M * v`; compose local transforms as `parent * local`.
- **Quaternions:** GLM `(w, x, y, z)` representation.
- **Angles:** radians.
- **Transform composition:** `T * R * S`.
- **Depth:** `GLM_FORCE_DEPTH_ZERO_TO_ONE` is enabled for a Vulkan-friendly [0, 1] clip-depth convention. OpenGL consumers must deliberately choose/convert their projection convention rather than silently assuming the default OpenGL [-1, 1] depth range.

### Important configuration rule

GLM configuration macros affect declarations and must be consistent across a program. Include the Aryabhatta public header before any direct GLM include in every translation unit. Do not define conflicting `GLM_*` macros in consuming projects. In a larger codebase, enforce this through a common precompiled header or target-level compile definitions.

## Build

```bash
cmake -S . -B build -DARYABHATTA_BUILD_TESTS=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

GLM is found as a system CMake package when available; otherwise CMake fetches the pinned 1.0.1 release. Catch2 v3 is fetched only when tests are enabled and no package is installed.

## Consume from another CMake project

As a Git submodule:

```bash
git submodule add https://github.com/rathoddinesh14/Aryabhatta.git external/Aryabhatta
add_subdirectory(external/Aryabhatta)
target_link_libraries(your_target PRIVATE Aryabhatta::Math)
```

Or through CMake `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(
    Aryabhatta
    GIT_REPOSITORY https://github.com/rathoddinesh14/Aryabhatta.git
    GIT_TAG        v0.1.0
)
FetchContent_MakeAvailable(Aryabhatta)
target_link_libraries(your_target PRIVATE Aryabhatta::Math)
```

Then in C++:

```cpp
#include <aryabhatta/math/math.hpp>

using namespace aryabhatta::math;

Transform local;
local.translation = Vec3{1.0f, 2.0f, 3.0f};
const Mat4 model = local.to_mat4();
```

Pin a release/tag or commit in consuming projects; avoid tracking the moving default branch for reproducible builds.

## Repository consumers

Expected consumers include:

- **Vishwakarma:** graphics-facing vectors, matrices, transforms, camera/projection helpers.
- **ImageRegistration:** image geometry, coordinate transforms, and numerical/linear-algebra utilities where appropriate.
- **DharaniNirmana:** terrain geometry, spatial transforms, and computational geometry.
- **AnimForge:** animation transforms, quaternion interpolation, and skeletal math.
- **Future projects:** depend only on the modules they need; Aryabhatta must not depend on any consumer repository.

## Architecture rules

1. Aryabhatta is a foundational dependency; it must never depend on Vishwakarma, AnimForge, ImageRegistration, or DharaniNirmana.
2. Keep the core independent of graphics APIs, window systems, rendering engines, and application frameworks.
3. Expose stable, documented APIs under `aryabhatta::` and avoid leaking project-specific naming into public headers.
4. Keep mathematical conventions explicit and tested.
5. Add algorithms with tests and numerical edge-case coverage before integrating them into consumers.
6. Prefer header-only APIs for small GLM adapters; introduce compiled targets only when there is a real implementation or ABI reason.
7. Do not copy third-party GLM source into this repository; consume GLM as a dependency.

## Migration note

The initial API is extracted and renamed from the math layer currently present in AnimForge (`aga::math`). It is a deliberate public-API transition to `aryabhatta::math`; consuming applications should migrate their includes and namespace references together. The extraction does not modify AnimForge.

## License

MIT. See [LICENSE](LICENSE).
