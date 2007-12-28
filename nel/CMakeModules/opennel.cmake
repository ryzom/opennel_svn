###
# Build Library Name
#
# Arguments: name - undecorated library name
# Sets: LIBNAME - decorated library name
###
MACRO(DECORATE_NEL_LIB name)

  IF(WIN32)
    IF(NL_BUILD_MODE MATCHES "NL_RELEASE_DEBUG")
      SET(LIBNAME "${name}_rd")
    ELSE(NL_BUILD_MODE MATCHES "NL_RELEASE_DEBUG")
      IF(NL_BUILD_MODE MATCHES "NL_DEBUG")
        SET(LIBNAME "${name}_d")
      ELSE(NL_BUILD_MODE MATCHES "NL_DEBUG")
        SET(LIBNAME "${name}_r")
      ENDIF(NL_BUILD_MODE MATCHES "NL_DEBUG")
    ENDIF(NL_BUILD_MODE MATCHES "NL_RELEASE_DEBUG")
  ELSE(WIN32)
    SET(LIBNAME "${name}")
  ENDIF(WIN32)

ENDMACRO(DECORATE_NEL_LIB)

###
# Checks build vs. source location. Prevents In-Source builds.
###
MACRO(CHECK_OUT_OF_SOURCE)
  IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "

CMake generation for this project is not allowed within the source directory!
Remove the CMakeCache.txt file and try again from another folder, e.g.:

   rm CMakeCache.txt
   mkdir cmake
   cd cmake
   cmake -G \"Unix Makefiles\" ..
    ")
  ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

ENDMACRO(CHECK_OUT_OF_SOURCE)

MACRO(NL_SETUP_DEFAULT_OPTIONS)
  ###
  # Features
  ###
  OPTION(WITH_LOGGING             "With Logging"                                  ON )
  OPTION(WITH_COVERAGE            "With Code Coverage Support"                    OFF)

  ###
  # Core libraries
  ###
  OPTION(WITH_NET                 "Build NLNET"                                   ON )
  OPTION(WITH_3D                  "Build NL3D"                                    ON )
  OPTION(WITH_PACS                "Build NLPACS"                                  ON )
  OPTION(WITH_GEORGES             "Build NLGEORGES"                               ON )
  OPTION(WITH_LIGO                "Build NLLIGO"                                  ON )
  OPTION(WITH_SOUND               "Build NLSOUND"                                 ON )

  ###
  # Drivers Support
  ###
  OPTION(WITH_DRIVER_OPENAL       "Build OpenAL Driver"                           ON )
  OPTION(WITH_DRIVER_OPENGL       "Build OpenGL Driver"                           ON )
  OPTION(WITH_DRIVER_DIRECT3D     "Build Direct3D Driver"                         OFF)

  ###
  # Optional support
  ###
  OPTION(WITH_CEGUI       "Build CEGUI Renderer"                                  OFF)
  OPTION(WITH_TOOLS       "Build NeL Tools"                                       OFF)
  OPTION(WITH_SAMPLES     "Build NeL Samples"                                     ON )
  OPTION(WITH_TESTS       "Build NeL Unit Tests"                                  OFF)
  OPTION(WITH_GTK         "With GTK Support"                                      OFF)

ENDMACRO(NL_SETUP_DEFAULT_OPTIONS)

MACRO(NL_FIND_OPENAL)
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
  ELSEIF(WIN32)
    IF(WITH_DRIVER_OPENAL)
      SET(OPENAL_LIB OpenAL32.lib)
    ENDIF(WITH_DRIVER_OPENAL)
  ENDIF(UNIX)
ENDMACRO(NL_FIND_OPENAL)

MACRO(NL_SETUP_BUILD)

  #-----------------------------------------------------------------------------
  # Setup the buildmode variables.
  #
  # None                  = NL_RELEASE_DEBUG
  # Debug                 = NL_DEBUG
  # Release               = NL_RELEASE
  # RelWithDebInfo        = NL_RELEASE_DEBUG
  # MinSizeRel            = NL_RELEASE_DEBUG

  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(NL_BUILD_MODE "NL_DEBUG")
  ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
    IF(CMAKE_BUILD_TYPE MATCHES "Release")
      SET(NL_BUILD_MODE "NL_RELEASE")
    ELSE(CMAKE_BUILD_TYPE MATCHES "Release")
      SET(NL_BUILD_MODE "NL_RELEASE_DEBUG")
    ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")
  ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

  IF(WIN32)
    SET(NL_DEBUG_CFLAGS "/ZI /Gy /GS-")
    SET(NL_RELEASE_CFLAGS "/Ox /Ob2 /Oi /Ot /Oy /GT /GL /GF")
    SET(NL_RELEASEDEBUG_CFLAGS "/DNL_RELEASE_DEBUG /Ob2 /GF")
  ELSE(WIN32)
    SET(PLATFORM_CFLAGS "-ftemplate-depth-24 -D_REENTRANT -Wall -ansi -W -Wpointer-arith -Wsign-compare -Wno-deprecated-declarations -Wno-multichar -Wno-long-long -Wno-unused")
    SET(PLATFORM_LINKFLAGS "${CMAKE_THREAD_LIBS_INIT} -lc -lm -lstdc++ -lrt")
    SET(NL_DEBUG_CFLAGS "-DNL_DEBUG -g")
    SET(NL_RELEASE_CFLAGS "-DNL_RELEASE -O6")
    SET(NL_RELEASEDEBUG_CFLAGS "-DNL_RELEASE_DEBUG -g -finline-functions -O3 ")
    SET(NL_NONE_CFLAGS "-DNL_RELEASE_DEBUG -g -finline-functions -O3 ")
  ENDIF(WIN32)
ENDMACRO(NL_SETUP_BUILD)
