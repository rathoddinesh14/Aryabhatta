# Public API Design

## 1. Namespace

Public mathematical symbols are defined under the nested namespace `aryabhatta::math`.

The foundational mathematics API uses the namespace:

```cpp
namespace aryabhatta::math {
}
```

All public mathematical types, constants, and helper functions belong to this namespace.

## 2. Header Layout

Public headers are installed beneath `include/aryabhatta/`.

Include the foundational mathematics API using:

```cpp
#include <aryabhatta/math/math.hpp>
```

Consumers should prefer public Aryabhatta headers instead of depending directly on internal implementation headers.

## 3. Naming Conventions

The API follows these naming conventions:

- **Types:** `PascalCase`
- **Functions:** `snake_case`
- **Variables:** `snake_case`
- **Constants:** `UPPER_SNAKE_CASE`
- **Namespaces:** lowercase
- **Header filenames:** `snake_case.hpp`

Examples:

```cpp
Vec3 position;
DTransform transform;

const DVec3 direction{1.0, 0.0, 0.0};

const DMat4 matrix = translate(direction);
```

## 4. Precision Model

Aryabhatta supports two explicit scalar precision families:

- **Single precision:** Uses `float`.
- **Double precision:** Uses `double`.

The existing single-precision API retains its original public names. Double-precision types use a `D` prefix.

### 4.1 Single-Precision Types

| Category | Public Type | GLM Type |
|---|---|---|
| 2D vector | `Vec2` | `glm::vec2` |
| 3D vector | `Vec3` | `glm::vec3` |
| 4D vector | `Vec4` | `glm::vec4` |
| 3x3 matrix | `Mat3` | `glm::mat3` |
| 4x4 matrix | `Mat4` | `glm::mat4` |
| Quaternion | `Quat` | `glm::quat` |
| Transform | `Transform` | `BasicTransform<float>` |

### 4.2 Double-Precision Types

| Category | Public Type | GLM Type |
|---|---|---|
| 2D vector | `DVec2` | `glm::dvec2` |
| 3D vector | `DVec3` | `glm::dvec3` |
| 4D vector | `DVec4` | `glm::dvec4` |
| 3x3 matrix | `DMat3` | `glm::dmat3` |
| 4x4 matrix | `DMat4` | `glm::dmat4` |
| Quaternion | `DQuat` | `glm::dquat` |
| Transform | `DTransform` | `BasicTransform<double>` |

### 4.3 Precision Selection

Consumers should select precision according to the requirements of their applications.

Use single precision when:

- Working with real-time rendering pipelines that use float data.
- Interoperating with graphics APIs or libraries that expect float values.
- Memory bandwidth and storage requirements are important.

Use double precision when:

- Working with large coordinate ranges.
- Performing numerical computations that require additional precision.
- Accumulating geometric or simulation calculations where float rounding errors are significant.
- Developing scientific, engineering, or computational geometry applications.

Double precision does not automatically guarantee numerical stability. Algorithm design, conditioning, and error analysis remain important.

## 5. Generic Implementation Architecture

The implementation uses scalar-parameterized types to share mathematical operations across supported precision families.

The generic implementation is based on:

```cpp
template <typename T>
struct BasicTransform;
```

The public convenience aliases are:

```cpp
using Transform = BasicTransform<float>;
using DTransform = BasicTransform<double>;
```

The currently supported scalar types are:

- `float`
- `double`

The generic implementation should avoid unnecessary duplication while preserving explicit and predictable public APIs.

Consumers using ordinary mathematical operations should not need to work directly with templates.

## 6. Constants

The API provides precision-specific coordinate basis vectors and identity quaternions.

### 6.1 Single Precision

```cpp
inline const Vec3 UNIT_X{1.0f, 0.0f, 0.0f};
inline const Vec3 UNIT_Y{0.0f, 1.0f, 0.0f};
inline const Vec3 UNIT_Z{0.0f, 0.0f, 1.0f};

inline const Quat IDENTITY_QUAT{1.0f, 0.0f, 0.0f, 0.0f};
```

### 6.2 Double Precision

```cpp
inline const DVec3 DUNIT_X{1.0, 0.0, 0.0};
inline const DVec3 DUNIT_Y{0.0, 1.0, 0.0};
inline const DVec3 DUNIT_Z{0.0, 0.0, 1.0};

inline const DQuat DIDENTITY_QUAT{1.0, 0.0, 0.0, 0.0};
```

The `D` prefix consistently distinguishes double-precision constants from their single-precision counterparts.

## 7. Transform API

The transform abstraction represents translation, rotation, and scale.

The generic implementation is:

```cpp
template <typename T>
struct BasicTransform {
    using Scalar = T;
    using Vec3Type = BasicVec3<T>;
    using QuatType = BasicQuat<T>;
    using Mat4Type = BasicMat4<T>;

    Vec3Type translation{T(0)};
    QuatType rotation{T(1), T(0), T(0), T(0)};
    Vec3Type scale{T(1)};

    [[nodiscard]] Mat4Type to_mat4() const;

    [[nodiscard]] static BasicTransform identity();
};
```

