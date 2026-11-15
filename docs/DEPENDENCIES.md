# Dependencies

## GLM

GLM is the math backend for the initial foundational math API.

- Version: 1.0.1
- Upstream: https://github.com/g-truc/glm
- Integration: CMake package discovery, with a FetchContent fallback.

GLM is not vendored into this repository.

## Catch2

Catch2 v3 is used for unit testing.

- Version: 3.7.1
- Upstream: https://github.com/catchorg/Catch2
- Integration: used only when `ARYABHATTA_BUILD_TESTS=ON`.

Catch2 is a development dependency and is not linked to consumer targets.

## Dependency policy

- Keep the runtime dependency set small.
- Pin fetched dependencies to explicit versions or tags.
- Do not expose test-only dependencies through public targets.
- Review licenses and maintenance status before adding dependencies.

## Downstream Dependency Requirement

GLM is a public dependency of Aryabhatta.

The installed package configuration calls:

    find_dependency(glm CONFIG)

Therefore, consumers using `find_package(Aryabhatta CONFIG REQUIRED)`
must have GLM installed and available through their CMake package
search paths.

`ARYABHATTA_FETCH_GLM` only helps while configuring/building Aryabhatta.
It does not bundle or install GLM as part of the Aryabhatta package.

This is intentional: Aryabhatta does not vendor or duplicate GLM.
