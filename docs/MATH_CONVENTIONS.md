# Mathematical Conventions

This document defines the conventions used by the currently implemented
mathematics API.

## Scalars and units

- Runtime vector, matrix, and quaternion aliases use GLM's default scalar
  type, `float`.
- Angles are expressed in radians.
- Distances and physical units are defined by the consuming application.
  When a physical interpretation is required, document the unit at the API
  boundary.

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

## Quaternions

- Quaternion components follow GLM's `(w, x, y, z)` representation.
- The identity quaternion is `(1, 0, 0, 0)`.
- Quaternions used to represent rotations should be normalized.
- Interpolation uses spherical linear interpolation.

## Projection and clip-space depth

The foundational math API does not impose a global graphics API clip-space
depth convention. Projection matrices and depth-range choices belong in
explicit projection or rendering-facing APIs.

## GLM configuration

Include the public Aryabhatta header before GLM extension headers in a
translation unit. Do not define conflicting GLM configuration macros in
different translation units of the same program. GLM extension use is kept
limited to the functionality required by the public API.