The public API exposes:

```cpp
using Transform = BasicTransform<float>;
using DTransform = BasicTransform<double>;
```

### 7.1 Transform Composition

Transforms are converted to matrices using:

```text
M = T * R * S
```

With column-vector multiplication, scale is applied first, followed by rotation and translation.

### 7.2 Identity Transform

A default-constructed transform represents the identity transformation:

- Translation: `(0, 0, 0)`
- Rotation: `(1, 0, 0, 0)`
- Scale: `(1, 1, 1)`

Both `Transform` and `DTransform` provide an `identity()` factory.

## 8. Mathematical Helper Functions

The API provides precision-aware helper functions for transformations and interpolation.

The same function names are used for both precision families. Each overload has an explicitly defined return type.

### 8.1 Transformation Helpers

**Translation**

```cpp
[[nodiscard]] BasicMat4<float> translate(const BasicVec3<float>& value);
[[nodiscard]] BasicMat4<double> translate(const BasicVec3<double>& value);
```

Public API equivalents:

```cpp
[[nodiscard]] Mat4 translate(const Vec3& value);
[[nodiscard]] DMat4 translate(const DVec3& value);
```

**Scaling**

```cpp
[[nodiscard]] BasicMat4<float> scale(const BasicVec3<float>& value);
[[nodiscard]] BasicMat4<double> scale(const BasicVec3<double>& value);
```

Public API equivalents:

```cpp
[[nodiscard]] Mat4 scale(const Vec3& value);
[[nodiscard]] DMat4 scale(const DVec3& value);
```

**Rotation**

```cpp
[[nodiscard]] BasicMat4<float> rotate(
    float radians,
    const BasicVec3<float>& axis);

[[nodiscard]] BasicMat4<double> rotate(
    double radians,
    const BasicVec3<double>& axis);
```

Public API equivalents:

```cpp
[[nodiscard]] Mat4 rotate(float radians, const Vec3& axis);
[[nodiscard]] DMat4 rotate(double radians, const DVec3& axis);
```

**Angle-axis quaternion construction**

```cpp
[[nodiscard]] BasicQuat<float> angle_axis(
    float radians,
    const BasicVec3<float>& axis);

[[nodiscard]] BasicQuat<double> angle_axis(
    double radians,
    const BasicVec3<double>& axis);
```

Public API equivalents:

```cpp
[[nodiscard]] Quat angle_axis(float radians, const Vec3& axis);
[[nodiscard]] DQuat angle_axis(double radians, const DVec3& axis);
```

### 8.2 Interpolation Helpers

**Vector linear interpolation**

```cpp
[[nodiscard]] BasicVec3<float> lerp(
    const BasicVec3<float>& a,
    const BasicVec3<float>& b,
    float t);

[[nodiscard]] BasicVec3<double> lerp(
    const BasicVec3<double>& a,
    const BasicVec3<double>& b,
    double t);
```

Public API equivalents:

```cpp
[[nodiscard]] Vec3 lerp(const Vec3& a, const Vec3& b, float t);
[[nodiscard]] DVec3 lerp(const DVec3& a, const DVec3& b, double t);
```

**Quaternion spherical linear interpolation**

```cpp
[[nodiscard]] BasicQuat<float> slerp(
    const BasicQuat<float>& a,
    const BasicQuat<float>& b,
    float t);

[[nodiscard]] BasicQuat<double> slerp(
    const BasicQuat<double>& a,
    const BasicQuat<double>& b,
    double t);
```

Public API equivalents:

```cpp
[[nodiscard]] Quat slerp(const Quat& a, const Quat& b, float t);
[[nodiscard]] DQuat slerp(const DQuat& a, const DQuat& b, double t);
```

**Transform interpolation**

```cpp
[[nodiscard]] BasicTransform<float> lerp(
    const BasicTransform<float>& a,
    const BasicTransform<float>& b,
    float t);

[[nodiscard]] BasicTransform<double> lerp(
    const BasicTransform<double>& a,
    const BasicTransform<double>& b,
    double t);
```

Public API equivalents:

```cpp
[[nodiscard]] Transform lerp(
    const Transform& a,
    const Transform& b,
    float t);

[[nodiscard]] DTransform lerp(
    const DTransform& a,
    const DTransform& b,
    double t);
```

### 8.3 Return-Type Consistency

The return type of every helper must preserve the precision of its input family.

| Helper | Float return type | Double return type |
|---|---|---|
| `translate` | `Mat4` | `DMat4` |
| `scale` | `Mat4` | `DMat4` |
| `rotate` | `Mat4` | `DMat4` |
| `angle_axis` | `Quat` | `DQuat` |
| Vector `lerp` | `Vec3` | `DVec3` |
| `slerp` | `Quat` | `DQuat` |
| Transform `lerp` | `Transform` | `DTransform` |

