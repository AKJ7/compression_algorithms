cmake_minimum_required(VERSION 3.11)

project(Compression_algorithms
        VERSION 1.0
        DESCRIPTION "Compilation of some Compression algorithms"
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

option(ENABLE_TESTING "Enable Testing" ON)

macro(title_message ALGORITHMUS_NAME)
    message(STATUS "${ALGORITHMUS_NAME} algorithmus found")
endmacro()

if (ENABLE_TESTING)
    message(STATUS "Tests enabled")
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.8.1
    )

    FetchContent_GetProperties(googletest)
    if (NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    macro(package_add_test TESTNAME)
        add_executable(${TESTNAME} ${ARGN})
        foreach(val ${ARGN})
            if (NOT EXISTS ${val})
                message(FATAL_ERROR "'${val}' was not found. Make sure to follow the directory structure:
                - algorithmName:
                -    src/algorithmName.h
                -    test/test.cpp")
            endif()
        endforeach()
        target_link_libraries(${TESTNAME} gtest gmock gtest_main)
        add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
        set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
    endmacro()
endif()


function(get_algorithms_folders)
    file(GLOB RESULT
            LIST_DIRECTORIES true
            RELATIVE ${PROJECT_SOURCE_DIR}
            ${PROJECT_SOURCE_DIR}/*)
    set(DIR_LIST "")
    set(DIR_EXCLUDE_LIST
            .idea
            .git
            bin
            build
            lib
            )
    foreach(child ${RESULT})
        if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${child} AND NOT ${child} IN_LIST DIR_EXCLUDE_LIST)
            LIST(APPEND DIR_LIST ${child})
        endif()
    endforeach()
    set(TO_RETURN ${DIR_LIST} PARENT_SCOPE)
endfunction()


get_algorithms_folders()

foreach(value ${TO_RETURN})
    message(STATUS "${value} Algorithm found")
    package_add_test(${value} ${PROJECT_SOURCE_DIR}/${value}/src/${value}.h ${PROJECT_SOURCE_DIR}/${value}/test/test.cpp)
endforeach()