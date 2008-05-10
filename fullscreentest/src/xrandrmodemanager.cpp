#include <dlfcn.h>
#include <string>
#include <iostream>
#include <map>
#include "xrandrmodemanager.h"
#include "exception.h"
#include "gfxmode.h"

bool XRandrModeManager::initLibraries() {
	// Xinerama is optional, which means initializing
	// the library may fail, which results in a disabled
	// Xinerama extension
	std::string libPath;
	if (xinAvail) {
		libPath = LIB_XINERAMA;
		if (libraries.count(libPath))
			libPath = libraries[libPath];
		xinerama
				= reinterpret_cast<XineramaExtensionWrapper*>(wrapperFactory->getInstance(EXTWRAP_XINERAMA, libPath.c_str()));
		if (xinerama == 0)
			std::cerr
					<< "failed to load Xinerama wrapper object. Disabling Xinerama usage."
					<< std::endl;
		xinAvail = (xinerama != 0);
	}

	libPath = LIB_XRANDR;
	if (libraries.count(libPath))
		libPath = libraries[libPath];
	xrandr
			= reinterpret_cast<XRandRExtensionWrapper*>(wrapperFactory->getInstance(EXTWRAP_XRANDR, libPath.c_str()));

	if (xrandr == 0) {
		std::cerr << "failed to acquire XRandR wrapper object." << std::endl;
		return false;
	}

	return true;
}

void XRandrModeManager::init11Modes(Display *dpy) {
	std::cerr << "RandR 1.1 support not implemented, yet." << std::endl;
}

void XRandrModeManager::init12Modes(Display *dpy) {
	Window root= RootWindow(dpy, 0); //, screenNum);
	XWindowAttributes windowAttributes;
	int depth = 24;
	if (XGetWindowAttributes(dpy, root, &windowAttributes) != 0) {
		depth = windowAttributes.depth;
	}
	if (resources != NULL)
		xrandr->XRRFreeScreenResources(resources);
	resources = xrandr->XRRGetScreenResources(dpy, root);
	std::cout << "resource with " << resources->ncrtc << " crtcs, "
			<< resources->noutput << " outputs and " << resources->nmode
			<< " modes" << std::endl;

	// load mode list into a map for quick access
	std::map<int, XRRModeInfo*> modeInfoList; // (res->nmode);
	for (int i = 0; i < resources->nmode; i++) {
		XRRModeInfo *mode = resources->modes + i;
		modeInfoList[mode->id] = mode;
	}
	// go through all outputs, determine default CRTC for it and
	// assign all modes it supports to it
	for (int outputNum = 0; outputNum < resources->noutput; outputNum++) {
		XRROutputInfo *output = xrandr->XRRGetOutputInfo(dpy, resources,
				resources->outputs[outputNum]);
		// don't handle modes, if no crtc is connected
		if (output->crtc > 0) {
			XRRCrtcInfo *crtc = xrandr->XRRGetCrtcInfo(dpy, resources,
					output->crtc);
			for (int modeNum = 0; modeNum < output->nmode; modeNum++) {
				// go through all modes for a specific output
				int modeIndex = output->modes[modeNum];
				// init addition mode info to be stored in UserData
				RandR12Mode *modeExt = new RandR12Mode();
				modeExt->version = RANDR_12;
				modeExt->crtc = output->crtc;
				modeExt->mode = output->modes[modeNum];
				modeExt->output = resources->outputs[outputNum];
				modeExt->rotation = crtc->rotation;
				modeExt->timestamp = output->timestamp; // TODO add timestamp info
				// construct GfxMode struct with all the data collected
				GfxMode
						*mode = new GfxMode(false, false,
								modeInfoList[modeIndex]->width,
								modeInfoList[modeIndex]->height,
								depth, // same for all?
								// round up for values > x.5
								(uint8_t)((2 * modeInfoList[modeIndex]->dotClock /
												(modeInfoList[modeIndex]->hTotal * modeInfoList[modeIndex]->vTotal)+1)/2),
								0,
								outputNum,
								crtc->x, // origin_x,
								crtc->y, // origin_y,
								this,
								modeExt);
				// put that into the modeList
				modeList.push_back(mode);
				// and some debug information
				std::cout << "added mode (@ " << ((void*)mode) << ") "
						<< mode->Width << "x" << mode->Height << "_"
						<< ((int)mode->Frequency) << " /" << outputNum //screenNum
						<< std::endl;
			}
			xrandr->XRRFreeCrtcInfo(crtc);
		}
		xrandr->XRRFreeOutputInfo(output);
	}
}

