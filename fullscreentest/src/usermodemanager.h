#ifndef USERMODEMANAGER_H_
#define USERMODEMANAGER_H_

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <GL/glx.h>

#include "modemanager.h"
#include "extensionwrapper.h"
#include "xineramaextwrapper.h"
#include "propdefs.h"

/**
 * \brief ModeManager using user defined "screen modes",
 *        defaulting to the informations returned by Xinerama
 *        and XWindows, if no Xinerama is running.
 * 
 * This ModeManager is the backup solution if the other
 * ModeManagers don't work properly. It doesn't change the
 * already configured resolutions; instead it retrieves
 * user supplied geometry information and sizes the "fullscreen"
 * mode(s) respectively. The data of the different CRTCs
 * is preinitialized with information from Xinerama, if
 * available, otherwise the geometry of the root window
 * is used for that purpose.
 * 
 * The data users have to supply is a list of geometry strings
 * (see XParseGeometry(3) [man]), one for each screen, or
 * an empty string for using the default "screen mode".
 * This list has to end with NULL/0, similar to the argv
 * command line argument list. The user mode list has to be
 * supplied via the property P_USER_MODES.
 * 
 * The user mode information is run _relative_ to the screen
 * information provided by X / Xinerama. That means with two
 * displays and running Xinerama the first two modes are
 * relative to the display sizes returned by Xinerama, while
 * the modes following it are relative to the X default
 * screen.
 */
class UserModeManager : public ModeManager {
public:
	UserModeManager(ExtensionWrapperFactory *fac) :
		ModeManager(fac), xinAvail(true), xinerama(0) {
		properties[P_USER_MODES] = (char**)0;
		properties[P_USE_XINERAMA] = true;
		properties[P_USE_GLX_TEST] = true;
	}
	virtual ~UserModeManager();

	virtual bool setMode(GfxMode *mode);
	virtual bool initLibraries();
	virtual void initModes();
	virtual GfxMode *getCurrentMode();
	virtual void cleanup(GfxMode *mode);

private:
	bool xinAvail;
	XineramaExtensionWrapper *xinerama;
	
	// returns true, if GLX test is either disabled or
	// GLX init worked
	bool isScreenGLXCapable(Display *dpy, int screen);
	
};

#endif /*USERMODEMANAGER_H_*/
