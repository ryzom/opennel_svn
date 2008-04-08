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
		xinerama = reinterpret_cast<XineramaExtensionWrapper*>(wrapperFactory->getInstance(EXTWRAP_XINERAMA, libPath.c_str()));
		if (xinerama == 0)
			std::cerr << "failed to load Xinerama wrapper object. Disabling Xinerama usage." << std::endl;
		xinAvail = (xinerama != 0);
	}
	
	libPath = LIB_XRANDR;
	if (libraries.count(libPath))
		libPath = libraries[libPath];
	xrandr = reinterpret_cast<XRandRExtensionWrapper*>(wrapperFactory->getInstance(EXTWRAP_XRANDR, libPath.c_str()));

	if (xrandr == 0) {
		std::cerr << "failed to acquire XRandR wrapper object." << std::endl;
		return false;
	}
	
	return true;
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
	Bool glxAvail = XQueryExtension(dpy, "GLX", &major, &event, &error);
    Bool rndAvail = XQueryExtension(dpy, "RANDR", &major, &event, &error);

    if (!glxAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the GLX extension.");
	}
    if (rndAvail) {
        if (!xrandr->XRRQueryVersion(dpy, &versionMajor, &versionMinor)) {
        	XCloseDisplay(dpy);
        	throw ModeException("Couldn't retrieve XRandR version information");
        }
        if (versionMajor > 1 || (versionMajor == 1 && versionMinor >= 2))
        	// TODO on NVidia, Xinerama returns more correct results than XRandR 1.2 >.<
        	xinAvail = false;
    }
    else {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the XRandR extension.");
	}
	if (xinAvail) {
		if (!xinerama->XineramaIsActive(dpy))
			xinAvail = false;
	}

	/*
	int numSize;
    XRRScreenSize *size = xrandr->XRRSizes(dpy, screenNum, &numSize);
    printf("XRandR info\n");
    for (int i = 0; i < numSize; i++) {
        printf("Screen #%d, size %d: width %d height %d rate",
               screenNum, i,
               size[i].width, size[i].height);
        int numRates;
        short *rates = xrandr->XRRRates(dpy, screenNum, i, &numRates);
        for (int j = 0; j < numRates; j++) {
            printf(" %d", rates[j]);
        }
        printf("\n");
    }
	*/
	
	//int screenNum = 0;
	// all the following code is based on RandR 1.2+
	if (versionMajor < 1 || (versionMajor == 1 && versionMinor < 2)) {
		XCloseDisplay(dpy);
		return;
	}
    Window root = RootWindow(dpy, 0); //, screenNum);
    XWindowAttributes windowAttributes;
    int depth = 24;
    if (XGetWindowAttributes(dpy, root, &windowAttributes) != 0) {
    	depth = windowAttributes.depth;
    }
    XRRScreenResources *res = xrandr->XRRGetScreenResources(dpy, root);
    std::cout << "resource with " << res->ncrtc << " crtcs, " << res->noutput << " outputs and "
    	<< res->nmode << " modes" << std::endl;
    
    // load mode list into a map for quick access
    std::map<int, XRRModeInfo*> modeInfoList; // (res->nmode);
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo *mode = res->modes + i;
        modeInfoList[mode->id] = mode;
    }
    /*
    for (int i = 0; i < res->ncrtc; i++) {
        XRRCrtcInfo *crtc = xrandr->XRRGetCrtcInfo(dpy, res, res->crtcs[i]);
        std::cout "crtc #%d id %d: %d x %d @ (%d, %d), mode %d, with %d outputs:",
               i,
               res->crtcs[i],
               crtc->width, crtc->height,
               crtc->x, crtc->y,
               crtc->mode,
               crtc->noutput);
        for (int j = 0; j < crtc->noutput; j++)
            printf(" %d", crtc->outputs[j]);
        printf("\n");
        xrandr->XRRFreeCrtcInfo(crtc);
    }
    */
    // go through all outputs, determine default CRTC for it and
    // assign all modes it supports to it
    for (int outputNum = 0; outputNum < res->noutput; outputNum++) {
    	XRROutputInfo *output = xrandr->XRRGetOutputInfo(dpy, res, res->outputs[outputNum]);
        XRRCrtcInfo *crtc = xrandr->XRRGetCrtcInfo(dpy, res, output->crtc);
        for (int modeNum = 0; modeNum < output->nmode; modeNum++) {
        	int modeIndex = output->modes[modeNum];
			GfxMode
					*mode = new GfxMode(false, false,
							modeInfoList[modeIndex]->width,
							modeInfoList[modeIndex]->height,
							depth, // same for all?
							// round up for values > x.5
							(uint8_t)((2000 * modeInfoList[modeIndex]->dotClock /
											(modeInfoList[modeIndex]->hTotal * modeInfoList[modeIndex]->vTotal)+1)/2),
							0,
							outputNum,
							crtc->x, // origin_x,
							crtc->y, // origin_y,
							this,
							modeInfoList[modeIndex]);
			modeList.push_back(mode);
			std::cout << "added mode (@ " << ((void*)mode) << ") "
					<< mode->Width << "x" << mode->Height << "_"
					<< ((int)mode->Frequency) << " /" << outputNum //screenNum
					<< std::endl;
        }
        xrandr->XRRFreeCrtcInfo(crtc);
   		xrandr->XRRFreeOutputInfo(output);
    }
    xrandr->XRRFreeScreenResources(res);

    //if (xinAvail)
	//	XFree(xinInfo);

	XCloseDisplay(dpy);
}

XRandrModeManager::~XRandrModeManager() {
}

void XRandrModeManager::setMode(GfxMode *mode) {
	return;
	/*
	if (mode == NULL)
		return;
	if (mode->Manager != this)
		return;

	Display *dpy= NULL;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		throw ModeException("Couldn't open display.");
	Bool result = (*fnXF86VidModeSwitchToMode)(dpy, (int)mode->CRTC, (XF86VidModeModeInfo*)mode->UserData);
	if (result)
		(*fnXF86VidModeSetViewPort)(dpy, (int)mode->CRTC, 0, 0); //mode->OriginX, mode->OriginY);
	XCloseDisplay(dpy);
	// return result == TRUE; ?
	*/
}

GfxMode *XRandrModeManager::getCurrentMode() {
	return NULL;
}

void XRandrModeManager::setUseXinerama(bool use) {
	useXinerama = use;
}
void XRandrModeManager::setIgnoreGLXTest(bool ignore) {
	ignoreGLXTest = ignore;
}

bool XRandrModeManager::handleX11Event(XEvent *event) {
	// empty for now
	return false;
}
