# Examples CMakeLists.txt - Conditional example building based on available components

if(SPIX_BUILD_QTQUICK)
    add_subdirectory(qtquick)
endif()

if(SPIX_BUILD_QTWIDGETS)
    add_subdirectory(qtwidgets)
endif()
