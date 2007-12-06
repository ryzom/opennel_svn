IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
MESSAGE(FATAL_ERROR "CMake generation for OpenNeL is not allowed within the source directory! 
Remove the CMakeCache.txt file and try again from another folder, e.g.: 

   rm CMakeCache.txt 
   mkdir cmake-make 
   cd cmake-make
   cmake -G \"Unix Makefiles\" ..
")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(OpenNeL)

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
OPTION(WITH_NET			"Build NLNET"					OFF)
OPTION(WITH_3D			"Build NL3D"					OFF)
OPTION(WITH_PACS		"Build NLPACS"					OFF)
OPTION(WITH_GEORGES		"Build NLGEORGES"				OFF)
OPTION(WITH_LIGO		"Build NLLIGO"					OFF)
OPTION(WITH_SOUND		"Build NLSOUND"					OFF)

###
# Drivers Support
###
OPTION(WITH_DRIVER_OPENAL	"Build OpenAL Driver"				OFF)
OPTION(WITH_DRIVER_OPENGL	"Build OpenGL Driver"				OFF)

###
# Optional support
###
OPTION(WITH_CEGUI	"Build CEGUI Renderer"					OFF)
OPTION(WITH_TOOLS	"Build NeL Tools"					OFF)
OPTION(WITH_SAMPLES	"Build NeL Samples"					OFF)
OPTION(WITH_TESTS	"Build NeL Unit Tests"					OFF)
OPTION(WITH_GTK		"With GTK Support"					OFF)

#-----------------------------------------------------------------------------
# Load some macros.

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

    INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
  ENDIF(WITH_3D)

  IF(WITH_GTK)
    INCLUDE($(CMAKE_ROOT)/Modules/FindGTK.cmake)
  ENDIF(WITH_GTK)

  INCLUDE($(CMAKE_ROOT)/Modules/FindLibXml2.cmake)

  INCLUDE($(CMAKE_ROOT)/Modules/FindThreads.cmake)

  SET(LLIBS "-lc -lm -lstdc++")

  SET(PLATFORM_CFLAGS "-pipe -fPIC -funsigned-char -fno-strict-aliasing -DXP_UNIX -Wno-char-subscripts")

  SET(PLATFORM_LINKFLAGS "")

  INCLUDE_DIRECTORIES(/usr/include /usr/local/include)
ENDIF(UNIX)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PLATFORM_CFLAGS} ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PLATFORM_CFLAGS} ")


