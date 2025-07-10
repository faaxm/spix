find_package(GTest REQUIRED)
find_package(AnyRPC REQUIRED)
find_package(Qt${SPIX_QT_MAJOR}
    COMPONENTS
        Core
        Gui
        Quick
        Test
    REQUIRED)


set(TEST_SOURCES
    unittests_main.cpp
    unittests/Commands/ClickOnItem_test.cpp
    unittests/Commands/DropFromExt_test.cpp
    unittests/Commands/GetProperty_test.cpp

    unittests/CommandExecuter/CommandExecuter_test.cpp
    unittests/CommandExecuter/ExecuterState_test.cpp

    unittests/Data/ItemPath_test.cpp
    unittests/Data/ItemPathComponent_test.cpp
    unittests/Data/ItemPosition_test.cpp
    unittests/Data/PasteboardContent_test.cpp

    unittests/Scene/Qt/QtItem_test.cpp
    unittests/Scene/Qt/QtItemTools_test.cpp
    unittests/Scene/Qt/QtTestUtils.h
    unittests/Scene/Qt/PickClick_test.cpp

    unittests/Utils/AnyRpcFunction_test.cpp
    unittests/Utils/AnyRpcUtils_test.cpp
    unittests/Utils/PathParser_test.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${TEST_SOURCES})

add_executable(Spix_test ${TEST_SOURCES})
target_include_directories(Spix_test PRIVATE ../src)
target_link_libraries(Spix_test Spix AnyRPC::anyrpc GTest::GTest Qt${SPIX_QT_MAJOR}::Test)
add_test( NAME SpixUnitTests COMMAND Spix_test )
