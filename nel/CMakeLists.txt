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
SET(WITH_COVERAGE	ON )

###
# Core libraries
###

###
# Drivers Support
###

###
# Optional support
###
SET(WITH_CEGUI		ON )
SET(WITH_TOOLS		OFF)
SET(WITH_SAMPLES	ON )
SET(WITH_TESTS		OFF)
SET(WITH_GTK		OFF)

#-----------------------------------------------------------------------------
#Platform specifics

FIND_PACKAGE(Threads)
FIND_PACKAGE(LibXml2)
NL_SETUP_BUILD()

IF(WITH_3D)
  FIND_PACKAGE(FreeType)

  IF(WITH_DRIVER_OPENGL)
    FIND_PACKAGE(OpenGL)
  ENDIF(WITH_DRIVER_OPENGL)

  IF(WITH_CEGUI)
    FIND_PACKAGE(CEGUI) 
  ENDIF(WITH_CEGUI)

  SET(JPEG_LIB jpeg)
ENDIF(WITH_3D)

IF(WITH_SOUND)
  IF(WITH_DRIVER_OPENAL)
    FIND_PACKAGE(OpenAL)
  ENDIF(WITH_DRIVER_OPENAL)
ENDIF(WITH_SOUND)

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
  
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

NL_SETUP_BUILD_FLAGS()

SUBDIRS(src include)

IF(WITH_SAMPLES)
  SUBDIRS(samples)
ENDIF(WITH_SAMPLES)

IF(WITH_TOOLS)
  SUBDIRS(tools)
ENDIF(WITH_TOOLS)
