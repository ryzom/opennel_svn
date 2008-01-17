#-----------------------------------------------------------------------------
#
# Snowballs
#    Author: Nevrax
#
# Notes:
#   * Changing install location: add -DCMAKE_INSTALL_PREFIX:PATH=/my/new/path
#   * Changing NeL prefix: add -DNEL_PREFIX_PATH:PATH=/my/nel/install
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Load some macros.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
INCLUDE(CMakeModules/opennel.cmake)

#-----------------------------------------------------------------------------
# Force out of source builds.
CHECK_OUT_OF_SOURCE()

IF(WIN32)
  SET(CMAKE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../3rdParty/include;${CMAKE_INCLUDE_PATH}")
  SET(CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/../3rdParty/lib;${CMAKE_INCLUDE_PATH}")
ENDIF(WIN32)

IF(DEFINED NEL_PREFIX_PATH)
  SET(CMAKE_INCLUDE_PATH "${NEL_PREFIX_PATH}/include;${CMAKE_INCLUDE_PATH}")
  SET(CMAKE_LIBRARY_PATH "${NEL_PREFIX_PATH}/lib;${CMAKE_INCLUDE_PATH}")
ENDIF(DEFINED NEL_PREFIX_PATH)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(Snowballs CXX C)
SET(NL_VERSION "0.3.0")

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
OPTION(BUILD_CLIENT "Build the Snowballs Client" ON)
OPTION(BUILD_SERVER "Build the Snowballs Servers" ON)

#-----------------------------------------------------------------------------
#Platform specifics

FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(LibXml2 REQUIRED)

NL_SETUP_BUILD()

FIND_PACKAGE(NeLMISC)
FIND_PACKAGE(NeLNET)

###
# An example of finding NeL 3D and CEGUI Renderer.
###
IF(BUILD_CLIENT)
  FIND_PACKAGE(FreeType)
  FIND_PACKAGE(NeL3D)
  FIND_PACKAGE(Jpeg)
  FIND_PACKAGE(NeLSOUND)
  FIND_PACKAGE(NeLSNDDRV)
  FIND_PACKAGE(NeLPACS)
  FIND_PACKAGE(NeLLIGO)
  FIND_PACKAGE(NeLGEORGES)
  
#    FIND_PACKAGE(CEGUI)
#    FIND_PACKAGE(NeLCEGUIRenderer)
ENDIF(BUILD_CLIENT)

IF(WIN32)
  INCLUDE(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)
  SET(WINSOCK2_LIB ws2_32.lib)
ENDIF(WIN32)

NL_SETUP_BUILD_FLAGS()

IF(UNIX)
  #SET(PKGDIR "${CMAKE_INSTALL_PREFIX}/share/nel/snowballs/")
  #SET(LOGDIR "${CMAKE_INSTALL_PREFIX}/var/log/snowballs/")
  SET(PKGDIR "share/nel/snowballs")
  SET(LOGDIR "var/log/snowballs")
  ADD_DEFINITIONS(-DSNOWBALLS_CONFIG="\\"${CMAKE_INSTALL_PREFIX}/${PKGDIR}/\\"" -DSNOWBALLS_STATE="\\"${CMAKE_INSTALL_PREFIX}/${PKGDIR}/\\"" -DSNOWBALLS_LOGS="\\"${CMAKE_INSTALL_PREFIX}/${LOGDIR}/\\"")
ENDIF(UNIX)

IF(BUILD_CLIENT)
  ADD_SUBDIRECTORY(client)
ENDIF(BUILD_CLIENT)

IF(BUILD_SERVER)
  ADD_SUBDIRECTORY(server)
ENDIF(BUILD_SERVER)

