#
# Spix Core Library
#
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#
# Dependencies
#
find_package(Threads REQUIRED)
find_package(AnyRPC REQUIRED)

#
# Sources
#
file(GLOB HEADERS_PUB_ROOT "include/Spix/*.h")
file(GLOB HEADERS_PUB_SUB "include/Spix/**/*.h")
set(HEADERS_PUB ${HEADERS_PUB_ROOT} ${HEADERS_PUB_SUB})

set(SOURCES
    ${HEADERS_PUB}
    src/AnyRpcServer.cpp
    src/TestServer.cpp

    src/Commands/ClickOnItem.cpp
    src/Commands/ClickOnItem.h
    src/Commands/Command.cpp
    src/Commands/CustomCmd.cpp
    src/Commands/CustomCmd.h
    src/Commands/DragBegin.cpp
    src/Commands/DragBegin.h
    src/Commands/DragEnd.cpp
    src/Commands/DragEnd.h
    src/Commands/DropFromExt.cpp
    src/Commands/DropFromExt.h
    src/Commands/EnterKey.cpp
    src/Commands/EnterKey.h
    src/Commands/ExistsAndVisible.cpp
    src/Commands/ExistsAndVisible.h
    src/Commands/GetBoundingBox.cpp
    src/Commands/GetBoundingBox.h
    src/Commands/GetProperty.cpp
    src/Commands/GetProperty.h
    src/Commands/GetTestStatus.cpp
    src/Commands/GetTestStatus.h
    src/Commands/InputText.cpp
    src/Commands/InputText.h
    src/Commands/InvokeMethod.cpp
    src/Commands/InvokeMethod.h
    src/Commands/Quit.cpp
    src/Commands/Quit.h
    src/Commands/Screenshot.cpp
    src/Commands/Screenshot.h
    src/Commands/ScreenshotBase64.cpp
    src/Commands/ScreenshotBase64.h
    src/Commands/SetProperty.cpp
    src/Commands/SetProperty.h
    src/Commands/Wait.cpp
    src/Commands/Wait.h
    src/Commands/WaitForItem.cpp
    src/Commands/WaitForItem.h

    src/CommandExecuter/CommandEnvironment.cpp
    src/CommandExecuter/CommandExecuter.cpp
    src/CommandExecuter/ExecuterState.cpp

    src/Data/Geometry.cpp
    src/Data/ItemPath.cpp
    src/Data/ItemPathComponent.cpp
    src/Data/ItemPosition.cpp
    src/Data/PasteboardContent.cpp

    src/Scene/Mock/MockEvents.cpp
    src/Scene/Mock/MockEvents.h
    src/Scene/Mock/MockScene.cpp
    src/Scene/Mock/MockScene.h
    src/Scene/Mock/MockItem.cpp
    src/Scene/Mock/MockItem.h

    src/Utils/AnyRpcUtils.cpp
    src/Utils/AnyRpcUtils.h
    src/Utils/AnyRpcFunction.h
    src/Utils/PathParser.cpp
    src/Utils/PathParser.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

#
# Target
#
add_library(SpixCore ${SOURCES})
target_include_directories(SpixCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        src
)
target_link_libraries(SpixCore
    PUBLIC
        Threads::Threads
    PRIVATE
        AnyRPC::anyrpc
)

#
# Export headers
#
include(GenerateExportHeader)
generate_export_header(SpixCore
    EXPORT_FILE_NAME "Spix/spix_core_export.h"
)

#
# Install
#
install(
    TARGETS SpixCore
    EXPORT SpixCoreTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/Spix DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Spix/spix_core_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Spix)

set(SPIX_CORE_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_DATADIR}/SpixCore/cmake")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../core/cmake/SpixCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SpixCoreConfig.cmake"
    INSTALL_DESTINATION "${SPIX_CORE_CMAKE_CONFIG_DIR}"
)

install(
    EXPORT SpixCoreTargets
    FILE SpixCoreTargets.cmake
    DESTINATION ${SPIX_CORE_CMAKE_CONFIG_DIR}
    NAMESPACE spix::
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SpixCoreConfig.cmake"
    DESTINATION ${SPIX_CORE_CMAKE_CONFIG_DIR}
)

export(
    EXPORT SpixCoreTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/SpixCoreTargets.cmake"
    NAMESPACE spix::
)

# Alias for consistency
add_library(spix::core ALIAS SpixCore)