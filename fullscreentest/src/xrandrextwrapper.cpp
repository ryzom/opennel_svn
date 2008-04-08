#include "xrandrextwrapper.h"
#include <iostream>

bool XRandRExtensionWrapper::initLibrary(const char *path) {
	if (path == NULL)
		path = LIB_XRANDR;

	std::cout << "XRandRExtensionWrapper::initLibrary(): init library (" << path << ") ..." << std::endl;

	if (!initLibraryHandle(path))
		return false;

	std::cout << "XRandRExtensionWrapper::initLibrary(): start function loading ..." << std::endl;
	
	LIBRARY_FUNCTION(fnXRRQueryVersion, FN_XRANDR_QUERY_VERSION);
	LIBRARY_FUNCTION(fnXRRTimes, FN_XRANDR_TIMES);
	LIBRARY_FUNCTION(fnXRRSizes, FN_XRANDR_SIZES);
	LIBRARY_FUNCTION(fnXRRRates, FN_XRANDR_RATES);
	LIBRARY_FUNCTION(fnXRRSetScreenConfigAndRate, FN_XRANDR_SET_SCREEN_CONFIG_AND_RATE);
	/* RandR 1.2+ */
	LIBRARY_FUNCTION(fnXRRGetCrtcInfo, FN_XRANDR_GET_CRTC_INFO);
	LIBRARY_FUNCTION(fnXRRFreeCrtcInfo, FN_XRANDR_FREE_CRTC_INFO);
	LIBRARY_FUNCTION(fnXRRGetOutputInfo, FN_XRANDR_GET_OUTPUT_INFO);
	LIBRARY_FUNCTION(fnXRRFreeOutputInfo, FN_XRANDR_FREE_OUTPUT_INFO);
	LIBRARY_FUNCTION(fnXRRConfigCurrentConfiguration, FN_XRANDR_CONFIG_CURRENT_CONFIGURATION);
	LIBRARY_FUNCTION(fnXRRGetScreenResources, FN_XRANDR_GET_SCREEN_RESOURCES);
	LIBRARY_FUNCTION(fnXRRFreeScreenResources, FN_XRANDR_FREE_SCREEN_RESOURCES);

	std::cout << "XRandRExtensionWrapper::initLibrary successful end"
			<< std::endl;

	// this is not 100% safe to do this way, but should suffice
	// considering the context it will be used in
	initialized = true;
	return true;
}

bool XRandRExtensionWrapper::XRRQueryVersion(Display *dpy, int *major,
		int *minor) {
	return initialized && (*fnXRRQueryVersion)(dpy, major, minor) == True;
}

Time XRandRExtensionWrapper::XRRTimes(Display *dpy, int screen,
		Time *config_timestamp) {
	if (!initialized)
		return 0;
	return (*fnXRRTimes)(dpy, screen, config_timestamp);
}

XRRScreenSize *XRandRExtensionWrapper::XRRSizes(Display *dpy, int screen,
		int *nsizes) {
	if (!initialized)
		return NULL;
	return (*fnXRRSizes)(dpy, screen, nsizes);
}

short *XRandRExtensionWrapper::XRRRates(Display *dpy, int screen, int sizeID,
		int *nrates) {
	if (!initialized)
		return NULL;
	return (*fnXRRRates)(dpy, screen, sizeID, nrates);
}

bool XRandRExtensionWrapper::XRRSetScreenConfigAndRate(Display *dpy,
		XRRScreenConfiguration *config, Drawable draw, int size_index,
		Rotation rotation, short rate, Time timestamp) {
	return initialized && (*fnXRRSetScreenConfigAndRate)(dpy, config, draw,
			size_index, rotation, rate, timestamp);
}

/* RandR 1.2+ */

XRRCrtcInfo *XRandRExtensionWrapper::XRRGetCrtcInfo(Display *dpy,
		XRRScreenResources *resources, RRCrtc crtc) {
	if (!initialized)
		return NULL;
	return (*fnXRRGetCrtcInfo)(dpy, resources, crtc);
}

void XRandRExtensionWrapper::XRRFreeCrtcInfo(XRRCrtcInfo *crtcInfo) {
	if (!initialized)
		return;
	(*fnXRRFreeCrtcInfo)(crtcInfo);
}

XRROutputInfo *XRandRExtensionWrapper::XRRGetOutputInfo(Display *dpy,
		XRRScreenResources *resources, RROutput output) {
	if (!initialized)
		return NULL;
	return (*fnXRRGetOutputInfo)(dpy, resources, output);
}

void XRandRExtensionWrapper::XRRFreeOutputInfo(XRROutputInfo *outputInfo) {
	if (!initialized)
		return;
	(*fnXRRFreeOutputInfo)(outputInfo);
}

SizeID XRandRExtensionWrapper::XRRConfigCurrentConfiguration(
		XRRScreenConfiguration *config, Rotation *rotation) {
	if (!initialized)
		return 0;
	return (*fnXRRConfigCurrentConfiguration)(config, rotation);
}

XRRScreenResources *XRandRExtensionWrapper::XRRGetScreenResources(Display *dpy,
		Window window) {
	if (!initialized)
		return NULL;
	return (*fnXRRGetScreenResources)(dpy, window);
}

void XRandRExtensionWrapper::XRRFreeScreenResources(
		XRRScreenResources *resources) {
	if (!initialized)
		return;
	(*fnXRRFreeScreenResources)(resources);
}
