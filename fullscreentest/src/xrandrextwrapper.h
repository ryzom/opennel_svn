#ifndef XRANDREXTWRAPPER_H_
#define XRANDREXTWRAPPER_H_

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include "extensionwrapper.h"

#define EXTWRAP_XRANDR "xrandr"

#define LIB_XRANDR   "libXrandr.so"

#define FN_XRANDR_QUERY_VERSION "XRRQueryVersion"
typedef Status (*fnXRRQueryVersion_t)(Display*, int*, int*);

#define FN_XRANDR_TIMES "XRRTimes"
typedef Time (*fnXRRTimes_t)(Display*, int, Time*);

#define FN_XRANDR_SIZES "XRRSizes"
typedef XRRScreenSize *(*fnXRRSizes_t)(Display*, int, int*);

#define FN_XRANDR_RATES "XRRRates"
typedef short *(*fnXRRRates_t)(Display*, int, int, int*);

#define FN_XRANDR_SET_SCREEN_CONFIG_AND_RATE "XRRSetScreenConfigAndRate"
typedef Status (*fnXRRSetScreenConfigAndRate_t)(Display*, XRRScreenConfiguration*, Drawable, int, Rotation, short, Time);

// XRandR 1.2+ functions

#define FN_XRANDR_GET_CRTC_INFO "XRRGetCrtcInfo"
typedef XRRCrtcInfo *(*fnXRRGetCrtcInfo_t)(Display*, XRRScreenResources*, RRCrtc);

#define FN_XRANDR_FREE_CRTC_INFO "XRRFreeCrtcInfo"
typedef void (*fnXRRFreeCrtcInfo_t)(XRRCrtcInfo*);

#define FN_XRANDR_GET_OUTPUT_INFO "XRRGetOutputInfo"
typedef XRROutputInfo *(*fnXRRGetOutputInfo_t)(Display*, XRRScreenResources*, RROutput);

#define FN_XRANDR_FREE_OUTPUT_INFO "XRRFreeOutputInfo"
typedef void (*fnXRRFreeOutputInfo_t)(XRROutputInfo*);

#define FN_XRANDR_CONFIG_CURRENT_CONFIGURATION "XRRConfigCurrentConfiguration"
typedef SizeID (*fnXRRConfigCurrentConfiguration_t)(XRRScreenConfiguration*, Rotation*);

#define FN_XRANDR_GET_SCREEN_RESOURCES "XRRGetScreenResources"
typedef XRRScreenResources *(*fnXRRGetScreenResources_t)(Display*, Window);

#define FN_XRANDR_FREE_SCREEN_RESOURCES "XRRFreeScreenResources"
typedef void (*fnXRRFreeScreenResources_t)(XRRScreenResources*);


class XRandRExtensionWrapper : public ExtensionWrapper {
public:
	XRandRExtensionWrapper() : ExtensionWrapper() { name = EXTWRAP_XRANDR; }
	virtual bool initLibrary(const char *path = NULL);
	
	bool XRRQueryVersion(Display *dpy, int *major, int *minor);
	Time XRRTimes(Display *dpy, int screen, Time *config_timestamp);
	XRRScreenSize *XRRSizes(Display *dpy, int screen, int *nsizes);
	short *XRRRates (Display *dpy, int screen, int sizeID, int *nrates);
	bool XRRSetScreenConfigAndRate(Display *dpy, XRRScreenConfiguration *config,Drawable draw, int size_index,
	                                 Rotation rotation, short rate, Time timestamp);
	/* RandR 1.2+ */
	XRRCrtcInfo *XRRGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc);
	void XRRFreeCrtcInfo(XRRCrtcInfo *crtcInfo);
	XRROutputInfo *XRRGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output);
	void XRRFreeOutputInfo(XRROutputInfo *outputInfo);
	SizeID XRRConfigCurrentConfiguration(XRRScreenConfiguration *config, Rotation *rotation);
	XRRScreenResources *XRRGetScreenResources(Display *dpy, Window window);
	void XRRFreeScreenResources(XRRScreenResources *resources);
private:
	fnXRRQueryVersion_t fnXRRQueryVersion;
	fnXRRTimes_t fnXRRTimes;
	fnXRRSizes_t fnXRRSizes;
	fnXRRRates_t fnXRRRates;
	fnXRRSetScreenConfigAndRate_t fnXRRSetScreenConfigAndRate;
	/* RandR 1.2+ */
	fnXRRGetCrtcInfo_t fnXRRGetCrtcInfo;
	fnXRRFreeCrtcInfo_t fnXRRFreeCrtcInfo;
	fnXRRGetOutputInfo_t fnXRRGetOutputInfo;
	fnXRRFreeOutputInfo_t fnXRRFreeOutputInfo;
	fnXRRConfigCurrentConfiguration_t fnXRRConfigCurrentConfiguration;
	fnXRRGetScreenResources_t fnXRRGetScreenResources;
	fnXRRFreeScreenResources_t fnXRRFreeScreenResources;
};


#endif /*XRANDREXTWRAPPER_H_*/
