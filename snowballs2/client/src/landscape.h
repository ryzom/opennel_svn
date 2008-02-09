/**
 * \file landscape.h
 * \brief CLandscape
 * \date 2008-02-03 20:52GMT
 * \author Jan Boon (Kaetemi)
 * CLandscape
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS, Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of OpenNeL Snowballs.
 * OpenNeL Snowballs is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * OpenNeL Snowballs is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenNeL Snowballs; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_LANDSCAPE_H
#define SBCLIENT_LANDSCAPE_H
#include <nel/misc/types_nl.h>

// Project includes
#include "config_proxy.h"
#include "member_callback_decl.h"

// NeL includes
#include <nel/3d/animation_time.h>

// STL includes

//#define SBCLIENT_BLOOM_TEST

#ifdef SBCLIENT_BLOOM_TEST
#include <nel/3d/bloom_effect.h>
#endif

namespace NLPACS {
	class UMovePrimitive;
}

namespace NLMISC {
	class IProgressCallback;
	class CVector;
}

namespace NL3D {
	class UScene;
	class ULandscape;
	class UDriver;
}

namespace SBCLIENT {

/**
 * \brief CLandscape
 * \date 2008-02-03 20:52GMT
 * \author Jan Boon (Kaetemi)
 * CLandscape
 */
class CLandscape
{
public:
	// pointers
	NL3D::UDriver *_Driver; // p
	NL3D::UScene *Scene; // P
	NL3D::ULandscape *Landscape; // P

	/// The landscape is updated around this vector
	NLMISC::CVector *RefreshZonesAround; // p
protected:
	/// Must always point to the animation time (NOT THE DELTA)
	NL3D::TGlobalAnimationTime *_GlobalAnimationTime; // p

	// instances
	CConfigProxy _Config;	
#ifdef SBCLIENT_BLOOM_TEST
	NL3D::CBloomEffect _BloomEffect;
#endif
private:
	float _Vision;
	float _VisionInitial;
public:
	CLandscape(NLMISC::IProgressCallback &progressCallback, const std::string &id, NL3D::UDriver *driver, NL3D::TGlobalAnimationTime *globalAnimationTime);
	virtual ~CLandscape();

	/// Update the scene animations
	SBCLIENT_CALLBACK_DECL(updateAnimations);
	
	/// Refresh the zones around the entity
	SBCLIENT_CALLBACK_DECL(updateLandscape);

	/// Render the scene and landscape
	SBCLIENT_CALLBACK_DECL(renderScene);

	/// Start and end of bloom effect
	SBCLIENT_CALLBACK_DECL(initBloom);
	SBCLIENT_CALLBACK_DECL(endBloom);

	/// Force refresh for use within a loading/teleporting screen
	void refresh(NLMISC::IProgressCallback &progressCallback);

private:
	SBCLIENT_CALLBACK_CONFIG_DECL(configReceiveShadowMap);
	SBCLIENT_CALLBACK_CONFIG_DECL(configTileNear);
	SBCLIENT_CALLBACK_CONFIG_DECL(configThreshold);
	SBCLIENT_CALLBACK_CONFIG_DECL(configVision);
	SBCLIENT_CALLBACK_CONFIG_DECL(configVisionInitial);
}; /* class CLandscape */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LANDSCAPE_H */

/* end of file */
