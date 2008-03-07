#include "xvidmodemanager.h"
#include "modeexception.h"
#include "gfxmode.h"
#include "libfunctions.h"
#include <dlfcn.h>
#include <string>
#include <iostream>

#define USE_NVIDIA_FIX 1

bool XVidModeManager::initXineramaLibrary()
{
	// assume we have Xinerama as default
	xinAvail = True;
	std::string libPath = LIB_XINERAMA;
	if (libraries.count(libPath))
		libPath = libraries[libPath];
	if (libXineramaHandle != NULL && dlclose(libXineramaHandle)) {
		// TODO spit out warning/error log here
		std::cerr << "can't close existing libXineramaHandle." << std::endl;
		return false;
	}
	libXineramaHandle = dlopen(libPath.c_str(), RTLD_NOW);
	if (libXineramaHandle == NULL) {
		// TODO warning/error log
		std::cerr << "dlopen(" << libPath << ", RTLD_NOW) failed." << std::endl;
		return false;
	}
	dlerror();
	fnXineramaIsActive = (fnXineramaIsActive_t)dlsym(libXineramaHandle, FN_XINERAMA_IS_ACTIVE);
	char *errMsg = dlerror();
	if (errMsg != NULL) {
		// TODO warning/error log
		// no need to clean up libXineramaHandle, dtor or recall
		// of initLibraries() will do that for us
		std::cerr << ("dlsym([xinerama], " FN_XINERAMA_IS_ACTIVE ") failed.") << std::endl;
		return false;
	}
	fnXineramaQueryScreens = (fnXineramaQueryScreens_t)dlsym(libXineramaHandle,
			FN_XINERAMA_QUERY_SCREENS);
	errMsg = dlerror();
	if (errMsg != NULL) {
		// TODO warning/error log
		std::cerr << ("dlsym([xinerama], " FN_XINERAMA_QUERY_SCREENS ") failed.") << std::endl;
		return false;
	}
	return true;
}

bool XVidModeManager::initLibraries()
{
	// Xinerama is optional, which means initializing
	// the library may fail, which results in a disabled
	// Xinerama extension
	if (!initXineramaLibrary())
		xinAvail = False;
	
	// XVidMode stuff
	std::string libPath = LIB_XVIDMODE;
	if (libraries.count(libPath))
		libPath = libraries[libPath];
	if (libXVidModeHandle != NULL && dlclose(libXVidModeHandle)) {
		// TODO warning/error log
		std::cerr << "can't close existing libXVidModeHandle." << std::endl;
		return false;
	}
	libXVidModeHandle = dlopen(libPath.c_str(), RTLD_NOW);
	if (libXVidModeHandle == NULL) {
		// TODO warning/error log
		std::cerr << "dlopen(" << libPath << ", RTLD_NOW) failed." << std::endl;
		return false;
	}
	dlerror();
	fnXF86VidModeGetAllModeLines =
		(fnXF86VidModeGetAllModeLines_t)dlsym(libXVidModeHandle,
											FN_XVIDMODE_GET_ALL_MODE_LINES);
	char *errMsg = dlerror();
	if (errMsg != NULL) {
		// TODO warning/error log
		std::cerr << ("dlsym([xvidmode], " FN_XVIDMODE_GET_ALL_MODE_LINES ") failed.") << std::endl;
		return false;
	}
	fnXF86VidModeSwitchToMode =
		(fnXF86VidModeSwitchToMode_t)dlsym(libXVidModeHandle,
				FN_XVIDMODE_SWITCH_TO_MODE);
	if (errMsg != NULL) {
		// TODO warning/error log
		std::cerr << ("dlsym([xvidmode], " FN_XVIDMODE_SWITCH_TO_MODE ") failed.") << std::endl;
		return false;
	}
	fnXF86VidModeSetViewPort =
		(fnXF86VidModeSetViewPort_t)dlsym(libXVidModeHandle,
				FN_XVIDMODE_SET_VIEW_PORT);
	if (errMsg != NULL) {
		// TODO warning/error log
		std::cerr << ("dlsym([xvidmode], " FN_XVIDMODE_SET_VIEW_PORT ") failed.") << std::endl;
		return false;
	}
	return true;
}

