#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Spix::Spix" for configuration "Debug"
set_property(TARGET Spix::Spix APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Spix::Spix PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libSpix.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS Spix::Spix )
list(APPEND _IMPORT_CHECK_FILES_FOR_Spix::Spix "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libSpix.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
