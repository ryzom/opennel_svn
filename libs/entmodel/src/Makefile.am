#
# $Id: Makefile.am,v 1.87 2007-06-20 09:23:14 distrib Exp $
#

MAINTAINERCLEANFILES  = Makefile.in

lib_LTLIBRARIES       = libnelentmod.la


libnelentmod_la_LIBADD  = -lc -lpthread -lrt -ldl

libnelentmod_la_LDFLAGS = -version-info @LIBTOOL_VERSION@

#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = nel-entmod.pc

# End of Makefile.am

libnelentmod_la_SOURCES = gameeventserver.cpp
