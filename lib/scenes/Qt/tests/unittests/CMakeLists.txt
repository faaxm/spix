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
    ../unittests_main.cpp
    Scene/QtItem_test.cpp
    Scene/QtItemTools_test.cpp
    Scene/QtTestUtils.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${TEST_SOURCES})

add_executable(SpixQt_test ${TEST_SOURCES})
target_include_directories(SpixQt_test PRIVATE ../../src)
target_link_libraries(SpixQt_test SpixQtScene AnyRPC::anyrpc GTest::GTest Qt${SPIX_QT_MAJOR}::Test)
add_test( NAME SpixQtUnitTests COMMAND SpixQt_test ) 