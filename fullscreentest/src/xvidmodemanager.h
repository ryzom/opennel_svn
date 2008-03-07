#ifndef XVIMODEMANAGER_H_
#define XVIMODEMANAGER_H_

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/Xinerama.h>
#include <GL/glx.h>

#include "modemanager.h"
#include "libfunctions.h"

class XVidModeManager : public ModeManager
{
public:
	XVidModeManager();
	virtual ~XVidModeManager();
	
	virtual void setMode(GfxMode *mode);
	virtual bool initLibraries();
	virtual void initModes();
	virtual GfxMode *getCurrentMode();
	
private:
	bool initXineramaLibrary();
	
	Bool xinAvail;
	XF86VidModeModeInfo **ms;
	void *libXineramaHandle;
	void *libXVidModeHandle;
	fnXF86VidModeGetAllModeLines_t fnXF86VidModeGetAllModeLines;
	fnXF86VidModeSwitchToMode_t fnXF86VidModeSwitchToMode;
	fnXF86VidModeSetViewPort_t fnXF86VidModeSetViewPort;
	fnXineramaIsActive_t fnXineramaIsActive;
	fnXineramaQueryScreens_t fnXineramaQueryScreens;

};

#endif /*XVIMODEMANAGER_H_*/
