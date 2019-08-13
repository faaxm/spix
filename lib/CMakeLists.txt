#
# Dependencies
#
find_package(Threads REQUIRED)
find_package(Qt5
    COMPONENTS
        Core
        GUI
        Quick
    REQUIRED)
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
    src/QtQmlBot.cpp
    src/TestServer.cpp

    src/Commands/ClickOnItem.cpp
    src/Commands/ClickOnItem.h
    src/Commands/Command.cpp
    src/Commands/Command.h
    src/Commands/CustomCmd.cpp
    src/Commands/CustomCmd.h
    src/Commands/DragBegin.cpp
    src/Commands/DragBegin.h
    src/Commands/DragEnd.cpp
    src/Commands/DragEnd.h
    src/Commands/DropFromExt.cpp
    src/Commands/DropFromExt.h
    src/Commands/ExistsAndVisible.cpp
    src/Commands/ExistsAndVisible.h
    src/Commands/GetProperty.cpp
    src/Commands/GetProperty.h
    src/Commands/GetTestStatus.cpp
    src/Commands/GetTestStatus.h
    src/Commands/InputText.cpp
    src/Commands/InputText.h
    src/Commands/Quit.cpp
    src/Commands/Quit.h
    src/Commands/Screenshot.cpp
    src/Commands/Screenshot.h
    src/Commands/Wait.cpp
    src/Commands/Wait.h

    src/CommandExecuter/CommandEnvironment.cpp
    src/CommandExecuter/CommandEnvironment.h
    src/CommandExecuter/CommandExecuter.cpp
    src/CommandExecuter/CommandExecuter.h
    src/CommandExecuter/ExecuterState.cpp
    src/CommandExecuter/ExecuterState.h

    src/Data/Geometry.cpp
    src/Data/ItemPath.cpp
    src/Data/ItemPosition.cpp
    src/Data/PasteboardContent.cpp

    src/Scene/Events.h
    src/Scene/Item.h
    src/Scene/Mock/MockEvents.cpp
    src/Scene/Mock/MockEvents.h
    src/Scene/Mock/MockScene.cpp
    src/Scene/Mock/MockScene.h
    src/Scene/Mock/MockItem.cpp
    src/Scene/Mock/MockItem.h
    src/Scene/Qt/QtEvents.cpp
    src/Scene/Qt/QtEvents.h
    src/Scene/Qt/QtItem.cpp
    src/Scene/Qt/QtItem.h
    src/Scene/Qt/QtItemTools.cpp
    src/Scene/Qt/QtItemTools.h
    src/Scene/Qt/QtScene.cpp
    src/Scene/Qt/QtScene.h
    src/Scene/Scene.h

    src/Utils/AnyRpcUtils.h
    src/Utils/DebugDump.cpp
    src/Utils/DebugDump.h
    src/Utils/QtEventRecorder.cpp
    src/Utils/QtEventRecorder.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})


#
# Qt MOC Files
#
qt5_wrap_cpp(MOC_FILES "include/Spix/QtQmlBot.h")


#
# Target
#
add_library(Spix ${SOURCES} ${MOC_FILES})
target_include_directories(Spix
    PUBLIC
        include
    PRIVATE
        src
)
target_link_libraries(Spix
    PUBLIC
        Threads::Threads
        Qt5::Core
        Qt5::Gui
        Qt5::Quick
        AnyRPC::anyrpc
)

#
# Install
#
install(
    TARGETS Spix
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/Spix DESTINATION include)
