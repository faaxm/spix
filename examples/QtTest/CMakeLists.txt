cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0099 NEW)
cmake_policy(SET CMP0020 NEW)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(QtTestExample VERSION 0.1 LANGUAGES CXX)

enable_testing()

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

find_package(Qt${SPIX_QT_MAJOR} REQUIRED COMPONENTS Test Quick)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0001 NEW)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${AnyRPC_INCLUDE_DIRS})

add_executable(QtTestExample tst_test.cpp)
add_test(NAME QtTestExample COMMAND QtTestExample)

target_link_libraries(QtTestExample PRIVATE
    Qt${SPIX_QT_MAJOR}::Test
    Spix
)