void XVidModeManager::initModes()
{
	// wipe the existing list of screen modes
	modeList.clear();
	
	Display *dpy= NULL;
	int nmodes;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		throw ModeException("Couldn't open display.");

	int major, event, error;
	if (xinAvail)
		xinAvail = XQueryExtension(dpy, "XINERAMA", &major, &event, &error);
	Bool glxAvail = XQueryExtension(dpy, "GLX", &major, &event, &error);
	Bool xvmAvail = XQueryExtension(dpy, "XFree86-VidModeExtension", &major, &event, &error);

	if (!glxAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the GLX extension.");
	}
	if (xinAvail) {
		if (!(*fnXineramaIsActive)(dpy))
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
		xinInfo = (*fnXineramaQueryScreens)(dpy, &screen_num);

	int glxVisualAttribList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_RED_SIZE,
			4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, None };

	for (int screen = 0; screen < screen_num; screen++) {
		int screenNum = screen;
		if (xinAvail) {
			screenNum = xinInfo[screen].screen_number;
			origin_x = xinInfo[screen].x_org;
			origin_y = xinInfo[screen].y_org;
		}

		// first check if we have GLX support on the screen
		XVisualInfo *visualInfo = glXChooseVisual(dpy, screenNum,
				glxVisualAttribList);
		// don't do anything if there isn't
		XFree(visualInfo);
		// kludge, but works as long as we don't access
		// the data visualInfo was pointing to once
		if (visualInfo == NULL)
			continue;

		// otherwise collect mode information data
		(*fnXF86VidModeGetAllModeLines)(dpy, screenNum, &nmodes, &ms);
		//nldebug("Available modes %d", nmodes);
		for (int modeNum = 0; modeNum < nmodes; modeNum++) {
			// nVidia returns weird modes, filter them out
#ifdef USE_NVIDIA_FIX			
			if (ms[modeNum]->vdisplay <= ms[modeNum]->vtotal
					&& ms[modeNum]->hdisplay <= ms[modeNum]->htotal) {
#endif				
				GfxMode *mode = new GfxMode(false, false,
						ms[modeNum]->hdisplay,
						ms[modeNum]->vdisplay,
						24,
						(uint8_t)(1000 * ms[modeNum]->dotclock /
								(ms[modeNum]->htotal * ms[modeNum]->vtotal)),
						0,
						screenNum,
						origin_x,
						origin_y,
						this,
						ms[modeNum]);
				modeList.push_back(mode);
				std::cout << "added mode (@ " << ((void*)mode) << ") " << mode->Width << "x" << mode->Height << "_" << ((int)mode->Frequency)
						<< " on screen " << screenNum << std::endl;
#ifdef USE_NVIDIA_FIX				
			}
#endif			
		}

	}
	if (xinAvail)
		XFree(xinInfo);

	XCloseDisplay(dpy);
}

XVidModeManager::XVidModeManager()
{
	ms = NULL;
	xinAvail = 0;
	libXineramaHandle = NULL;
	libXVidModeHandle = NULL;
	fnXineramaIsActive = NULL;
	fnXineramaQueryScreens = NULL;
	fnXF86VidModeGetAllModeLines = NULL;
	fnXF86VidModeSwitchToMode = NULL;
	fnXF86VidModeSetViewPort = NULL;
}

XVidModeManager::~XVidModeManager()
{
	if (libXineramaHandle != NULL) {
		dlclose(libXineramaHandle);
		dlerror(); // "eat" error if there was one
	}
	if (libXVidModeHandle != NULL) {
		dlclose(libXVidModeHandle);
		dlerror(); // "eat" error if there was one
	}
	if (ms != NULL)
		XFree(ms);
}

void XVidModeManager::setMode(GfxMode *mode)
{
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
}

GfxMode *XVidModeManager::getCurrentMode()
{
	return NULL;
}
