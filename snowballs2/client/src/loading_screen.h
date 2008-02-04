/**
 * \file loading_screen.h
 * \brief CLoadingScreen
 * \date 2008-01-09 15:30GMT
 * \author Jan Boon (Kaetemi)
 * CLoadingScreen
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
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

#ifndef SBCLIENT_LOADING_SCREEN_H
#define SBCLIENT_LOADING_SCREEN_H
#include <nel/misc/types_nl.h>
#include <nel/misc/progress_callback.h>

#include <nel/misc/geom_ext.h>
#include <nel/3d/u_text_context.h>
#include <nel/3d/u_material.h>

#include <map>

namespace NL3D {
	class UDriver;
	class UTextureFile;
}

namespace SBCLIENT {

/**
 * \brief CLoadingScreen
 * \date 2008-01-09 15:30GMT
 * \author Jan Boon (Kaetemi)
 * CLoadingScreen
 */
class CLoadingScreen : public NLMISC::IProgressCallback
{
	struct _CLoadingBackground
	{
		std::string EmptyFile;
		float EmptyWidth;
		float EmptyHeight;
		std::string FullFile;
		float FullWidth;
		float FullHeight;
		float SideLeft;
		float SideRight;
	};
	struct _CLoadingMessage
	{
		float X;
		float Y;
		NLMISC::CRGBA Color;
		uint32 FontSize;
		NL3D::UTextContext::THotSpot HotSpot;
		ucstring Message;
	};
protected:
	// pointers
	NL3D::UDriver *_Driver; // not deleted here
	NL3D::UTextContext *_TextContext; // not deleted here
	NL3D::UTextureFile *_EmptyTexture; // deleted here
	NL3D::UMaterial _EmptyMaterial; // deleted here
	NL3D::UTextureFile *_FullTexture; // deleted here
	NL3D::UMaterial _FullMaterial; // deleted here
	
	// instances
	std::map<uint, _CLoadingBackground> _Backgrounds;
	std::map<uint, _CLoadingMessage> _Messages;
	NLMISC::CQuadUV _EmptyQuad;
	NLMISC::CQuadUV _FullQuad;
	float _SideLeft;
	float _SideRight;
	float _RangeBegin;
	float _RangeEnd;
	float _EmptyWidth;
	float _EmptyHeight;
	float _FullWidth;
	float _FullHeight;
	uint _Background;
public:
	CLoadingScreen();
	virtual ~CLoadingScreen();

	void progress(float progressValue);

	void setRange(float beginValue, float endValue) { _RangeBegin = beginValue; _RangeEnd = endValue; }

	void addMessage(uint id, float x, float y, NLMISC::CRGBA color, uint32 fontSize, NL3D::UTextContext::THotSpot hotSpot);
	void setMessage(uint id, const ucstring &message);
	void removeMessage(uint id);

	void addBackground(uint id, const std::string &empty, int sXe, int sYe, int bXe, int bYe, const std::string &full, int sXf, int sYf, int bXf, int bYf, float sL, float sR);
	void setBackground(uint id);
	void removeBackground(uint id);
	
	void setDriver(NL3D::UDriver *driver);
	void setTextContext(NL3D::UTextContext *textContext) { _TextContext = textContext; }
}; /* class CLoadingScreen */

} /* namespace SBCLIENT */

// setDirection(CVector)
// setSpeed(float)
// addForce(CVector) // average of forces (sum(forces).norm)
// removeForce
// average of average of forces and direction multiplied with speed
// ((sum(forces.norm) + direction).norm * speed

#endif /* #ifndef SBCLIENT_LOADING_SCREEN_H */

/* end of file */
