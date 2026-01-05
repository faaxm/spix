cmake_minimum_required(VERSION 3.18)
project(Spix LANGUAGES CXX)

option(SPIX_BUILD_EXAMPLES "Build Spix examples." ON)
option(SPIX_BUILD_TESTS "Build Spix unit tests." OFF)
option(SPIX_BUILD_QTQUICK "Build the QtQuick scene library." ON)
option(SPIX_BUILD_QTWIDGETS "Build the QtWidgets scene library." OFF)
set(SPIX_QT_MAJOR "6" CACHE STRING "Major Qt version to build Spix against")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
set(CMAKE_CXX_STANDARD 17)

# Hide symbols unless explicitly flagged with SPIX_EXPORT
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_subdirectory(libs)

if(SPIX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SPIX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(libs/Core/tests)
    if(SPIX_BUILD_QTQUICK)
        add_subdirectory(libs/Scenes/QtQuick/tests)
    endif()
    if(SPIX_BUILD_QTWIDGETS)
        add_subdirectory(libs/Scenes/QtWidgets/tests)
    endif()
endif()

#
# Install main Spix config
#
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SpixConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SpixConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/Spix/cmake"
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SpixConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/Spix/cmake"
)
