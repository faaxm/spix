#
# Spix QtWidgets Tests
#
find_package(GTest REQUIRED)
find_package(Qt${SPIX_QT_MAJOR} COMPONENTS Test Widgets REQUIRED)

set(QTWIDGETS_TEST_SOURCES
    unittests_main.cpp
    QtWidgetsItemTools_test.cpp
    QtWidgetsItem_test.cpp
    QtWidgetsTestUtils.h
)

add_executable(SpixQtWidgetsTests ${QTWIDGETS_TEST_SOURCES})
target_link_libraries(SpixQtWidgetsTests
    PRIVATE
        Spix::QtWidgets
        Qt${SPIX_QT_MAJOR}::Test
        Qt${SPIX_QT_MAJOR}::Widgets
        GTest::gtest
        GTest::gmock
)

target_include_directories(SpixQtWidgetsTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

add_test(NAME SpixQtWidgetsTests COMMAND SpixQtWidgetsTests)
set_tests_properties(SpixQtWidgetsTests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
