# Main libs CMakeLists.txt for modular Spix libraries

add_subdirectory(Core)

if(SPIX_BUILD_QTQUICK)
    add_subdirectory(Scenes/QtQuick)
endif()

if(SPIX_BUILD_QTWIDGETS)
    add_subdirectory(Scenes/QtWidgets)
endif()
