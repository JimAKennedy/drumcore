cmake_minimum_required(VERSION 3.14)

project(drumcore
    VERSION 0.1.0
    DESCRIPTION "Shared drum pattern data structures for VST3 plugins"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only INTERFACE library
add_library(drumcore INTERFACE)
target_include_directories(drumcore INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Tests
option(DRUMCORE_BUILD_TESTS "Build drumcore tests" ON)
if(DRUMCORE_BUILD_TESTS AND CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(drumcore_tests
        tests/drumgrid_test.cpp
        tests/drummapping_test.cpp
        tests/timesignature_test.cpp
        tests/lockfreequeue_test.cpp
        tests/constants_test.cpp
        tests/seed_test.cpp
    )

    target_link_libraries(drumcore_tests PRIVATE
        drumcore
        gtest
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(drumcore_tests)
endif()
