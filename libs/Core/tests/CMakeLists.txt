#
# Spix Core Tests
#
find_package(GTest REQUIRED)
find_package(AnyRPC REQUIRED)

set(CORE_TEST_SOURCES
    unittests_main.cpp
    CommandExecuter/CommandExecuter_test.cpp
    CommandExecuter/ExecuterState_test.cpp
    Commands/ClickOnItem_test.cpp
    Commands/DropFromExt_test.cpp
    Commands/GetProperty_test.cpp
    Data/ItemPathComponent_test.cpp
    Data/ItemPath_test.cpp
    Data/ItemPosition_test.cpp
    Data/PasteboardContent_test.cpp
    Utils/AnyRpcFunction_test.cpp
    Utils/AnyRpcUtils_test.cpp
    Utils/PathParser_test.cpp
)

add_executable(SpixCoreTests ${CORE_TEST_SOURCES})
target_link_libraries(SpixCoreTests
    PRIVATE
        Spix::Core
        GTest::gtest
        GTest::gmock
        AnyRPC::anyrpc
)

target_include_directories(SpixCoreTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_test(NAME SpixCoreTests COMMAND SpixCoreTests)
