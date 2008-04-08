#ifndef XINERAMAEXTWRAPPER_H_
#define XINERAMAEXTWRAPPER_H_

#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
#include "extensionwrapper.h"

#define EXTWRAP_XINERAMA "xinerama"

#define LIB_XINERAMA "libXinerama.so"

#define FN_XINERAMA_IS_ACTIVE "XineramaIsActive"
typedef Bool (*fnXineramaIsActive_t)(Display*);

#define FN_XINERAMA_QUERY_SCREENS "XineramaQueryScreens"
typedef XineramaScreenInfo *(*fnXineramaQueryScreens_t)(Display*, int*);

#define FN_XINERAMA_QUERY_VERSION "XineramaQueryVersion"
typedef Status (*fnXineramaQueryVersion_t)(Display*, int*, int*);

/**
 * \brief XineramaExtensionWrapper covers the few functions
 *        the X11 Xinerama extensions offers to programmers
 *        and makes it available through this class.
 */
class XineramaExtensionWrapper : public ExtensionWrapper {
public:
	XineramaExtensionWrapper() : ExtensionWrapper() { name = EXTWRAP_XINERAMA; }
	//virtual ~XineramaExtensionWrapper() {};
	virtual bool initLibrary(const char *path = NULL);
	
	bool XineramaIsActive(Display *dpy);
	XineramaScreenInfo* XineramaQueryScreens(Display *dpy, int *amount);
	bool XineramaQueryVersion(Display *dpy, int *major, int *minor);
	
private:
	fnXineramaIsActive_t fnXineramaIsActive;
	fnXineramaQueryScreens_t fnXineramaQueryScreens;
	fnXineramaQueryVersion_t fnXineramaQueryVersion;
};

#endif /*XINERAMAEXTWRAPPER_H_*/
