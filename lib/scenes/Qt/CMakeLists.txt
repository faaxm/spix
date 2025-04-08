#
# Includes
#
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(SPIX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

#
# Dependencies
#
# Assuming SPIX_QT_MAJOR is defined globally or inherited
find_package(Qt${SPIX_QT_MAJOR}
    COMPONENTS
        Core
        Gui
        Quick # Assuming Quick is needed based on lib_core, adjust if not
    REQUIRED)

#
# Sources
#
# Public header
set(HEADERS_PUB
    include/SpixQtScene/QtScene.h
    include/SpixQtScene/QtQmlBot.h
)

# Internal headers
set(HEADERS_PRIV
    src/QtEvents.h
    src/QtItem.h
    src/QtItemTools.h
    src/Utils/DebugDump.h
    src/Utils/QtEventRecorder.h
)

# Source files
set(SOURCES
    src/QtEvents.cpp
    src/QtItem.cpp
    src/QtItemTools.cpp
    src/QtScene.cpp
    src/QtQmlBot.cpp
    src/Utils/DebugDump.cpp
    src/Utils/QtEventRecorder.cpp
    ${HEADERS_PUB} # Include public headers in sources for IDEs
    ${HEADERS_PRIV} # Include private headers in sources for IDEs
)

#
# Qt MOC Files
# We need to process headers with Q_OBJECT
set(MOC_HEADERS
    include/SpixQtScene/QtQmlBot.h
    src/Utils/QtEventRecorder.h
)
# Only call MOC if there are headers that need it
if(MOC_HEADERS)
    cmake_language(CALL "qt${SPIX_QT_MAJOR}_wrap_cpp" MOC_FILES ${MOC_HEADERS})
else()
    set(MOC_FILES "") # Ensure MOC_FILES is defined even if empty
endif()


#
# Target
#
add_library(SpixQtScene ${SOURCES} ${MOC_FILES})
target_include_directories(SpixQtScene
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        src # Private headers and sources in src/
)
target_link_libraries(SpixQtScene
    PUBLIC
        Spix # Link directly against the target name defined in the same project
        Qt${SPIX_QT_MAJOR}::Core
        Qt${SPIX_QT_MAJOR}::Gui
        Qt${SPIX_QT_MAJOR}::Quick
)

#
# Export headers
#
include(GenerateExportHeader)
generate_export_header(SpixQtScene
  EXPORT_FILE_NAME "SpixQtScene/spix_export.h"
)

#
# SpixConfig.cmake File
#
set(SPIX_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/Spix/cmake")

configure_package_config_file(
    cmake/SpixQtSceneConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/SpixQtSceneConfig.cmake"
    INSTALL_DESTINATION "${SPIX_CMAKE_CONFIG_DIR}"
)


#
# Install
#
install(
    TARGETS SpixQtScene
    EXPORT SpixTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/SpixQtScene DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SpixQtScene/spix_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SpixQtScene)
install(
    EXPORT SpixTargets
    FILE SpixTargets.cmake
    DESTINATION ${SPIX_CMAKE_CONFIG_DIR}
    NAMESPACE Spix::
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SpixQtSceneConfig.cmake"
    DESTINATION ${SPIX_CMAKE_CONFIG_DIR}
)

export(
    EXPORT SpixTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/SpixQtSceneTargets.cmake"
    NAMESPACE Spix::
)
