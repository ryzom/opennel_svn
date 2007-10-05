#
# $Id: Makefile.am,v 1.1 2005-04-04 09:45:06 cado Exp $
#

MAINTAINERCLEANFILES      = Makefile.in

EXTRA_DIST		= i18n_8.vcproj main.dsp main.dsw

bin_PROGRAMS              = i18n

i18n_SOURCES = main.cpp

i18ndir = $(datadir)/nel/samples/configfile

i18n_DATA = english.uxt french.uxt

AM_CXXFLAGS               = -DCF_DIR="\"$(i18ndir)\"" -I$(top_srcdir)/src 

i18n_LDADD   = ../../../src/misc/libnelmisc.la 


# End of Makefile.am

