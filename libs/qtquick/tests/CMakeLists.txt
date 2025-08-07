#
# Spix QtQuick Tests
#
find_package(GTest REQUIRED)
find_package(Qt${SPIX_QT_MAJOR} COMPONENTS Test REQUIRED)

set(QTQUICK_TEST_SOURCES
    unittests_main.cpp
    Qt/QtItemTools_test.cpp
    Qt/QtItem_test.cpp
    Qt/QtTestUtils.h
)

add_executable(SpixQtQuickTests ${QTQUICK_TEST_SOURCES})
target_link_libraries(SpixQtQuickTests
    PRIVATE
        Spix::QtQuick
        Qt${SPIX_QT_MAJOR}::Test
        GTest::gtest
        GTest::gmock
)

target_include_directories(SpixQtQuickTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_test(NAME SpixQtQuickTests COMMAND SpixQtQuickTests)