
# Inicializando...
cmake_minimum_required (VERSION 2.6)

project (Pirates CXX)
set (PROJECT_NAME "Pirates")
set (OUTPUT_NAME "pirates")
set (VERSION "0.0.1")

message ("=== ${PROJECT_NAME} ${VERSION} ===")

set (SRC_DIR src)
set (BIN_DIR bin)

configure_file (
    "${SRC_DIR}/config/config.h.in"
    "${SRC_DIR}/config/config.h"
    )

include (${SRC_DIR}/src_list.txt)

set (CMAKE_CXX_FLAGS "-fPIC -O2")

include_directories (${SRC_DIR})
include_directories (/usr/include/python2.6)

set(CMAKE_OSX_ARCHITECTURES "ppc;i386")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # MACOSX
    include_directories (/Developer/Panda3D/include)
    set (PANDA_LIB_DIR /Developer/Panda3D/lib)
else ()
    include_directories ("/usr/include/panda3d")
    set (PANDA_LIB_DIR /usr/lib/panda3d)
endif ()

add_executable (${OUTPUT_NAME} ${SRC})

find_library (LIB_P3FRAMEWORK p3framework PATH ${PANDA_LIB_DIR})
if (NOT LIB_P3FRAMEWORK)
    message ("Library not found: p3framework")
endif (NOT LIB_P3FRAMEWORK)

find_library (LIB_PANDA panda PATH ${PANDA_LIB_DIR})
if (NOT LIB_PANDA)
    message ("Library not found: panda")
endif (NOT LIB_PANDA)

find_library (LIB_PANDAFX pandafx PATH ${PANDA_LIB_DIR})
if (NOT LIB_PANDAFX)
    message ("Library not found: pandafx")
endif (NOT LIB_PANDAFX)

find_library (LIB_PANDAEXPRESS pandaexpress PATH ${PANDA_LIB_DIR})
if (NOT LIB_PANDAEXPRESS)
    message ("Library not found: pandaexpress")
endif (NOT LIB_PANDAEXPRESS)

find_library (LIB_P3DTOOLCONFIG p3dtoolconfig PATH ${PANDA_LIB_DIR})
if (NOT LIB_P3DTOOLCONFIG)
    message ("Library not found: p3dtoolconfig")
endif (NOT LIB_P3DTOOLCONFIG)

find_library (LIB_P3DTOOL p3dtool PATH ${PANDA_LIB_DIR})
if (NOT LIB_P3DTOOL)
    message ("Library not found: p3dtool")
endif (NOT LIB_P3DTOOL)

find_library (LIB_P3PYSTUB p3pystub PATH ${PANDA_LIB_DIR})
if (NOT LIB_P3PYSTUB)
    message ("Library not found: p3pystub")
endif (NOT LIB_P3PYSTUB)

find_library (LIB_P3DIRECT p3direct PATH ${PANDA_LIB_DIR})
if (NOT LIB_P3DIRECT)
    message ("Library not found: p3direct")
endif (NOT LIB_P3DIRECT)

set (LIBS ${LIB_P3FRAMEWORK} ${LIB_PANDA} ${LIB_PANDAFX} ${LIB_P3DTOOLCONFIG} ${LIB_P3DTOOL}
    ${LIB_P3PYSTUB} ${LIB_P3DIRECT})

set_target_properties (${OUTPUT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set_target_properties (${OUTPUT_NAME} PROPERTIES COMPILE_FLAGS "${CFLAGS}")
target_link_libraries (${OUTPUT_NAME} ${LIBS})