The generic implementation must produce these exact public return types.

### 8.4 Precision-Safe Overload Resolution

Mathematical helper functions must not silently mix single- and double-precision inputs.

For example:

```cpp
const DVec3 a{0.0, 0.0, 0.0};
const DVec3 b{10.0, 10.0, 10.0};

// Valid: double vectors and double interpolation parameter.
const DVec3 result = lerp(a, b, 0.5);
```

The following should not compile:

```cpp
// Invalid: float interpolation parameter with double vectors.
const DVec3 result = lerp(a, b, 0.5f);
```

This restriction prevents accidental precision changes and makes overload resolution predictable.

## 9. Numerical and Input Validation

Mathematical operations should document their numerical assumptions and handle invalid inputs consistently.

### 9.1 Rotation Axis Validation

Rotation helpers normalize their input axes.

The following inputs are invalid:

- Zero-length axes.
- Axes containing NaN components.
- Axes containing positive or negative infinity.

Invalid axes should result in `std::invalid_argument`.

Axis normalization should avoid unnecessary intermediate overflow and underflow, particularly when handling very large or very small finite double-precision vectors.

### 9.2 Quaternion Requirements

Quaternions used to represent rotations should be normalized.

Interpolation should preserve the expected quaternion rotation behavior and produce normalized intermediate rotations for valid normalized inputs.

### 9.3 Precision Preservation

Double-precision operations must remain in double precision throughout their calculations.

Implementations must avoid unintended conversions to `float` in:

- Scalar calculations.
- Vector operations.
- Matrix construction and multiplication.
- Quaternion operations.
- Transform composition.
- Interpolation.

Any conversion that can lose precision must be explicit at the API boundary.

## 10. Type Safety

The API should use compile-time type constraints wherever appropriate.

Requirements:

- Float helpers must return float-based GLM types.
- Double helpers must return double-based GLM types.
- Generic implementations must restrict supported scalar types.
- Mixed-precision operations must not introduce implicit narrowing.
- Public aliases must retain their documented underlying GLM types.

Compile-time assertions should verify important public type relationships.

## 11. Backward Compatibility

Existing public float APIs are considered stable.

Adding double precision must not require existing consumers to modify their code.

The following names and their float semantics must remain available:

```cpp
Vec2
Vec3
Vec4

Mat3
Mat4

Quat
Transform

UNIT_X
UNIT_Y
UNIT_Z
IDENTITY_QUAT
```

The double API is additive:

```cpp
DVec2
DVec3
DVec4

DMat3
DMat4

DQuat
DTransform

DUNIT_X
DUNIT_Y
DUNIT_Z
DIDENTITY_QUAT
```

Existing float tests must continue to pass without requiring changes to their expected behavior.

## 12. Dependency and Build Requirements

The public mathematics API uses GLM as its underlying mathematical backend.

Requirements:

- Preserve the currently pinned GLM version.
- Do not introduce additional runtime dependencies for double precision.
- Keep GLM available through the public `Aryabhatta::Math` target.
- Preserve the installed CMake package interface.
- Ensure downstream consumers can use both precision families through the installed package.

The double-precision implementation must not require separate linking targets or additional package configuration.

## 13. Testing Requirements

Every public precision family must have appropriate tests.

### 13.1 Float Compatibility

Existing float tests must continue to verify:

- Vector and matrix behavior.
- Quaternion behavior.
- Transform composition.
- Translation, rotation, and scale helpers.
- Linear and spherical interpolation.
- Invalid rotation axes.

### 13.2 Double Precision

Double tests should cover:

- Vector construction and arithmetic.
- Dot product, cross product, length, and normalization.
- Matrix identity, translation, scaling, and multiplication.
- Homogeneous point transformations.
- Quaternion construction, normalization, rotation, and SLERP.
- Transform identity and TRS composition.
- Vector and transform interpolation.
- Large and small finite values.
- Zero and non-finite rotation axes.
- Values for which float precision is insufficient.
- Compile-time scalar type checks.

### 13.3 Installed Package Consumer

The package-consumer test must compile and execute representative operations using both float and double types.

This verifies that the installed package exposes the complete public API without requiring internal repository paths.

## 14. API Evolution Principles

Future API extensions should follow these principles:

- Prefer generic implementations when multiple scalar types share identical mathematical behavior.
- Keep public convenience aliases explicit and easy to discover.
- Preserve established public names and semantics.
- Avoid exposing implementation details unnecessarily.
- Prefer compile-time constraints over runtime type ambiguity.
- Document numerical assumptions and precision requirements.
- Add tests for normal, boundary, and degenerate inputs.
- Avoid introducing dependencies without a clear architectural need.
- Keep public headers usable by downstream C++20 applications.

The goal is to provide a reusable mathematical foundation that can serve graphics, geometry, simulation, scientific computing, and other computational applications without forcing consumers to adopt unnecessary implementation complexity.
