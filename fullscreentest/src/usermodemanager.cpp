
#include "usermodemanager.h"
#include "exception.h"

UserModeManager::~UserModeManager() {
	// do nothing atm
}

void UserModeManager::cleanup(GfxMode *mode) {
	// nothing to do here
}

bool UserModeManager::initLibraries() {
	// Xinerama is optional, which means initializing
	// the library may fail, which results in a disabled
	// Xinerama extension
	std::string libPath;
	libPath = LIB_XINERAMA;
	if (libraries.count(libPath))
		libPath = libraries[libPath];
	xinerama = reinterpret_cast<XineramaExtensionWrapper*>(wrapperFactory->getInstance(EXTWRAP_XINERAMA, libPath.c_str()));
	if (xinerama == 0)
		std::cerr << "failed to load Xinerama wrapper object. Disabling Xinerama usage." << std::endl;
	xinAvail = (xinerama != 0);
	return true;
}

void UserModeManager::initModes() {
	// wipe the existing list of screen modes
	modeList.clear();
	
	bool useXin = properties[P_USE_XINERAMA] && xinAvail;
	//bool useGlxTest = properties[P_USE_GLX_TEST];
	char **userModes = properties[P_USER_MODES];
	
	Display *dpy= NULL;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		throw ModeException("Couldn't open display.");

	int major, event, error;
	if (useXin)
		useXin = XQueryExtension(dpy, "XINERAMA", &major, &event, &error) == True;
	Bool glxAvail = XQueryExtension(dpy, "GLX", &major, &event, &error);

	if (!glxAvail) {
		XCloseDisplay(dpy);
		throw ModeException("The server doesn't support the GLX extension.");
	}
	if (useXin) {
		if (!xinerama->XineramaIsActive(dpy))
			useXin = false;
	}

	XineramaScreenInfo *xinInfo= 0; //NULL;
	int xinScreenCount = 1;
	if (useXin) {
		xinInfo = xinerama->XineramaQueryScreens(dpy, &xinScreenCount);
		useXin = xinInfo != 0; //NULL;
	}
	int screenCount = xinScreenCount;
	
	// screen width and height for the default screen only
	for (int screen = 0; screen < screenCount || (userModes != 0 && userModes[screen] != 0); screen++) {
		int width, height, origin_x, origin_y;
		int screenNum;
		
		if (useXin && screen < xinScreenCount) {
			screenNum = xinInfo[screen].screen_number;
			width = xinInfo[screen].width;
			height = xinInfo[screen].height;
			origin_x = xinInfo[screen].x_org;
			origin_y = xinInfo[screen].y_org;
		} else {
			screenNum = DefaultScreen(dpy);
			width = DisplayWidth(dpy, screenNum);
			height = DisplayHeight(dpy, screenNum);
			origin_x = 0;
			origin_y = 0;
		}
		int depth = DefaultDepth(dpy, screenNum);
		
		// user modes are run relative to the "hardware" screen
		// information, either from
		if (userModes != 0 && userModes[screen] != 0) {
			int x, y;
			unsigned int w, h;
			int bitmask = XParseGeometry(userModes[screen], &x, &y, &w, &h);
			
			if (!(bitmask & WidthValue))
				w = width;
			if (!(bitmask & HeightValue))
				h = height;
			if (bitmask & XValue) {
				if (bitmask & XNegative)
					x = origin_x + width + x - w;
				else
					x += origin_x;
			}
			if (bitmask & YValue) {
				if (bitmask & YNegative)
					y = origin_y + height + y - h;
				else
					y += origin_y;
			}
			width = w;
			height = h;
			origin_x = x;
			origin_y = y;
		}
		
		GfxMode	*mode = new GfxMode(false, false,
						width,
						height,
						depth,
						60, // there is no more info available, so 60Hz
						0,
						screenNum,
						origin_x,
						origin_y,
						this,
						0); // no UserInfo needed, just resize the window properly
		modeList.push_back(mode);
		std::cout << "added mode (@ " << ((void*)mode) << ") "
				<< mode->Width << "x" << mode->Height << "_"
				<< ((int)mode->Frequency) << " /" << screenNum
				<< std::endl;

	}
	
	if (useXin)
		XFree(xinInfo);

	XCloseDisplay(dpy);
}

GfxMode *UserModeManager::getCurrentMode() {
	return 0;
}

bool UserModeManager::setMode(GfxMode *mode) {
	// nothing to do here, actually, as we don't switch modes
	std::cout << "setting mode " << mode->Width << "x" << mode->Height << "@"
		<< mode->OriginX << "," << mode->OriginY << "/" << (int)(mode->CRTC) << std::endl;
	return true;
}

bool UserModeManager::isScreenGLXCapable(Display *dpy, int screen) {
	// check if we are even supposed to check GLX
	bool useGlxTest = properties[P_USE_GLX_TEST];
	// if not, assume everything works ...
	if (!useGlxTest)
		return true;
	
	// ... otherwise lets check for a GLX visual
	int glxVisualAttribList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_RED_SIZE,
			4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, None };
	
	// first check if we have GLX support on the screen
	XVisualInfo *visualInfo = glXChooseVisual(dpy, screen, glxVisualAttribList);
	// don't do anything if there isn't
	XFree(visualInfo);
	// kludge, but works as long as we don't access
	// the data visualInfo was pointing to once
	return visualInfo != 0; //NULL;
}
