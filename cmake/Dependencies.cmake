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

    FetchContent_GetProperties(glm)

    if(NOT glm_POPULATED)
        FetchContent_Populate(glm)
    endif()

    add_library(glm::glm INTERFACE IMPORTED GLOBAL)

    set_target_properties(glm::glm PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${glm_SOURCE_DIR}"
    )
endif()