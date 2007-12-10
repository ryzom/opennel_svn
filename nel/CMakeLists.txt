IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
MESSAGE(FATAL_ERROR "CMake generation for OpenNeL is not allowed within the source directory! 
Remove the CMakeCache.txt file and try again from another folder, e.g.: 

   rm CMakeCache.txt 
   mkdir cmake
   cd cmake
   cmake -G \"Unix Makefiles\" ..
")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

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

###
# Features
###
OPTION(WITH_LOGGING		"With Logging"					ON)
OPTION(WITH_COVERAGE		"With Code Coverage Support"			ON)

###
# Core libraries
###
OPTION(WITH_NET			"Build NLNET"					ON )
OPTION(WITH_3D			"Build NL3D"					ON )
OPTION(WITH_PACS		"Build NLPACS"					OFF)
OPTION(WITH_GEORGES		"Build NLGEORGES"				ON )
OPTION(WITH_LIGO		"Build NLLIGO"					ON )
OPTION(WITH_SOUND		"Build NLSOUND"					ON )

###
# Drivers Support
###
OPTION(WITH_DRIVER_OPENAL	"Build OpenAL Driver"				OFF)
OPTION(WITH_DRIVER_OPENGL	"Build OpenGL Driver"				ON )
OPTION(WITH_DRIVER_DIRECT3D	"Build OpenGL Driver"				OFF)

###
# Optional support
###
OPTION(WITH_CEGUI	"Build CEGUI Renderer"					OFF)
OPTION(WITH_TOOLS	"Build NeL Tools"					OFF)
OPTION(WITH_SAMPLES	"Build NeL Samples"					ON )
OPTION(WITH_TESTS	"Build NeL Unit Tests"					OFF)
OPTION(WITH_GTK		"With GTK Support"					OFF)

#-----------------------------------------------------------------------------
# Load some macros.
INCLUDE(automacros/opennel.cmake)

#-----------------------------------------------------------------------------
#Platform specifics

IF(UNIX)
  IF(WITH_DRIVER_OPENAL)
    INCLUDE(${CMAKE_ROOT}/Modules/FindOpenAL.cmake)
    IF(OPENAL_FOUND)
      SET(WITH_OPENAL ON)
      SET(OPENAL_LIB ${OPENAL_LIBRARY})
      SET(OPENAL_INC ${OPENAL_INCLUDE_DIR})
    ELSE(OPENAL_FOUND)
      SET(WITH_OPENAL OFF)
    ENDIF(OPENAL_FOUND)

    FIND_LIBRARY(ALUT_LIBRARY
      NAMES alut
      PATHS
      /usr/local/lib
      /usr/lib
      /sw/lib
      /opt/local/lib
      /opt/csw/lib
      /opt/lib
    )
    IF(ALUT_LIBRARY)
      SET(OPENAL_LIB ${OPENAL_LIB} ${ALUT_LIBRARY})
    ENDIF(ALUT_LIBRARY)
  ENDIF(WITH_DRIVER_OPENAL)

  IF(WITH_3D)
    FIND_PATH(FREETYPE_INC
      freetype
      PATHS
      /usr/local/include/freetype2
      /usr/include/freetype2
      /sw/include/freetype2
      /opt/local/include/freetype2
      /opt/csw/include/freetype2
      /opt/include/freetype2
      NO_DEFAULT_PATH
    )
    SET(FREETYPE_LIB freetype)

    SET(JPEG_LIB jpeg)

    IF(WITH_DRIVER_OPENGL)
      INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
    ENDIF(WITH_DRIVER_OPENGL)
  ENDIF(WITH_3D)

  IF(WITH_GTK)
    INCLUDE($(CMAKE_ROOT)/Modules/FindGTK.cmake)
  ENDIF(WITH_GTK)

  INCLUDE(${CMAKE_ROOT}/Modules/FindLibXml2.cmake)

  INCLUDE(${CMAKE_ROOT}/Modules/FindThreads.cmake)

  #-----------------------------------------------------------------------------
  # Setup the buildmode variables.
  #
  # None                  = NL_RELEASE_DEBUG
  # Debug                 = NL_DEBUG
  # Release               = NL_RELEASE
  # RelWithDebInfo        = NL_RELEASE_DEBUG
  # MinSizeRel            = NL_RELEASE_DEBUG

  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(NL_DEBUG_CFLAGS "-DNL_DEBUG -g")
    SET(NL_BUILD_MODE "NL_DEBUG")
  ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
    IF(CMAKE_BUILD_TYPE MATCHES "Release")
      SET(NL_RELEASE_CFLAGS "-DNL_RELEASE -O6")
      SET(NL_BUILD_MODE "NL_RELEASE")
    ELSE(CMAKE_BUILD_TYPE MATCHES "Release")
      SET(NL_RELEASEDEBUG_CFLAGS "-DNL_RELEASE_DEBUG -g -finline-functions -O3 ")
      SET(NL_NONE_CFLAGS "-DNL_RELEASE_DEBUG -g -finline-functions -O3 ")
      SET(NL_BUILD_MODE "NL_RELEASE_DEBUG")
    ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")
  ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")


  #SET(LLIBS "-lc -lm -lstdc++")

  SET(PLATFORM_CFLAGS "-ftemplate-depth-24 -D_REENTRANT -Wall -ansi -W -Wpointer-arith -Wsign-compare -Wno-deprecated-declarations -Wno-multichar -Wno-long-long -Wno-unused")

  SET(PLATFORM_LINKFLAGS "${CMAKE_THREAD_LIBS_INIT} -lc -lm -lstdc++ -lrt")

  INCLUDE_DIRECTORIES(/usr/include /usr/local/include)
  


ENDIF(UNIX)

IF(WIN32)
  INCLUDE(${CMAKE_ROOT}/Modules/Platform/Windows-cl.cmake)
  
  SET(DEPENDSDIR ${CMAKE_SOURCE_DIR}/../depends)
  
  SET(LIBXML2_LIBRARIES libxml2.lib)
  
  SET(WINSOCK2_LIB ws2_32.lib)
  
  IF(WITH_DRIVER_OPENAL)
    SET(OPENAL_LIB OpenAL32.lib)
  ENDIF(WITH_DRIVER_OPENAL)
  
  SET(NL_DEBUG_CFLAGS "") # -g
  SET(NL_RELEASE_CFLAGS "") # -O6
  SET(NL_RELEASEDEBUG_CFLAGS "/DNL_RELEASE_DEBUG") # -g -finline-functions -O3 
  
  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(NL_BUILD_MODE "NL_DEBUG")
  ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
    IF(CMAKE_BUILD_TYPE MATCHES "Release")
      SET(NL_BUILD_MODE "NL_RELEASE")
    ELSE(CMAKE_BUILD_TYPE MATCHES "Release")
      SET(NL_BUILD_MODE "NL_RELEASE_DEBUG")
    ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")
  ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
  
  INCLUDE_DIRECTORIES(${DEPENDSDIR})
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
