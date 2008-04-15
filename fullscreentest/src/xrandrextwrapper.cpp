#include "xrandrextwrapper.h"
#include <iostream>

bool XRandRExtensionWrapper::initLibrary(const char *path) {
	if (path == NULL)
		path = LIB_XRANDR;

	std::cout << "XRandRExtensionWrapper::initLibrary(): init library ("
			<< path << ") ..." << std::endl;

	if (!initLibraryHandle(path))
		return false;

	std::cout
			<< "XRandRExtensionWrapper::initLibrary(): start function loading ..."
			<< std::endl;

	LIBRARY_FUNCTION(fnXRRQueryVersion, FN_XRANDR_QUERY_VERSION)
	init12 = true;
	/* RandR 1.1 */
	LIBRARY_FUNCTION(fnXRRGetScreenInfo, FN_XRANDR_GET_SCREEN_INFO)
	LIBRARY_FUNCTION(fnXRRFreeScreenConfigInfo, FN_XRANDR_FREE_SCREEN_CONFIG_INFO)
	LIBRARY_FUNCTION(fnXRRSetScreenConfigAndRate, FN_XRANDR_SET_SCREEN_CONFIG_AND_RATE)
	LIBRARY_FUNCTION(fnXRRConfigSizes, FN_XRANDR_CONFIG_SIZES)
	LIBRARY_FUNCTION(fnXRRConfigRates, FN_XRANDR_CONFIG_RATES)
	LIBRARY_FUNCTION(fnXRRConfigCurrentConfiguration, FN_XRANDR_CONFIG_CURRENT_CONFIGURATION)
	LIBRARY_FUNCTION(fnXRRConfigCurrentRate, FN_XRANDR_CONFIG_CURRENT_RATE)
	/* RandR 1.2+ */
	LIBRARY_FUNCTION_ACTION(fnXRRGetCrtcInfo, FN_XRANDR_GET_CRTC_INFO, init12 = false)
	if (init12) {
		LIBRARY_FUNCTION_ACTION(fnXRRFreeCrtcInfo, FN_XRANDR_FREE_CRTC_INFO, init12 = false)
	}
	if (init12) {
		LIBRARY_FUNCTION_ACTION(fnXRRGetOutputInfo, FN_XRANDR_GET_OUTPUT_INFO, init12 = false)
	}
	if (init12) {
		LIBRARY_FUNCTION_ACTION(fnXRRFreeOutputInfo, FN_XRANDR_FREE_OUTPUT_INFO, init12 = false)
	}
	if (init12) {
		LIBRARY_FUNCTION_ACTION(fnXRRSetCrtcConfig, FN_XRANDR_SET_CRTC_CONFIG, init12 = false)
	}
	if (init12) {
		LIBRARY_FUNCTION_ACTION(fnXRRGetScreenResources, FN_XRANDR_GET_SCREEN_RESOURCES, init12 = false)
	}
	if (init12) {
		LIBRARY_FUNCTION_ACTION(fnXRRFreeScreenResources, FN_XRANDR_FREE_SCREEN_RESOURCES, init12 = false)
	}

	std::cout << "XRandRExtensionWrapper::initLibrary successful "
			<< (init12 ? "(with v1.2+)" : "") << " end" << std::endl;

	// this is not 100% safe to do this way, but should suffice
	// considering the context it will be used in
	initialized = true;
	return true;
}

bool XRandRExtensionWrapper::XRRQueryVersion(Display *dpy, int *major,
		int *minor) {
	return initialized && (*fnXRRQueryVersion)(dpy, major, minor) == True;
}

XRRScreenConfiguration *XRandRExtensionWrapper::XRRGetScreenInfo(Display *dpy,
		Window window) {
	if (!initialized)
		return NULL;
	return (*fnXRRGetScreenInfo)(dpy, window);
}

void XRandRExtensionWrapper::XRRFreeScreenConfigInfo(
		XRRScreenConfiguration *config) {
	if (!initialized)
		return;
	(*fnXRRFreeScreenConfigInfo)(config);
}

XRRScreenSize *XRandRExtensionWrapper::XRRConfigSizes(
		XRRScreenConfiguration *config, int *nsizes) {
	if (!initialized)
		return NULL;
	return (*fnXRRConfigSizes)(config, nsizes);
}

short *XRandRExtensionWrapper::XRRConfigRates(XRRScreenConfiguration *config,
		int sizeID, int *nrates) {
	if (!initialized)
		return NULL;
	return (*fnXRRConfigRates)(config, sizeID, nrates);
}

SizeID XRandRExtensionWrapper::XRRConfigCurrentConfiguration(
		XRRScreenConfiguration *config, Rotation *rotation) {
	if (!initialized)
		return 0;
	return (*fnXRRConfigCurrentConfiguration)(config, rotation);
}

short XRandRExtensionWrapper::XRRConfigCurrentRate(
		XRRScreenConfiguration *config) {
	if (!initialized)
		return 0;
	return (*fnXRRConfigCurrentRate)(config);
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
	if (!init12)
		return NULL;
	return (*fnXRRGetCrtcInfo)(dpy, resources, crtc);
}

void XRandRExtensionWrapper::XRRFreeCrtcInfo(XRRCrtcInfo *crtcInfo) {
	if (!init12)
		return;
	(*fnXRRFreeCrtcInfo)(crtcInfo);
}

XRROutputInfo *XRandRExtensionWrapper::XRRGetOutputInfo(Display *dpy,
		XRRScreenResources *resources, RROutput output) {
	if (!init12)
		return NULL;
	return (*fnXRRGetOutputInfo)(dpy, resources, output);
}

void XRandRExtensionWrapper::XRRFreeOutputInfo(XRROutputInfo *outputInfo) {
	if (!init12)
		return;
	(*fnXRRFreeOutputInfo)(outputInfo);
}

bool XRandRExtensionWrapper::XRRSetCrtcConfig(Display *dpy,
		XRRScreenResources *resources, RRCrtc crtc, Time timestamp, int x,
		int y, RRMode mode, Rotation rotation, RROutput *outputs, int noutputs) {
	return init12 && (*fnXRRSetCrtcConfig)(dpy, resources, crtc, timestamp, x,
			y, mode, rotation, outputs, noutputs);
}

XRRScreenResources *XRandRExtensionWrapper::XRRGetScreenResources(Display *dpy,
		Window window) {
	if (!init12)
		return NULL;
	return (*fnXRRGetScreenResources)(dpy, window);
}

void XRandRExtensionWrapper::XRRFreeScreenResources(
		XRRScreenResources *resources) {
	if (!init12)
		return;
	(*fnXRRFreeScreenResources)(resources);
}
