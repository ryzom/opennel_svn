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

class XVidModeManager : public ModeManager
{
public:
	XVidModeManager(ExtensionWrapperFactory *fac) : ModeManager(fac), xinAvail(true),
		nvModeFilter(true), ignoreGLXTest(false), ms(NULL), xvidmode(0), xinerama(0) {}

	virtual ~XVidModeManager();
	
	virtual bool setMode(GfxMode *mode);
	virtual bool initLibraries();
	virtual void initModes();
	virtual GfxMode *getCurrentMode();
	
	void setNvidiaModeFilter(bool filter);
	void setXineramaAvailable(bool avail);
	void setIgnoreGLXTest(bool ignore);
	
private:
	bool xinAvail;
	bool nvModeFilter;
	bool ignoreGLXTest; 
	XF86VidModeModeInfo **ms;
	XVidModeExtensionWrapper *xvidmode;
	XineramaExtensionWrapper *xinerama;
};

#endif /*XVIMODEMANAGER_H_*/
