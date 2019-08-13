add_library(Spix::Spix UNKNOWN IMPORTED)

set_target_properties(Spix::Spix PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
  IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/spix.lib
  INTERFACE_INCLUDE_DIRECTORIES include
)

if(WIN32)
    set_target_properties(Spix::Spix PROPERTIES
        IMPORTED_LINK_INTERFACE_LIBRARIES "ws2_32"
    )
endif()
