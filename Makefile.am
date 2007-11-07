#
# $Id: Makefile.am,v 1.4 2002-04-10 07:32:46 lecroart Exp $
#

MAINTAINERCLEANFILES = Makefile.in configure config.guess missing \
                       config.sub ltconfig aclocal.m4 config.h.in \
                       install-sh mkinstalldirs

EXTRA_DIST	     = autogen.sh \
		       automacros \
		       kdevelop

dist-hook:
	find $(distdir) -name CVS -print | xargs rm -fr
	find $(distdir) -name .svn -print | xargs rm -fr

SUBDIRS              = libs

# End of Makefile.am

