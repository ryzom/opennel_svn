/**
 * \file lens_flare.h
 * \brief CLensFlare
 * Generic code for managing the lens flare.
 * This code was taken and modified from Snowballs 2 specific code.
 * This code was taken from Snowballs 1.
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_LENS_FLARE_H
#define SBCLIENT_LENS_FLARE_H
#include <nel/misc/types_nl.h>

#include <nel/misc/vector.h>
#include <nel/3d/u_material.h>
#include <nel/3d/u_camera.h>
#include <nel/misc/bitmap.h>

namespace NL3D {
	class UDriver;
}

namespace SBCLIENT {

/**
 * \brief CLensFlare
 * \author Stephane Coutelas
 * \author Nevrax France
 * \date 2000
 * CLensFlare
 * Modified by Jan Boon (Kaetemi) on 2008-01-09 19:52GMT
 */
class CLensFlare
{
	struct _CFlare
	{
		NL3D::UMaterial Material;
		float Width;
		float Height;
		float Location;
		float Scale;
	};
	typedef std::vector<_CFlare> _CFlares;

protected:
	// pointers
	NL3D::UDriver *_Driver;
	
	// instances
	/** _AlphaCoef */
	float _AlphaCoef;
	/** _MaxLensFlareLenght */
	float _MaxLensFlareLenght;
	/** _SunRadius */
	float _SunRadius;
	/** flares due to light */
	_CFlares _Flares;
	/** direction of the sun */
	NLMISC::CVector _SunDirection;
	/** used camera */
	NL3D::UCamera _Camera;
	/** materials */
	NL3D::UMaterial _Pink;
	NL3D::UMaterial _Original;
	NL3D::UMaterial _Dazzle;
	/** things for the d3d workaround */
	NLMISC::CBitmap _BitmapPink;
	NLMISC::CBitmap _BitmapOriginal;
public:
	CLensFlare(NL3D::UDriver *driver, const NL3D::UCamera &camera);
	virtual ~CLensFlare();
	
	void addFlare(NL3D::UTexture *texture, float width, float height, float location = 1.0f, float scale = 1.0f);
	void setAlphaCoef(float coef) { _AlphaCoef = coef; }
	void setSunDirection(const NLMISC::CVector &direction) { _SunDirection = direction; }
	void setCamera(const NL3D::UCamera &camera) { _Camera = camera; }
	/** 
		Shows the lens flare. 
		You should call this after all 3d has been rendered,
		and before you start rendering your user interface.
	*/
	void show();
}; /* class CLensFlare */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LENS_FLARE_H */

/* end of file */
