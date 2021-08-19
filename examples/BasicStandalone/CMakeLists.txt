cmake_minimum_required(VERSION 3.12)
project(SpixStandaloneExample LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Dependencies
#
find_package(Threads REQUIRED)
find_package(AnyRPC REQUIRED)
find_package(Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Spix CONFIG REQUIRED)

add_executable(SpixStandaloneExample "main.cpp" "qml.qrc")
target_compile_definitions(SpixStandaloneExample PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(SpixStandaloneExample PRIVATE Threads::Threads Qt5::Core Qt5::Quick Spix::Spix)
