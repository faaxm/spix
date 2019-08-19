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

find_library(AnyRPCRelease_LIBRARIES 
  NAMES
    anyrpc
  HINTS
    ${AnyRPC_LIBRARY_DIR_HINT}/Release/lib)

find_library(AnyRPCDebug_LIBRARIES 
  NAMES
    anyrpc
  HINTS
    ${AnyRPC_LIBRARY_DIR_HINT}/Debug/lib)

if(${AnyRPC_VERBOSE_FIND})
    message(STATUS "AnyRPC libraries: ${AnyRPCRelease_LIBRARIES} ${AnyRPCDebug_LIBRARIES}")
endif()

find_package_handle_standard_args(AnyRPC
  DEFAULT_MSG
  AnyRPC_INCLUDE_DIRS
  AnyRPCDebug_LIBRARIES
  AnyRPCRelease_LIBRARIES)

mark_as_advanced(AnyRPC_INCLUDE_DIRS
    AnyRPCRelease_LIBRARIES
    AnyRPCDebug_LIBRARIES)

if(ANYRPC_FOUND)
    message(STATUS "AnyRPC found")
    add_library(AnyRPC::anyrpc STATIC IMPORTED)

    set_property(TARGET AnyRPC::anyrpc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE DEBUG RELWITHDEBINFO MINSIZEREL)

    set_target_properties(AnyRPC::anyrpc PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION_RELEASE ${AnyRPCRelease_LIBRARIES}
        IMPORTED_LOCATION_DEBUG ${AnyRPCDebug_LIBRARIES}
        INTERFACE_INCLUDE_DIRECTORIES ${AnyRPC_INCLUDE_DIRS})

    set_target_properties(AnyRPC::anyrpc PROPERTIES
        MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
        MAP_IMPORTED_CONFIG_MINSIZEREL Release)
    
    if(WIN32)
        set_target_properties(AnyRPC::anyrpc PROPERTIES
            IMPORTED_LINK_INTERFACE_LIBRARIES "ws2_32")
    endif()
else()
    message(FATAL_ERROR "AnyRPC not found")
endif()