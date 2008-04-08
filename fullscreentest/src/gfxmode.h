#ifndef GFXMODE_H_
#define GFXMODE_H_

struct GfxMode;

#include <stdint.h>
#include "modemanager.h"

struct GfxMode
{
public:
	GfxMode(bool offscreen, bool windowed, uint16_t width, uint16_t height,
			uint8_t depth, uint8_t frequency, int8_t antialias, int8_t crtc,
			uint32_t originx, uint32_t originy, ModeManager *manager, void *userdata) :
				OffScreen(offscreen), Windowed(windowed), Width(width), Height(height),
				Depth(depth), Frequency(frequency), AntiAlias(antialias),
				CRTC(crtc), OriginX(originx), OriginY(originy), Manager(manager),
				UserData(userdata)
	{}
	
	bool		OffScreen;	// offscreen display?
	bool		Windowed;	// windowed or fullscreen mode?
	uint16_t	Width;		// width in pixels
	uint16_t	Height;		// height in pixels
	uint8_t		Depth;		// depth in bits per pixel (bpp)
	uint8_t		Frequency;	// vertical retrace frequency in Hz
	int8_t		AntiAlias;	// -1 = no AA, 0 = max, 2 = 2x sample, 4 = 4x sample, ...
	int8_t		CRTC;		// number of the CRTC which is going to receive the mode info
	uint32_t	OriginX;	// origin in regard to the framebuffer in use, X
	uint32_t	OriginY;	// and Y
	ModeManager *Manager;	// the manager for this mode
	void	    *UserData;	// handler specific userdata (i.e. XF86VidModeModeInfo data)
};

#endif /*GFXMODE_H_*/
