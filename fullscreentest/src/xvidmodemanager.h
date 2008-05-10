#ifndef XVIMODEMANAGER_H_
#define XVIMODEMANAGER_H_

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/Xinerama.h>
#include <GL/glx.h>

#include "modemanager.h"
#include "xvidmodeextwrapper.h"
#include "xineramaextwrapper.h"
#include "propdefs.h"

/**
 * XVidModeManager folds the XF86VidMode extension into the GfxMode
 * struct NeL uses to manage video modes. If available, it relies on
 * Xinerama to provide additional information about the different
 * displays.
 */
class XVidModeManager : public ModeManager
{
public:
	XVidModeManager(ExtensionWrapperFactory *fac) : ModeManager(fac), xinAvail(true),
		ms(NULL), xvidmode(0), xinerama(0) {
		properties[P_USE_GLX_TEST] = true;
		properties[P_USE_XINERAMA] = true;
		properties[P_USE_MODE_FILTER] = true;
	}

	virtual ~XVidModeManager();
	
	virtual bool setMode(GfxMode *mode);
	virtual bool initLibraries();
	virtual void initModes();
	virtual GfxMode *getCurrentMode();
	
private:
	bool xinAvail;
	XF86VidModeModeInfo **ms;
	XVidModeExtensionWrapper *xvidmode;
	XineramaExtensionWrapper *xinerama;
};

#endif /*XVIMODEMANAGER_H_*/
