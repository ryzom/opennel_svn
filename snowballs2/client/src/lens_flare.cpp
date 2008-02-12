/**
 * \file lens_flare.cpp
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

#include <nel/misc/types_nl.h>
#include "lens_flare.h"

// #include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

//#include "_mouse_listener.h"
//#include <nel/misc/vectord.h> // temp!
//extern C3dMouseListener *MouseListener;

namespace SBCLIENT {

CLensFlare::CLensFlare(NL3D::UDriver *driver, const NL3D::UCamera &camera) 
: _AlphaCoef(1.0f), _MaxLensFlareLenght(0.4f), _Camera(camera), 
_SunRadius(24), _Driver(driver), _SunDirection(CVector::Null)
{
	nlassert(_Driver);

	// create an ugly color to test stuff
	_Pink = _Driver->createMaterial();
    _Pink.initUnlit();
    _Pink.setBlend(true);
	_Pink.setColor(CRGBA(255, 0, 255, 255));
	
	// material to place back original
	_Original = _Driver->createMaterial();
    _Original.initUnlit();
    _Original.setBlend(true);
	_Original.setColor(CRGBA(255, 255, 255, 255));
	_Original.setZWrite(false);
	_Original.setZFunc(UMaterial::always);
	
	// a white glow that fills the screen
	_Dazzle = _Driver->createMaterial();
    _Dazzle.initUnlit();
    _Dazzle.setBlend(true);
	_Dazzle.setColor(CRGBA(255, 0, 0, 255));
	_Dazzle.setZWrite(false);
	_Dazzle.setZFunc(UMaterial::always);
}

CLensFlare::~CLensFlare()
{
	_Driver->deleteMaterial(_Dazzle);
	_Driver->deleteMaterial(_Original);
	_Driver->deleteMaterial(_Pink);

	for(_CFlares::iterator it = _Flares.begin(); it != _Flares.end(); it++)
		_Driver->deleteMaterial(it->Material);
}

/*********************************************************\
					addFlare()
\*********************************************************/
void CLensFlare::addFlare(UTexture *texture, float width, float height, float location, float scale)
{
	_CFlare flare;
	flare.Material = _Driver->createMaterial();
	flare.Material.initUnlit();
	flare.Material.setTexture(texture);
	flare.Material.setBlendFunc(UMaterial::srcalpha, UMaterial::one);
	flare.Material.setBlend(true);
	flare.Material.setZFunc(UMaterial::always);
	flare.Material.setZWrite(false);
	
	// quad dimension
	flare.Width = width;
	flare.Height = height;
	
	// location on the lens-flare ray
	flare.Location = location;
	
	// texture scale
	flare.Scale = scale;
	flare.Width = width;
	flare.Height = height;
	_Flares.push_back(flare);
}

