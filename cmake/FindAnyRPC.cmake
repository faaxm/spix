include(FindPackageHandleStandardArgs)

set(AnyRPC_INCLUDE_DIR_HINT "" CACHE PATH "AnyRPC include dir")
set(AnyRPC_LIBRARY_DIR_HINT "" CACHE PATH "AnyRPC library dir")
set(AnyRPC_VERBOSE_FIND ON CACHE BOOL "AnyRPC verbosisty")

find_path(AnyRPC_INCLUDE_DIRS
  anyrpc/anyrpc.h
  HINTS
    ${AnyRPC_INCLUDE_DIR_HINT})

if(${AnyRPC_VERBOSE_FIND})
  message(STATUS "AnyRPC include dir : ${AnyRPC_INCLUDE_DIRS}")
endif()

find_library(AnyRPC_LIBRARIES 
  NAMES
    anyrpc
  HINTS
    ${AnyRPC_LIBRARY_DIR_HINT})

if(${AnyRPC_VERBOSE_FIND})
    message(STATUS "AnyRPC libraries dir : ${AnyRPC_LIBRARIES}")
endif()

find_package_handle_standard_args(AnyRPC
  DEFAULT_MSG
  AnyRPC_INCLUDE_DIRS
  AnyRPC_LIBRARIES)

mark_as_advanced(AnyRPC_INCLUDE_DIRS
  AnyRPC_LIBRARIES)

if( ANYRPC_FOUND )
  if( NOT TARGET AnyRPC::anyrpc )
    add_library(AnyRPC::anyrpc UNKNOWN IMPORTED)

    set_target_properties(AnyRPC::anyrpc PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
      IMPORTED_LOCATION ${AnyRPC_LIBRARIES}
      INTERFACE_INCLUDE_DIRECTORIES ${AnyRPC_INCLUDE_DIRS}
    )
    
    if (WIN32)
      set_target_properties(AnyRPC::anyrpc PROPERTIES
        IMPORTED_LINK_INTERFACE_LIBRARIES "ws2_32"
      )
    endif()
  endif()
endif()
