#
# Spix QtQuick Library
#
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#
# Dependencies
#
find_package(Qt${SPIX_QT_MAJOR}
    COMPONENTS
        Core
        Gui
        Quick
    REQUIRED)
message("Building Spix QtQuick for Qt ${SPIX_QT_MAJOR}")

#
# Sources
#
file(GLOB HEADERS_PUB_ROOT "include/Spix/*.h")
set(HEADERS_PUB ${HEADERS_PUB_ROOT})

set(SOURCES
    ${HEADERS_PUB}
    src/QtQmlBot.cpp

    src/FindQtItem.cpp
    src/FindQtItem.h
    src/QtEvents.cpp
    src/QtEvents.h
    src/QtItem.cpp
    src/QtItem.h
    src/QtItemTools.cpp
    src/QtItemTools.h
    src/QtScene.cpp
    src/QtScene.h

    src/Utils/QtEventRecorder.cpp
    src/Utils/QtEventRecorder.h
    src/Utils/DebugDump.cpp
    src/Utils/DebugDump.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

#
# Qt MOC Files
#
cmake_language(CALL "qt${SPIX_QT_MAJOR}_wrap_cpp" MOC_FILES "include/Spix/QtQmlBot.h")

#
# Target
#
add_library(SpixQtQuick ${SOURCES} ${MOC_FILES})
target_include_directories(SpixQtQuick
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        src
)
target_link_libraries(SpixQtQuick
    PUBLIC
        Spix::Core
        Qt${SPIX_QT_MAJOR}::Core
        Qt${SPIX_QT_MAJOR}::Gui
        Qt${SPIX_QT_MAJOR}::Quick
)

#
# Export headers
#
include(GenerateExportHeader)
generate_export_header(SpixQtQuick
    EXPORT_FILE_NAME "Spix/spix_qtquick_export.h"
)

#
# Install
#
install(
    TARGETS SpixQtQuick
    EXPORT SpixQtQuickTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/Spix DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Spix/spix_qtquick_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Spix)

set(SPIX_QTQUICK_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/SpixQtQuick/cmake")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SpixQtQuickConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SpixQtQuickConfig.cmake"
    INSTALL_DESTINATION "${SPIX_QTQUICK_CMAKE_CONFIG_DIR}"
)

install(
    EXPORT SpixQtQuickTargets
    FILE SpixQtQuickTargets.cmake
    DESTINATION ${SPIX_QTQUICK_CMAKE_CONFIG_DIR}
    NAMESPACE Spix::
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SpixQtQuickConfig.cmake"
    DESTINATION ${SPIX_QTQUICK_CMAKE_CONFIG_DIR}
)

export(
    EXPORT SpixQtQuickTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/SpixQtQuickTargets.cmake"
    NAMESPACE Spix::
)

# Alias for consistency
add_library(Spix::QtQuick ALIAS SpixQtQuick)