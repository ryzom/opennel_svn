#include <dlfcn.h>
#include <string>
#include <iostream>
#include "xvidmodemanager.h"
#include "exception.h"
#include "gfxmode.h"
//#include "libfunctions.h"


bool XVidModeManager::initLibraries() {
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
	
	libPath = LIB_XVIDMODE;
	if (libraries.count(libPath))
		libPath = libraries[libPath];
	xvidmode = reinterpret_cast<XVidModeExtensionWrapper*>(wrapperFactory->getInstance(EXTWRAP_XVIDMODE, libPath.c_str()));

	if (xvidmode == 0) {
		std::cerr << "failed to acquire XVidMode wrapper object." << std::endl;
		return false;
	}
	
	return true;
}

void XVidModeManager::initModes() {
	// wipe the existing list of screen modes
	modeList.clear();
	
	if (xvidmode == 0)
		return;

	Display *dpy= NULL;
	int nmodes;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		throw ModeException("Couldn't open display.");

	int major, event, error;
	if (xinAvail)
		xinAvail = XQueryExtension(dpy, "XINERAMA", &major, &event, &error)
				== True;
	Bool glxAvail = XQueryExtension(dpy, "GLX", &major, &event, &error);
	Bool xvmAvail = XQueryExtension(dpy, "XFree86-VidModeExtension", &major, &event, &error);

	if (!glxAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the GLX extension.");
	}
	if (xinAvail) {
		if (!xinerama->XineramaIsActive(dpy))
			xinAvail = False;
	}
	if (!xvmAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the XF86VidMode extension.");
	}

	int screen_num = 1;
	int origin_x = 0;
	int origin_y = 0;
	XineramaScreenInfo *xinInfo= NULL;
	if (xinAvail)
		xinInfo = xinerama->XineramaQueryScreens(dpy, &screen_num);

	int glxVisualAttribList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_RED_SIZE,
			4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, None };

	for (int screen = 0; screen < screen_num; screen++) {
		int screenNum = screen;
		if (xinAvail) {
			screenNum = xinInfo[screen].screen_number;
			origin_x = xinInfo[screen].x_org;
			origin_y = xinInfo[screen].y_org;
		}

		if (!ignoreGLXTest) {
			// first check if we have GLX support on the screen
			XVisualInfo *visualInfo = glXChooseVisual(dpy, screenNum,
					glxVisualAttribList);
			// don't do anything if there isn't
			XFree(visualInfo);
			// kludge, but works as long as we don't access
			// the data visualInfo was pointing to once
			if (visualInfo == NULL) {
				std::cout << "ignoring modes on screen #" << screenNum
						<< std::endl;
				continue;
			}
		}

		// otherwise collect mode information data
		xvidmode->XF86VidModeGetAllModeLines(dpy, screenNum, &nmodes, &ms);
		//nldebug("Available modes %d", nmodes);
		for (int modeNum = 0; modeNum < nmodes; modeNum++) {
			// nVidia returns weird modes, filter them out if nvModeFilter is set
			if (!nvModeFilter || (ms[modeNum]->vdisplay <= ms[modeNum]->vtotal
					&& ms[modeNum]->hdisplay <= ms[modeNum]->htotal)) {
				GfxMode
						*mode = new GfxMode(false, false,
								ms[modeNum]->hdisplay,
								ms[modeNum]->vdisplay,
								24,
								// round up for values > x.5
								(uint8_t)((2000 * ms[modeNum]->dotclock /
												(ms[modeNum]->htotal * ms[modeNum]->vtotal)+1)/2),
								0,
								screenNum,
								origin_x,
								origin_y,
								this,
								ms[modeNum]);
				modeList.push_back(mode);
				std::cout << "added mode (@ " << ((void*)mode) << ") "
						<< mode->Width << "x" << mode->Height << "_"
						<< ((int)mode->Frequency) << " /" << screenNum
						<< std::endl;
			}
		}

	}
	if (xinAvail)
		XFree(xinInfo);

	XCloseDisplay(dpy);
}

XVidModeManager::~XVidModeManager() {
	if (ms != NULL)
		XFree(ms);
}

bool XVidModeManager::setMode(GfxMode *mode) {
	if (mode == NULL)
		return false;
	if (mode->Manager != this)
		return false;
	if (xvidmode == 0)
		return false;

	Display *dpy= NULL;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		return false;
		//throw ModeException("Couldn't open display.");
	Bool result = xvidmode->XF86VidModeSwitchToMode(dpy, (int)mode->CRTC, (XF86VidModeModeInfo*)mode->UserData);
	if (result)
		xvidmode->XF86VidModeSetViewPort(dpy, (int)mode->CRTC, 0, 0); //mode->OriginX, mode->OriginY);
	XCloseDisplay(dpy);
	return result == True;
}

GfxMode *XVidModeManager::getCurrentMode() {
	return NULL;
}

void XVidModeManager::setNvidiaModeFilter(bool filter) {
	nvModeFilter = filter;
}

void XVidModeManager::setXineramaAvailable(bool avail) {
	xinAvail = avail;
}
void XVidModeManager::setIgnoreGLXTest(bool ignore) {
	ignoreGLXTest = ignore;
}
