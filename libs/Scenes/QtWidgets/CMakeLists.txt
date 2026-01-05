#
# Spix QtWidgets Library
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
        Widgets
    REQUIRED)
message("Building Spix QtWidgets for Qt ${SPIX_QT_MAJOR}")

#
# Sources
#
file(GLOB HEADERS_PUB_ROOT "include/Spix/*.h")
set(HEADERS_PUB ${HEADERS_PUB_ROOT})

set(SOURCES
    ${HEADERS_PUB}
    src/QtWidgetsBot.cpp

    src/FindQtWidget.cpp
    src/FindQtWidget.h
    src/QtWidgetsEvents.cpp
    src/QtWidgetsEvents.h
    src/QtWidgetsItem.cpp
    src/QtWidgetsItem.h
    src/QtWidgetsItemTools.cpp
    src/QtWidgetsItemTools.h
    src/QtWidgetsScene.cpp
    src/QtWidgetsScene.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

#
# Qt MOC Files
#
cmake_language(CALL "qt${SPIX_QT_MAJOR}_wrap_cpp" MOC_FILES "include/Spix/QtWidgetsBot.h")

#
# Target
#
add_library(SpixQtWidgets ${SOURCES} ${MOC_FILES})
target_include_directories(SpixQtWidgets
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        src
)
target_link_libraries(SpixQtWidgets
    PUBLIC
        Spix::Core
        Qt${SPIX_QT_MAJOR}::Core
        Qt${SPIX_QT_MAJOR}::Gui
        Qt${SPIX_QT_MAJOR}::Widgets
)

#
# Export headers
#
include(GenerateExportHeader)
generate_export_header(SpixQtWidgets
    EXPORT_FILE_NAME "Spix/spix_qtwidgets_export.h"
)

#
# Install
#
install(
    TARGETS SpixQtWidgets
    EXPORT SpixQtWidgetsTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/Spix DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Spix/spix_qtwidgets_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Spix)

set(SPIX_QTWIDGETS_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/SpixQtWidgets/cmake")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SpixQtWidgetsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SpixQtWidgetsConfig.cmake"
    INSTALL_DESTINATION "${SPIX_QTWIDGETS_CMAKE_CONFIG_DIR}"
)

install(
    EXPORT SpixQtWidgetsTargets
    FILE SpixQtWidgetsTargets.cmake
    DESTINATION ${SPIX_QTWIDGETS_CMAKE_CONFIG_DIR}
    NAMESPACE Spix::
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SpixQtWidgetsConfig.cmake"
    DESTINATION ${SPIX_QTWIDGETS_CMAKE_CONFIG_DIR}
)

export(
    EXPORT SpixQtWidgetsTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/SpixQtWidgetsTargets.cmake"
    NAMESPACE Spix::
)

# Alias for consistency
add_library(Spix::QtWidgets ALIAS SpixQtWidgets)