void XRandrModeManager::initModes() {
	// wipe the existing list of screen modes
	modeList.clear();

	Display *dpy= NULL;
	//int nmodes;

	// get the display
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		throw ModeException("Couldn't open display.");

	// check for extensions
	int major, event, error;
	if (xinAvail)
		xinAvail = XQueryExtension(dpy, "XINERAMA", &major, &event, &error)
				== True;
	bool glxAvail = XQueryExtension(dpy, "GLX", &major, &event, &error) == True;
	bool rndAvail = XQueryExtension(dpy, "RANDR", &major, &event, &error)
			== True;

	if (!glxAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the GLX extension.");
	}
	if (!rndAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the XRandR extension.");
	}
	if (!xrandr->XRRQueryVersion(dpy, &versionMajor, &versionMinor)) {
		XCloseDisplay(dpy);
		throw ModeException("Couldn't retrieve XRandR version information");
	}
	if (xinAvail) {
		if (!xinerama->XineramaIsActive(dpy))
			xinAvail = false;
	}

	try {
		if (versionMajor > 1 || (versionMajor == 1 && versionMinor >= 2))
		init12Modes(dpy);
		else
		init11Modes(dpy);
		XCloseDisplay(dpy);
	} catch (ModeException &me) {
		// close display in case we received an error
		XCloseDisplay(dpy);
		// then happily delegate the bug crap
		throw me;
	}
}

void XRandrModeManager::cleanup(GfxMode *mode) {
	// delete the RandR<ver>Mode data
	// TODO stupid C++ complains about killing a (possible) *void
	// here ... how to solve it without resorting to explicitely
	// writing it down? WTS ...
	if (mode->UserData != 0)
		delete mode->UserData;
}

XRandrModeManager::~XRandrModeManager() {
	if (resources != NULL)
		xrandr->XRRFreeScreenResources(resources);
}

bool XRandrModeManager::setMode(GfxMode *mode) {
	// check for NULL info
	if (mode == 0)
		return false;
	// if we have no resources available, don't do anything
	if (resources == NULL)
		return false;
	// and don't do anything if the data doesn't belong to us
	if (mode->Manager != this)
		return false;
	// without additional data we can't do anything, either
	if (mode->UserData == 0)
		return false;
	// init display
	Display *dpy= NULL;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		return false;
		//throw ModeException("Couldn't open display");
	// handle mode info according to the version tag
	RandRVersion *version = reinterpret_cast<RandRVersion*>(mode->UserData);
	bool result = false;
	switch (*version) {
	case RANDR_11: {
		std::cout << "loading up RandR 1.1 mode" << std::endl;
		break;
	}
	case RANDR_12: {
		std::cout << "loading up RandR 1.2+ mode" << std::endl;
		RandR12Mode *modeExt = reinterpret_cast<RandR12Mode*>(mode->UserData);
		result = (xrandr->XRRSetCrtcConfig(dpy, resources, modeExt->crtc,
				modeExt->timestamp, (int)mode->OriginX, (int)mode->OriginY,
				modeExt->mode, modeExt->rotation, &(modeExt->output), 1) == 0);
		std::cout << "  crtc #" << modeExt->crtc << ", output #"
				<< modeExt->output << ", mode #" << modeExt->mode << ", result = " << result << std::endl;
		break;
	}
	default: {
		std::cout << "unknown RandR mode info?" << std::endl;
	}
	}
	// close display
	XCloseDisplay(dpy);
	return result;
}

GfxMode *XRandrModeManager::getCurrentMode() {
	return NULL;
}

/*
void XRandrModeManager::setUseXinerama(bool use) {
	useXinerama = use;
}
void XRandrModeManager::setIgnoreGLXTest(bool ignore) {
	ignoreGLXTest = ignore;
}
*/
bool XRandrModeManager::handleX11Event(XEvent *event) {
	// empty for now
	return false;
}
