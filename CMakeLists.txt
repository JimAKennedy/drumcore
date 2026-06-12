cmake_minimum_required(VERSION 3.14)

project(drumcore
    VERSION 0.1.0
    DESCRIPTION "Shared drum pattern data structures for VST3 plugins"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate version header from project(VERSION)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/drumcore/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/drumcore/version.h
    @ONLY
)

# Header-only INTERFACE library
add_library(drumcore INTERFACE)
target_include_directories(drumcore INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Install rules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS drumcore EXPORT drumcoreTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PATTERN "*.in" EXCLUDE
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT drumcoreTargets
    FILE drumcoreTargets.cmake
    NAMESPACE drumcore::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/drumcore
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/drumcore-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/drumcore-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/drumcore
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/drumcore-config-version.cmake
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/drumcore-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/drumcore-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/drumcore
)

# Tests
option(DRUMCORE_BUILD_TESTS "Build drumcore tests" ON)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/jk_warnings.cmake)

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
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(drumcore_tests
        tests/constants_test.cpp
        tests/denormalguard_test.cpp
        tests/drumgrid_test.cpp
        tests/drummapping_test.cpp
        tests/genremapper_test.cpp
        tests/lockfreequeue_test.cpp
        tests/seed_test.cpp
        tests/timesignature_test.cpp
        tests/version_test.cpp
    )

    target_link_libraries(drumcore_tests PRIVATE
        drumcore
        gtest
        gtest_main
    )

    jk_target_warnings(drumcore_tests)

    include(GoogleTest)
    gtest_discover_tests(drumcore_tests)
endif()
