#ifndef LIB_FUNCTIONS_H_
#define LIB_FUNCTIONS_H_

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/Xinerama.h>


/**** XF86VidMode ****/

#define LIB_XVIDMODE "libXxf86vm.so"

#define FN_XVIDMODE_GET_ALL_MODE_LINES "XF86VidModeGetAllModeLines"
typedef Bool (*fnXF86VidModeGetAllModeLines_t)(Display*, int, int*, XF86VidModeModeInfo***);
#define FN_XVIDMODE_SWITCH_TO_MODE "XF86VidModeSwitchToMode"
typedef Bool (*fnXF86VidModeSwitchToMode_t)(Display*, int, XF86VidModeModeInfo*);
#define FN_XVIDMODE_SET_VIEW_PORT "XF86VidModeSetViewPort"
typedef Bool (*fnXF86VidModeSetViewPort_t)(Display *, int, int, int);


/**** Xinerama *******/

#define LIB_XINERAMA "libXinerama.so"

#define FN_XINERAMA_IS_ACTIVE "XineramaIsActive"
typedef Bool (*fnXineramaIsActive_t)(Display*);
#define FN_XINERAMA_QUERY_SCREENS "XineramaQueryScreens"
typedef XineramaScreenInfo *(*fnXineramaQueryScreens_t)(Display*, int*);


/**** XRandR *********/

#define LIB_XRANDR   "libXrandr.so"


#endif /* LIB_FUNCTIONS_H_ */
