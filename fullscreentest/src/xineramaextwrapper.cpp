#include "xineramaextwrapper.h"

bool XineramaExtensionWrapper::initLibrary(const char *path) {
	if (path == NULL)
		path = LIB_XINERAMA;
	if (!initLibraryHandle(path))
		return false;
	
	LIBRARY_FUNCTION(fnXineramaIsActive, FN_XINERAMA_IS_ACTIVE)
	LIBRARY_FUNCTION(fnXineramaQueryScreens, FN_XINERAMA_QUERY_SCREENS)
	LIBRARY_FUNCTION(fnXineramaQueryVersion, FN_XINERAMA_QUERY_VERSION)

	std::cout << "XineramaExtensionWrapper::initLibrary successful end" << std::endl;

	// this is not 100% safe to do this way, but should suffice
	// considering the context it will be used in
	initialized = true;
	return true;
}

bool XineramaExtensionWrapper::XineramaIsActive(Display *dpy) {
	return initialized && ((*fnXineramaIsActive)(dpy) == True);
}

XineramaScreenInfo* XineramaExtensionWrapper::XineramaQueryScreens(
		Display *dpy, int *amount) {
	if (!initialized)
		return NULL;
	return (*fnXineramaQueryScreens)(dpy, amount);
}

bool XineramaExtensionWrapper::XineramaQueryVersion(Display *dpy, int *major,
		int *minor) {
	return initialized &&
		(*fnXineramaQueryVersion)(dpy, major, minor) != 0;
}
