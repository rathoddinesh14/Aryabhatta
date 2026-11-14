# Math Conventions

This document defines project-wide conventions. Consumers must not silently override them.

## Scalar and units

- Runtime graphics and geometry defaults use `float`.
- Angles are radians.
- Distances are application-defined; use meters when a physical interpretation is required and document other units.

## Coordinate system

- Handedness: right-handed.
- Up axis: +Y.
- Initial world-space forward convention: +Z. Camera/view-space conventions are subsystem-specific and must be documented at API boundaries.
- Basis: `cross(+X, +Y) = +Z`.

## Vectors and matrices

- GLM column-major storage.
- Column vectors; transform points with `M * v`.
- Matrix access in GLM is `matrix[column][row]`.
- Hierarchical transform composition: `parent * local`.
- TRS conversion: `T * R * S` (scale first, then rotate, then translate when applied to a column vector).

## Quaternions

- GLM order: `(w, x, y, z)`; `w` is the scalar component.
- Identity quaternion: `(1, 0, 0, 0)`.
- Use normalized quaternions for rotations. Normalize values after algorithms that may accumulate drift.

## Clip depth

The default public header defines `GLM_FORCE_DEPTH_ZERO_TO_ONE`, making GLM projection helpers Vulkan-friendly with a [0, 1] depth interval. OpenGL's conventional clip-depth interval is [-1, 1]; OpenGL clients must use a documented conversion or an explicit projection implementation. Do not treat the macro as a universal API-independent choice.

## GLM configuration

All GLM configuration macros must be defined before including any GLM header and must be identical across translation units. Consumers should include `<aryabhatta/math/math.hpp>` first and must not independently toggle handedness, depth, constructor initialization, or quaternion-related configuration.
