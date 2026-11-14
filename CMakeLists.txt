cmake_minimum_required(VERSION 3.20)
project(Aryabhatta VERSION 0.1.0 LANGUAGES CXX)

option(ARYABHATTA_BUILD_TESTS "Build Aryabhatta unit tests" OFF)
option(ARYABHATTA_FETCH_GLM "Fetch GLM when no package is available" ON)

include(cmake/Dependencies.cmake)

add_library(aryabhatta_math INTERFACE)
add_library(Aryabhatta::Math ALIAS aryabhatta_math)
target_compile_features(aryabhatta_math INTERFACE cxx_std_20)
target_include_directories(aryabhatta_math INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(aryabhatta_math INTERFACE glm::glm)

if(ARYABHATTA_BUILD_TESTS)
    enable_testing()
    find_package(Catch2 3 CONFIG QUIET)
    if(NOT Catch2_FOUND)
        include(FetchContent)
        FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1
            GIT_SHALLOW TRUE)
        set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
        set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(Catch2)
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    endif()
    add_executable(aryabhatta_math_tests
        tests/math/test_math.cpp)
    target_link_libraries(aryabhatta_math_tests PRIVATE
        Aryabhatta::Math Catch2::Catch2WithMain)
    include(Catch)
    catch_discover_tests(aryabhatta_math_tests)
endif()

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS aryabhatta_math EXPORT AryabhattaMathTargets)
install(EXPORT AryabhattaMathTargets
    NAMESPACE Aryabhatta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AryabhattaMath)
