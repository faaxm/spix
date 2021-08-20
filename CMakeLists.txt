cmake_minimum_required(VERSION 3.12)
project(Spix LANGUAGES CXX)

option(SPIX_BUILD_EXAMPLES "Build Spix examples." ON)
option(SPIX_BUILD_TESTS "Build Spix unit tests." OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_CXX_STANDARD 14)

# Hide symbols unless explicitly flagged with SPIX_EXPORT
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_subdirectory(lib)

if(SPIX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SPIX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(lib/tests)
endif()
