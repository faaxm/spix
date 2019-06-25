set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
find_package(GTest REQUIRED)

add_executable(SpixGTestExample "main.cpp" "qml.qrc")
target_compile_definitions(SpixGTestExample PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(SpixGTestExample PRIVATE Qt5::Core Qt5::Quick GTest::GTest Spix)
