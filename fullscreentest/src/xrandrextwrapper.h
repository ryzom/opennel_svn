#ifndef XRANDREXTWRAPPER_H_
#define XRANDREXTWRAPPER_H_

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include "extensionwrapper.h"

#define EXTWRAP_XRANDR "xrandr"

#define LIB_XRANDR   "libXrandr.so"

#define FN_XRANDR_QUERY_VERSION "XRRQueryVersion"
typedef Status (*fnXRRQueryVersion_t)(Display*, int*, int*);

// XRandR 1.1

#define FN_XRANDR_GET_SCREEN_INFO "XRRGetScreenInfo"
typedef XRRScreenConfiguration *(*fnXRRGetScreenInfo_t)(Display*, Window);

#define FN_XRANDR_FREE_SCREEN_CONFIG_INFO "XRRFreeScreenConfigInfo"
typedef void (*fnXRRFreeScreenConfigInfo_t)(XRRScreenConfiguration*);

#define FN_XRANDR_SET_SCREEN_CONFIG_AND_RATE "XRRSetScreenConfigAndRate"
typedef Status (*fnXRRSetScreenConfigAndRate_t)(Display*, XRRScreenConfiguration*, Drawable, int, Rotation, short, Time);

#define FN_XRANDR_CONFIG_SIZES "XRRConfigSizes"
typedef XRRScreenSize *(*fnXRRConfigSizes_t)(XRRScreenConfiguration*, int*);

#define FN_XRANDR_CONFIG_RATES "XRRConfigRates"
typedef short *(*fnXRRConfigRates_t)(XRRScreenConfiguration*, int, int*);

#define FN_XRANDR_CONFIG_CURRENT_CONFIGURATION "XRRConfigCurrentConfiguration"
typedef SizeID (*fnXRRConfigCurrentConfiguration_t)(XRRScreenConfiguration*, Rotation*);

#define FN_XRANDR_CONFIG_CURRENT_RATE "XRRConfigCurrentRate"
typedef short (*fnXRRConfigCurrentRate_t)(XRRScreenConfiguration*);

// XRandR 1.2+

#define FN_XRANDR_GET_CRTC_INFO "XRRGetCrtcInfo"
typedef XRRCrtcInfo *(*fnXRRGetCrtcInfo_t)(Display*, XRRScreenResources*, RRCrtc);

#define FN_XRANDR_FREE_CRTC_INFO "XRRFreeCrtcInfo"
typedef void (*fnXRRFreeCrtcInfo_t)(XRRCrtcInfo*);

#define FN_XRANDR_GET_OUTPUT_INFO "XRRGetOutputInfo"
typedef XRROutputInfo *(*fnXRRGetOutputInfo_t)(Display*, XRRScreenResources*, RROutput);

#define FN_XRANDR_FREE_OUTPUT_INFO "XRRFreeOutputInfo"
typedef void (*fnXRRFreeOutputInfo_t)(XRROutputInfo*);

#define FN_XRANDR_SET_CRTC_CONFIG "XRRSetCrtcConfig"
typedef Status (*fnXRRSetCrtcConfig_t)(Display*, XRRScreenResources*, RRCrtc, Time, int, int,
		                               RRMode, Rotation, RROutput*, int);

#define FN_XRANDR_GET_SCREEN_RESOURCES "XRRGetScreenResources"
typedef XRRScreenResources *(*fnXRRGetScreenResources_t)(Display*, Window);

#define FN_XRANDR_FREE_SCREEN_RESOURCES "XRRFreeScreenResources"
typedef void (*fnXRRFreeScreenResources_t)(XRRScreenResources*);


class XRandRExtensionWrapper : public ExtensionWrapper {
public:
	XRandRExtensionWrapper() : ExtensionWrapper(), init12(false) { name = EXTWRAP_XRANDR; }
	virtual bool initLibrary(const char *path = NULL);
	
	bool isInit12() { return init12; }
	
	bool XRRQueryVersion(Display *dpy, int *major, int *minor);
	XRRScreenConfiguration *XRRGetScreenInfo(Display *dpy, Window window);
	void XRRFreeScreenConfigInfo(XRRScreenConfiguration *config);
	XRRScreenSize *XRRConfigSizes(XRRScreenConfiguration *config, int *nsizes);
	short *XRRConfigRates(XRRScreenConfiguration *config, int sizeID, int *nrates);
	SizeID XRRConfigCurrentConfiguration(XRRScreenConfiguration *config, Rotation *rotation);
	short XRRConfigCurrentRate(XRRScreenConfiguration *config);
	bool XRRSetScreenConfigAndRate(Display *dpy, XRRScreenConfiguration *config,
			                       Drawable draw, int size_index, Rotation rotation,
			                       short rate, Time timestamp);
	/* RandR 1.2+ */
	XRRCrtcInfo *XRRGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc);
	void XRRFreeCrtcInfo(XRRCrtcInfo *crtcInfo);
	XRROutputInfo *XRRGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output);
	void XRRFreeOutputInfo(XRROutputInfo *outputInfo);
	bool XRRSetCrtcConfig(Display *dpy, XRRScreenResources *resources, RRCrtc crtc,
	                      Time timestamp, int x, int y, RRMode mode, Rotation rotation,
	                      RROutput *outputs, int noutputs);
	XRRScreenResources *XRRGetScreenResources(Display *dpy, Window window);
	void XRRFreeScreenResources(XRRScreenResources *resources);
private:
	fnXRRQueryVersion_t fnXRRQueryVersion;
	/* RandR 1.1 */
	fnXRRGetScreenInfo_t fnXRRGetScreenInfo;
	fnXRRFreeScreenConfigInfo_t fnXRRFreeScreenConfigInfo;
	fnXRRConfigSizes_t fnXRRConfigSizes;
	fnXRRConfigRates_t fnXRRConfigRates;
	fnXRRConfigCurrentConfiguration_t fnXRRConfigCurrentConfiguration;
	fnXRRConfigCurrentRate_t fnXRRConfigCurrentRate;
	fnXRRSetScreenConfigAndRate_t fnXRRSetScreenConfigAndRate;
	/* RandR 1.2+ */
	bool init12;
	fnXRRGetCrtcInfo_t fnXRRGetCrtcInfo;
	fnXRRFreeCrtcInfo_t fnXRRFreeCrtcInfo;
	fnXRRGetOutputInfo_t fnXRRGetOutputInfo;
	fnXRRFreeOutputInfo_t fnXRRFreeOutputInfo;
	fnXRRSetCrtcConfig_t fnXRRSetCrtcConfig;
	fnXRRGetScreenResources_t fnXRRGetScreenResources;
	fnXRRFreeScreenResources_t fnXRRFreeScreenResources;
};


#endif /*XRANDREXTWRAPPER_H_*/
