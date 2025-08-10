# Examples CMakeLists.txt - Conditional example building based on available components

if(SPIX_BUILD_QTQUICK)
    add_subdirectory(qtquick)
endif()
