#ifndef XVIDMODEEXTWRAPPER_H_
#define XVIDMODEEXTWRAPPER_H_

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include "extensionwrapper.h"

#define EXTWRAP_XVIDMODE "xvidmode"

#define LIB_XVIDMODE "libXxf86vm.so"

#define FN_XVIDMODE_GET_ALL_MODE_LINES "XF86VidModeGetAllModeLines"
typedef Bool (*fnXF86VidModeGetAllModeLines_t)(Display*, int, int*, XF86VidModeModeInfo***);

#define FN_XVIDMODE_SWITCH_TO_MODE "XF86VidModeSwitchToMode"
typedef Bool (*fnXF86VidModeSwitchToMode_t)(Display*, int, XF86VidModeModeInfo*);

#define FN_XVIDMODE_SET_VIEW_PORT "XF86VidModeSetViewPort"
typedef Bool (*fnXF86VidModeSetViewPort_t)(Display *, int, int, int);

#define FN_XVIDMODE_QUERY_VERSION "XF86VidModeQueryVersion"
typedef Status (*fnXF86VidModeQueryVersion_t)(Display*, int*, int*);

/**
 * \brief Wrapper for some XF86VidMode X11 extension functions.
 */
class XVidModeExtensionWrapper : public ExtensionWrapper {
public:
	XVidModeExtensionWrapper() : ExtensionWrapper() { name = EXTWRAP_XVIDMODE; }
	virtual bool initLibrary(const char *path = NULL);
	
	bool XF86VidModeGetAllModeLines(Display *dpy, int screen, int* modeCount, XF86VidModeModeInfo*** info);
	bool XF86VidModeSwitchToMode(Display *dpy, int screen, XF86VidModeModeInfo *modeInfo);
	bool XF86VidModeSetViewPort(Display *dpy, int screen, int x, int y);
	bool XF86VidModeQueryVersion(Display *dpy, int *major, int *minor);
private:
	fnXF86VidModeGetAllModeLines_t fnXF86VidModeGetAllModeLines;
	fnXF86VidModeSwitchToMode_t fnXF86VidModeSwitchToMode;
	fnXF86VidModeSetViewPort_t fnXF86VidModeSetViewPort;
	fnXF86VidModeQueryVersion_t fnXF86VidModeQueryVersion;
};


#endif /*XVIDMODEEXTWRAPPER_H_*/
