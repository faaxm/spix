set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt${SPIX_QT_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

add_executable(SpixQtWidgetsFormExample "main.cpp")
target_compile_definitions(SpixQtWidgetsFormExample
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(SpixQtWidgetsFormExample
    PRIVATE
        Qt${SPIX_QT_MAJOR}::Core
        Qt${SPIX_QT_MAJOR}::Gui
        Qt${SPIX_QT_MAJOR}::Widgets
        Spix::QtWidgets)
