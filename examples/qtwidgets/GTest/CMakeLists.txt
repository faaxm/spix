set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt${SPIX_QT_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
find_package(GTest REQUIRED)

add_executable(SpixQtWidgetsGTestExample "main.cpp")
target_compile_definitions(SpixQtWidgetsGTestExample
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(SpixQtWidgetsGTestExample
    PRIVATE
        Qt${SPIX_QT_MAJOR}::Core
        Qt${SPIX_QT_MAJOR}::Gui
        Qt${SPIX_QT_MAJOR}::Widgets
        GTest::GTest
        Spix::QtWidgets)