/*********************************************************\
						render()
\*********************************************************/
void CLensFlare::show()
{
	// vector to sun
	//==============
	// CVector userLook = MouseListener->getViewDirection();
	CVector userLook = _Camera.getRotEuler();
	CVector sunDirection = (-100000 * _SunDirection);

	// cosinus between the two previous vectors
	//=========================================
	float cosAngle = sunDirection * userLook / sunDirection.norm();	

	// alpha
	//======
	float alphaf = cosAngle < 0 ? 0.0f : 255 * (float)(pow(cosAngle, 20));

	if (!(alphaf > 0.0f)) return;

	// landscape's masking sun ?
	//==========================
	CMatrix camMatrix;
	camMatrix = _Camera.getMatrix();
	camMatrix.setPos(CVector::Null);
	camMatrix.invert();
	CVector tmp = camMatrix * sunDirection;
	tmp = _Camera.getFrustum().project(tmp);
	uint32 w,h;
	_Driver->getWindowSize(w, h);
	uint32 sun_radius = (uint32)_SunRadius;
	CRect rect(
		(uint32)(tmp.x * (float)w) - sun_radius,
		(uint32)(tmp.y * (float)h) - sun_radius,
		2 * sun_radius,
		2 * sun_radius);
	vector<float> zbuff;
	_Driver->getZBufferPart(zbuff, rect); // !!!!!! not working in d3d apparently .........
	_Driver->setMatrixMode2D11();
	float view = 0.0f;
	if (zbuff.size() == 0) // workaround for direct3d problem (slow!)
	{
		// rect for reading pixels
		CRect pixels(rect.X, 
			h - rect.bottom(), 
			rect.Width, rect.Height);
		
		// quad for drawing rectangles to screen
		float x0 = rect.X / (float)w;
		float y0 = rect.Y / (float)h;
		float x1 = rect.right() / (float)w;
		float y1 = rect.bottom() / (float)h;
		CQuad quad = CQuad(
			CVector(x0, y0, -1), CVector(x1, y0, -1),
			CVector(x1, y1, -1), CVector(x0, y1, -1));
		
		// get the original view, todo: copy to texture to place back		
		_Driver->getBufferPart(_BitmapOriginal, pixels);		 
		CObjectVector<uint8> &pixelsOriginal = _BitmapOriginal.getPixels();
		
		// draw a pink square behind everything else
		_Driver->drawQuad(quad, _Pink);
		_Driver->getBufferPart(_BitmapPink, pixels);
		_BitmapPink.convertToType(CBitmap::RGBA);
		CObjectVector<uint8> &pixelsPink = _BitmapPink.getPixels();
		uint8 *bitsPink = pixelsPink.getPtr();
		
		// compare with pink CRGBA(255, 0, 255, 255))
		uint32 difference = 0;
		uint32 pixel_color_count = (uint)pixelsPink.size();
		for (uint i = 0; i < pixel_color_count; i += 4)
		{
			if (bitsPink[i] != 255 || bitsPink[i + 1] != 0
				|| bitsPink[i + 2] != 255 || bitsPink[i + 3] != 255)
			{
				++difference;
			}
		}
		view = (float)((double)difference / (double)(pixel_color_count / 4));
	}
	else // normal way
	{
		float sum = 0.0f;
		for (uint i = 0; i < zbuff.size(); ++i)
		{
			if (zbuff[i] >= 0.99999f) ++sum;
		}
		view = sum / (_SunRadius * 2 * _SunRadius * 2);
	}

	// quad for dazzle 
	//================
	uint8 alpha = (uint8)(alphaf * view / 2.0f);
	if(alpha != 0)
	{
		_Dazzle.setColor(CRGBA(255, 255, 255, alpha));
		static const CQuad quad(
			CVector(0, 0, 0), CVector(1, 0, 0),
			CVector(1, 1, 0), CVector(0, 1, 0));
		_Driver->drawQuad(quad, _Dazzle);
	}

	// Set the alpha
	setAlphaCoef(1.0f - (float)cos(alphaf * view * Pi / (2.0f * 255.0f)));

	// Display the lens flare

	CMatrix mtx;
	mtx.identity();

	nlassert(_Driver != NULL && !_Camera.empty());

	_Driver->setMatrixMode2D11();

	// Determining axis "screen center - light" vector
	CMatrix cameraMatrix = _Camera.getMatrix();
	cameraMatrix.invert();
	CVector light = (-100000 * _SunDirection);
	light = cameraMatrix * light;
	light = _Camera.getFrustum().project(light);
	
	CVector screenCenter(0.5f,0.5f,0);
	CVector axis = light - screenCenter;

	if(axis.norm()>_MaxLensFlareLenght)
	{
		return;
	}

	// rendering flares
	for(_CFlares::iterator itflr = _Flares.begin(); itflr != _Flares.end(); itflr++)
	{
		itflr->Material.setColor(CRGBA(255,255,255,(uint8)(_AlphaCoef*255)));
			
		CQuadUV quad;
		
		float xCenterQuad = screenCenter.x + itflr->Location * axis.x;
		float yCenterQuad = screenCenter.y + itflr->Location * axis.y;
		
		float x,y;


		x = xCenterQuad - itflr->Width * itflr->Scale / 2.f;
		y = yCenterQuad - itflr->Height * itflr->Scale / 2.f;
		quad.V0.set (x, y, 0);
		
		x = xCenterQuad + itflr->Width * itflr->Scale / 2.f;
		y = yCenterQuad - itflr->Height * itflr->Scale / 2.f;
		quad.V1.set (x, y, 0);
		
		x = xCenterQuad + itflr->Width * itflr->Scale / 2.f;
		y = yCenterQuad + itflr->Height * itflr->Scale / 2.f;
		quad.V2.set (x, y, 0);
		
		x = xCenterQuad - itflr->Width * itflr->Scale / 2.f;
		y = yCenterQuad + itflr->Height * itflr->Scale / 2.f;
		quad.V3.set (x, y, 0);

		quad.Uv0.U = 0.0f; quad.Uv0.V = 1.0f;
		quad.Uv1.U = 1.0f; quad.Uv1.V = 1.0f;
		quad.Uv2.U = 1.0f; quad.Uv2.V = 0.0f;
		quad.Uv3.U = 0.0f; quad.Uv3.V = 0.0f;

		_Driver->drawQuad(quad, itflr->Material);
	}
}

} /* namespace SBCLIENT */

/* end of file */
