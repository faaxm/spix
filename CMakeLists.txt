cmake_minimum_required(VERSION 3.3)
project(Spix LANGUAGES CXX)

option(SPIX_BUILD_EXAMPLES "Build Spix examples." ON)
option(SPIX_BUILD_TESTS "Build Spix unit tests." OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib)

if(SPIX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SPIX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(lib/tests)
endif()


include(GNUInstallDirs)

#
# SpixConfig.cmake File
#
set(SPIX_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/Spix/cmake")

include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/SpixConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/SpixConfig.cmake"
    INSTALL_DESTINATION "${SPIX_CMAKE_CONFIG_DIR}"
)

#
# Install
#
install(
    TARGETS Spix
    EXPORT SpixTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY lib/include/Spix DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT SpixTargets
    FILE SpixTargets.cmake
    DESTINATION ${SPIX_CMAKE_CONFIG_DIR}
    NAMESPACE Spix::
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SpixConfig.cmake"
    DESTINATION ${SPIX_CMAKE_CONFIG_DIR}
)
