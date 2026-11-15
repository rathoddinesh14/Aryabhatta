# Mathematical Conventions

This document defines the conventions used by the implemented mathematics API.

## Scalars and units

Aryabhatta provides two scalar precision families:

- Float types use GLM's `float` scalar type.
- Double types use GLM's `double` scalar type.

The public aliases are:

| Float | Double |
|---|---|
| `Vec2` | `DVec2` |
| `Vec3` | `DVec3` |
| `Vec4` | `DVec4` |
| `Mat3` | `DMat3` |
| `Mat4` | `DMat4` |
| `Quat` | `DQuat` |
| `Transform` | `DTransform` |

The float aliases preserve the original API.

The double aliases provide corresponding double-precision types.

Angles are expressed in radians.

Distances and physical units are defined by the consuming application.
When a physical interpretation is required, document the unit at the API
boundary.

## Precision selection and conversion

Choose float or double according to the numerical requirements of the
consuming application.

- Float is appropriate for many real-time rendering and graphics workloads.
- Double is useful when larger coordinate ranges or reduced rounding error
  are important.

Float and double types are intentionally distinct.

The library's helper functions require matching scalar types. For example,
a double vector must be interpolated with a double interpolation parameter.

Conversions between precision families should be explicit. Converting from
double to float can lose precision or range.

Double precision does not guarantee numerical stability. Consumers remain
responsible for choosing appropriate algorithms and tolerances.

## Coordinate systems

- The initial 3D convention is right-handed.
- The nominal up direction is positive Y.
- The basis convention is `cross(+X, +Y) = +Z`.
- Coordinate-system conversions must be explicit at module boundaries.

## Vectors and matrices

- Matrices use GLM's column-major storage.
- Vectors are treated as column vectors.
- Transform points using `M * v`.
- GLM matrix elements are accessed as `matrix[column][row]`.
- Hierarchical transforms compose as `parent * local`.
- TRS conversion uses `T * R * S`; with column vectors, scale is applied
  first, then rotation, then translation.

These conventions apply to both float and double types.

## Quaternions

- Quaternion components follow GLM's `(w, x, y, z)` representation.
- The identity quaternion is `(1, 0, 0, 0)`.
- Quaternions used to represent rotations should be normalized.
- Interpolation uses spherical linear interpolation.

The float identity quaternion is `IDENTITY_QUAT`.

The double identity quaternion is `DIDENTITY_QUAT`.

## Rotation axes

Rotation helpers normalize their input axes.

Zero-length and non-finite axes are rejected with `std::invalid_argument`.

Axis normalization scales the input before calculating its length. This avoids
unnecessary intermediate overflow or underflow for very large or very small
finite axis values.

## Projection and clip-space depth

The foundational math API does not impose a global graphics API clip-space
depth convention. Projection matrices and depth-range choices belong in
explicit projection or rendering-facing APIs.

## GLM configuration

Include the public Aryabhatta header before GLM extension headers in a
translation unit. Do not define conflicting GLM configuration macros in
different translation units of the same program.

GLM extension use is kept limited to the functionality required by the
public API.
