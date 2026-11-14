include(FetchContent)

find_package(glm CONFIG QUIET)

if(NOT glm_FOUND)
    if(NOT ARYABHATTA_FETCH_GLM)
        message(FATAL_ERROR
            "GLM was not found. Install GLM or enable ARYABHATTA_FETCH_GLM."
        )
    endif()

    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
    )

    set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glm)

    if(NOT TARGET glm::glm)
        add_library(glm::glm ALIAS glm)
    endif()
endif()
