CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(nelcollada CXX C)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#-----------------------------------------------------------------------------
# Redirect output files
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

FIND_PACKAGE(LibXml2 REQUIRED)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})


#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-W -Wall -Wno-unused-parameter -Wno-unused-function -O0 -g)

ADD_DEFINITIONS(-DPLUGIN_EXPORT -D_DEBUG -DLINUX)

INCLUDE_DIRECTORIES(/home/mattr/nelhome/include/FCollada)
#list all source files here
ADD_EXECUTABLE(nelcollada main.cpp)

FIND_LIBRARY(FCOLLADA_LIBRARY
      NAMES
	FColladaD
      PATHS
	/home/mattr/nelhome/lib
        /usr/lib
        /usr/local/lib
        /usr/openwin/lib
        /usr/X11R6/lib
        /opt/gnome/lib
        /opt/lib
        /sw/lib
    )

#need to link to some other libraries ? just add them here
#TARGET_LINK_LIBRARIES(nelcollada png jpeg)
TARGET_LINK_LIBRARIES(nelcollada ${LIBXML2_LIBRARIES} ${FCOLLADA_LIBRARY})
 

