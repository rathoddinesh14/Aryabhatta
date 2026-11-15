cmake_minimum_required(VERSION 3.20)

project(Aryabhatta
    VERSION 0.1.0
    DESCRIPTION "A modular, reusable C++ mathematics library"
    LANGUAGES CXX
)

option(ARYABHATTA_BUILD_TESTS "Build Aryabhatta unit tests" OFF)
option(ARYABHATTA_FETCH_GLM "Fetch GLM when no package is available" ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(cmake/Dependencies.cmake)

add_library(aryabhatta_math INTERFACE)
add_library(Aryabhatta::Math ALIAS aryabhatta_math)

# Export the installed target as Aryabhatta::Math.
set_target_properties(aryabhatta_math PROPERTIES
    EXPORT_NAME Math
)

target_compile_features(aryabhatta_math INTERFACE cxx_std_20)

target_include_directories(aryabhatta_math
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(aryabhatta_math
    INTERFACE
        glm::glm
)

if(ARYABHATTA_BUILD_TESTS)
    enable_testing()

    find_package(Catch2 3 CONFIG QUIET)
    if(NOT Catch2_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1
            GIT_SHALLOW TRUE
        )
        set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
        set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(Catch2)
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    endif()

    add_executable(aryabhatta_math_tests
        tests/math/test_math.cpp
        tests/math/test_glm_integration.cpp
        tests/math/test_math_helpers.cpp
        tests/math/test_double_precision.cpp
    )
    target_link_libraries(aryabhatta_math_tests
        PRIVATE
            Aryabhatta::Math
            Catch2::Catch2WithMain
    )

    include(Catch)
    catch_discover_tests(aryabhatta_math_tests)
endif()

install(
    TARGETS aryabhatta_math
    EXPORT AryabhattaMathTargets
)

install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    EXPORT AryabhattaMathTargets
    FILE AryabhattaMathTargets.cmake
    NAMESPACE Aryabhatta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Aryabhatta
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AryabhattaConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AryabhattaConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Aryabhatta"
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/AryabhattaConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/AryabhattaConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/AryabhattaConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Aryabhatta
)
