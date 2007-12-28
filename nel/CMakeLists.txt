#-----------------------------------------------------------------------------
# Load some macros.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
INCLUDE(CMakeModules/opennel.cmake)

# Force out of source builds.
CHECK_OUT_OF_SOURCE()

IF(WIN32)
  SET(CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../3rdParty/include;${CMAKE_INCLUDE_PATH}")
  SET(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../3rdParty/lib;${CMAKE_INCLUDE_PATH}")
ENDIF(WIN32)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(OpenNeL CXX C)
SET(NL_VERSION "0.6.0")

#-----------------------------------------------------------------------------
# Redirect output files
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

#-----------------------------------------------------------------------------
# Set default config options
#
NL_SETUP_DEFAULT_OPTIONS()

#-----------------------------------------------------------------------------
# Override default options
###
# Features
###
OPTION(WITH_COVERAGE		"With Code Coverage Support"			ON )

###
# Core libraries
###

###
# Drivers Support
###

###
# Optional support
###
OPTION(WITH_CEGUI	"Build CEGUI Renderer"					OFF)
OPTION(WITH_TOOLS	"Build NeL Tools"					OFF)
OPTION(WITH_SAMPLES	"Build NeL Samples"					ON )
OPTION(WITH_TESTS	"Build NeL Unit Tests"					OFF)
OPTION(WITH_GTK		"With GTK Support"					OFF)

#-----------------------------------------------------------------------------
#Platform specifics

FIND_PACKAGE(Threads)
FIND_PACKAGE(LibXml2)
FIND_PACKAGE(OpenAL)

NL_SETUP_BUILD()

IF(WITH_3D)
  FIND_PACKAGE(FreeType)

  IF(WITH_DRIVER_OPENGL)
    FIND_PACKAGE(OpenGL)
  ENDIF(WITH_DRIVER_OPENGL)

#  IF(WITH_CEGUI)
#    INCLUDE(${CMAKE_SOURCE_DIR}/cmake/FindCEGUI.cmake)
#  ENDIF(WITH_CEGUI)

  SET(JPEG_LIB jpeg)
ENDIF(WITH_3D)

IF(WITH_GTK)
  FIND_PACKAGE(GTK2)
ENDIF(WITH_GTK)

IF(UNIX)

  INCLUDE_DIRECTORIES(/usr/include /usr/local/include)
  
  SET(prefix ${CMAKE_INSTALL_PREFIX})
  SET(exec_prefix ${CMAKE_INSTALL_PREFIX}/bin)
  SET(libdir ${CMAKE_INSTALL_PREFIX}/lib)
  SET(includedir ${CMAKE_INSTALL_PREFIX}/include)
  SET(enable_net ${WITH_NET})
  SET(enable_3d ${WITH_3D})
  SET(enable_pacs ${WITH_PACS})
  SET(enable_sound ${WITH_SOUND})
  CONFIGURE_FILE(nel-config.in nel-config)

  INSTALL(PROGRAMS ${CMAKE_BINARY_DIR}/nel-config DESTINATION bin)

ENDIF(UNIX)

IF(WIN32)
  INCLUDE(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)
  
  SET(WINSOCK2_LIB ws2_32.lib)
  
  #INCLUDE_DIRECTORIES(${DEPENDSDIR})
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

## None
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${NL_NONE_CFLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NL_NONE_CFLAGS} ${PLATFORM_CFLAGS} ")

## Debug
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${NL_DEBUG_CFLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${NL_DEBUG_CFLAGS} ${PLATFORM_CFLAGS} ")

## Release
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${NL_RELEASE_CFLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${NL_RELEASE_CFLAGS} ${PLATFORM_CFLAGS} ")

## RelWithDebInfo
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${NL_RELEASEDEBUG_CFLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${NL_RELEASEDEBUG_CFLAGS} ${PLATFORM_CFLAGS} ")

## MinSizeRel
SET(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${NL_RELEASEDEBUG_CFLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${NL_RELEASEDEBUG_CFLAGS} ${PLATFORM_CFLAGS} ")

SUBDIRS(src include)

IF(WITH_SAMPLES)
  SUBDIRS(samples)
ENDIF(WITH_SAMPLES)

IF(WITH_TOOLS)
  SUBDIRS(tools)
ENDIF(WITH_TOOLS)
