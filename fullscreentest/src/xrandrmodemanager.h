#ifndef XRANDRMODEMANAGER_H_
#define XRANDRMODEMANAGER_H_

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <GL/glx.h>

#include "modemanager.h"
#include "extensionwrapper.h"
#include "xrandrextwrapper.h"
#include "xineramaextwrapper.h"

/*
 What is still needed:
 - a generic concept of the X11 window handling
 - extraction of the X11 event handling
 */

/**
 * \brief ModeManager using XRandR extension, with optional
 *        access to Xinerama extension.
 * 
 * XRandrModeManager uses the XRandR extension to determine
 * the available screen modes, including multiscreen support,
 * if the XRandR version allows for it (1.2+). It can access
 * Xinerama, too, which might be useful for earlier versions
 * of XRandR; this feature is disabled by default, though.
 */
class XRandrModeManager : public ModeManager {
public:
	XRandrModeManager(ExtensionWrapperFactory *fac) :
		ModeManager(fac), versionMajor(0), versionMinor(0), baseEventCode(0),
				useXinerama(false), xinAvail(true), ignoreGLXTest(false),
				xrandr(0), xinerama(0), resources(NULL) { }
	virtual ~XRandrModeManager();

	virtual bool setMode(GfxMode *mode);
	virtual bool initLibraries();
	virtual void initModes();
	virtual GfxMode *getCurrentMode();
	virtual void cleanup(GfxMode *mode);

	// enforce Xinerama use (disabled by default)
	//void setUseXinerama(bool use);
	// standard, probably contained in every manager
	//void setIgnoreGLXTest(bool ignore);

	// call this to handle XRandr events
	bool handleX11Event(XEvent *event);

private:
	int versionMajor, versionMinor;
	int baseEventCode;
	bool useXinerama;
	bool xinAvail;
	bool ignoreGLXTest;
	XRandRExtensionWrapper *xrandr;
	XineramaExtensionWrapper *xinerama;
	
	XRRScreenResources *resources;
	
	// uses RandR 1.1 + Xinerama functions to fill mode list
	void init11Modes(Display *dpy);
	// uses RandR 1.2+ functions to fill mode list
	void init12Modes(Display *dpy);

	enum RandRVersion { RANDR_11, RANDR_12 };
	
	struct RandR11Mode {
		RandRVersion version;
	};
	
	/**
	 * \brief Container to keep all the mode data not contained in
	 *        GfxMode (which will be stored as a pointer in userdata
	 *        section).
	 * 
	 * This data is RandR 1.2+ specific and used for the
	 * XRRSetCrtcConfig() function.
	 */
	struct RandR12Mode {
		RandRVersion version;
		RRCrtc   crtc;
		Time     timestamp;
		RRMode   mode;
		RROutput output;
		Rotation rotation;
	};
	
};

#endif /*XRANDRMODEMANAGER_H_*/
