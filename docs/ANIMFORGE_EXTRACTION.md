# AnimForge Math Extraction Inventory

Source repository: `rathoddinesh14/animforge`, branch `main`.

Source files reviewed:

- `include/aga/math/math.hpp`
- `src/math/math_dummy.cpp`
- `tests/math/test_glm_integration.cpp`
- `tests/math/test_math_helpers.cpp`
- `docs/MATH_CONVENTIONS.md`
- `cmake/Dependencies.cmake`

Extracted functionality:

- GLM-backed aliases: `Vec2`, `Vec3`, `Vec4`, `Mat3`, `Mat4`, `Quat`.
- Unit-axis and identity-quaternion constants.
- TRS `Transform`, identity factory, and matrix conversion.
- `translate`, `scale`, `rotate`, and `angle_axis` helpers.
- `lerp` for vectors and transforms; `slerp` for quaternions.
- The existing matrix, quaternion, handedness, transform, and interpolation test themes.

Changes made during extraction:

- Public namespace changed from `aga::math` to `aryabhatta::math`.
- Include path changed from `<aga/math/math.hpp>` to `<aryabhatta/math/math.hpp>`.
- Replaced the engine-specific static-library/dummy translation-unit setup with a small CMake `INTERFACE` target.
- Removed the AnimForge/Vishwakarma/OpenGL/Vulkan build coupling.
- Kept GLM as an external pinned dependency.
- Reworked test coverage into a standalone math test target.

Not included:

- AnimForge samples, rendering, animation, geometry sample helpers, graphics integration, and application-level CMake settings.
- A duplicate copy of GLM itself.
