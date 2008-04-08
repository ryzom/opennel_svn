#include "xvidmodeextwrapper.h"

bool XVidModeExtensionWrapper::initLibrary(const char *path) {
	if (path == NULL)
		path = LIB_XVIDMODE;
	if (!initLibraryHandle(path))
		return false;
	
	LIBRARY_FUNCTION(fnXF86VidModeGetAllModeLines, FN_XVIDMODE_GET_ALL_MODE_LINES)
	LIBRARY_FUNCTION(fnXF86VidModeSwitchToMode, FN_XVIDMODE_SWITCH_TO_MODE)
	LIBRARY_FUNCTION(fnXF86VidModeSetViewPort, FN_XVIDMODE_SET_VIEW_PORT)
	LIBRARY_FUNCTION(fnXF86VidModeQueryVersion, FN_XVIDMODE_QUERY_VERSION)

	std::cout << "XVidModeExtensionWrapper::initLibrary successful end" << std::endl;

	// this is not 100% safe to do this way, but should suffice
	// considering the context it will be used in
	initialized = true;
	return true;
}

bool XVidModeExtensionWrapper::XF86VidModeGetAllModeLines(Display *dpy, int screen, int* modeCount, XF86VidModeModeInfo*** info) {
	return initialized && (*fnXF86VidModeGetAllModeLines)(dpy, screen, modeCount, info) == True;
}

bool XVidModeExtensionWrapper::XF86VidModeSwitchToMode(Display *dpy, int screen, XF86VidModeModeInfo *modeInfo) {
	return initialized && (*fnXF86VidModeSwitchToMode)(dpy, screen, modeInfo) == True;
}

bool XVidModeExtensionWrapper::XF86VidModeSetViewPort(Display *dpy, int screen, int x, int y) {
	return initialized && (*fnXF86VidModeSetViewPort)(dpy, screen, x, y) == True;
}

bool XVidModeExtensionWrapper::XF86VidModeQueryVersion(Display *dpy, int *major, int *minor) {
	return initialized && (*fnXF86VidModeQueryVersion)(dpy, major, minor) == True;
}
