cmake_minimum_required(VERSION 3.18)
project(Spix LANGUAGES CXX)

option(SPIX_BUILD_EXAMPLES "Build Spix examples." ON)
option(SPIX_BUILD_TESTS "Build Spix unit tests." OFF)
set(SPIX_QT_MAJOR "6" CACHE STRING "Major Qt version to build Spix against")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
set(CMAKE_CXX_STANDARD 17)

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
