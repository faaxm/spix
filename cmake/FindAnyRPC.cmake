include(FindPackageHandleStandardArgs)

find_path(AnyRPC_INCLUDE_DIRS anyrpc/anyrpc.h)
find_library(AnyRPC_LIBRARIES NAMES anyrpc)

find_package_handle_standard_args(AnyRPC DEFAULT_MSG AnyRPC_INCLUDE_DIRS AnyRPC_LIBRARIES )
mark_as_advanced(AnyRPC_INCLUDE_DIRS AnyRPC_LIBRARIES )

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
