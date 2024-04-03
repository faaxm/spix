cmake_minimum_required(VERSION 3.16)

project(QtTestExample VERSION 1.0 LANGUAGES CXX)

enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(AnyRPC REQUIRED)
find_package(Qt${SPIX_QT_MAJOR} REQUIRED COMPONENTS Test Quick)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${AnyRPC_INCLUDE_DIRS})

add_executable(QtTestExample tst_test.cpp)
add_test(NAME QtTestExample COMMAND QtTestExample)

target_link_libraries(QtTestExample PRIVATE
    Qt${SPIX_QT_MAJOR}::Test    
    Spix

    PRIVATE
        AnyRPC::anyrpc
)

