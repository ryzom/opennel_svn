dnl ====================================================================
dnl Configuration script for Snowballs 2
dnl ====================================================================
dnl
dnl $Id: configure.in,v 1.16 2002-07-08 15:10:16 lecroart Exp $
dnl 


dnl ====================================================================
dnl Process this file with autoconf to produce a configure script.
dnl ====================================================================

# If you want to change the version, must must change AC_INIT
# *and* AC_SUBST(LIBTOOL_VERSION)

AC_PREREQ(2.57)
AC_INIT([sfb-personal],[0.1.0],[matt.raykowski@gmail.com])
AM_INIT_AUTOMAKE([tar-ustar])

AM_CONFIG_HEADER(config.h)

AC_SUBST(LIBTOOL_VERSION, [0:1:0])

dnl ====================================================================
dnl Checks for programs.
dnl ====================================================================
AC_CANONICAL_HOST
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
AM_SANITY_CHECK

AC_SYS_LARGEFILE

AM_MAINTAINER_MODE

dnl The following hack should ensure that configure doesn't add optimizing
dnl or debugging flags to CFLAGS or CXXFLAGS
CXXFLAGS="$CXXFLAGS -ftemplate-depth-24"

dnl Set up CXXFLAGS so that we can reach includes.
CXXFLAGS="$CXXFLAGS -I\${top_srcdir}/libs/entmodel/include"

AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([dl], [dlopen])

dnl ====================================================================
dnl Configure Settings
dnl ====================================================================

dnl Set the C++ compiler as the default compiler for the configure script
dnl compilation tests
AC_LANG_CPLUSPLUS


dnl ====================================================================
dnl Debug/optimized compilation mode
dnl ====================================================================

AM_NEL_DEBUG


dnl ====================================================================
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ====================================================================

# Test endianness
AC_C_BIGENDIAN

# Supress GCC "multi-character character constant" warnings.
if test "$ac_cv_cxx_compiler_gnu" = "yes";
then
    if test "$with_debug" = "yes"
    then
        #
        # When debugging variables are declared for the sole purpose of
        # inspecting their content with a debugger. They are not used
        # in the code itself and this is legitimate, hence the -Wno-unused
        #
        CXXFLAGS="$CXXFLAGS -Wno-unused"
    fi
fi

# Add some common define
if test "$ac_cv_cxx_compiler_gnu" = "yes";
then
    CXXFLAGS="$CXXFLAGS -D_REENTRANT -Wall -ansi -W -Wpointer-arith -Wsign-compare -Wno-deprecated-declarations -Wno-multichar -Wno-long-long -Wno-unused"
fi


dnl ====================================================================
dnl Checks for header and lib files.
dnl ====================================================================

AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h malloc.h netdb.h netinet/in.h stddef.h stdlib.h string.h sys/ioctl.h sys/socket.h unistd.h sys/time.h])
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([dl], [dlopen])

dnl ====================================================================
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ====================================================================

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_TYPES([size_t])
AC_CHECK_TYPES([uintptr_t])

dnl ====================================================================
dnl X11

AC_PATH_X

if test ! "$no_x" = "yes" ; then
    if test ! X"$x_libraries" = X
    then
        LIBS="$LIBS -L$x_libraries"
    fi
    if test ! X"$x_includes" = X
    then
        CXXFLAGS="$CXXFLAGS -I$x_includes"
    fi
else
    if test "$enable_client" = "yes"
    then
        AC_MSG_ERROR(X11 must be installed.)
    fi
fi


dnl ====================================================================
dnl Checks for libraries.
dnl ====================================================================

dnl ====================================================================
dnl STLPort

AM_PATH_STLPORT('/usr/local /usr')
LIBS="$LIBS $STLPORT_LIBS"
CPPFLAGS="$CPPFLAGS $STLPORT_INCLUDES"
LDFLAGS="$LDFLAGS $STLPORT_LDFLAGS"


dnl ====================================================================
dnl NeL

dnl         misc   net    3d              pacs            sound ai    georges
AM_PATH_NEL("yes", "yes", "yes",          "yes",          "no", "no", "no")

dnl ====================================================================
dnl FreeType 2

AM_PATH_FREETYPE("yes")

dnl ====================================================================
dnl Checks for library functions.
dnl ====================================================================

AC_CONFIG_FILES([Makefile					\
		 libs/Makefile					\
		 libs/entmodel/Makefile				\
		 libs/entmodel/src/Makefile			\
])
AC_OUTPUT

dnl End of configure.in

