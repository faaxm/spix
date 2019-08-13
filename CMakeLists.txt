cmake_minimum_required(VERSION 3.3)
project(Spix LANGUAGES CXX)

option(SPIX_BUILD_EXAMPLES "Build Spix examples." ON)
option(SPIX_BUILD_TESTS "Build Spix unit tests." OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 14)

set(ARTIFACTS_DIR "${CMAKE_BINARY_DIR}/artifacts")

add_subdirectory(lib)

if(SPIX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SPIX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(lib/tests)
endif()

install(FILES cmake/install_resources/Spix.cmake
    DESTINATION .)

include(cmake/spix_cpack.cmake)
