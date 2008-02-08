/**
 * \file loading_screen.cpp
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

#include <nel/misc/types_nl.h>
#include "loading_screen.h"

// #include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/driver_user.h>
#include <nel/3d/driver.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CLoadingScreen::CLoadingScreen() : _Driver(NULL), _TextContext(NULL),
_Background(0), _EmptyTexture(NULL), _FullTexture(NULL),
_RangeBegin(0.0f), _RangeEnd(1.0f)
{
	_EmptyQuad.V0.set(0.0f, 0.0f, 0.0f);
	_EmptyQuad.V1.set(1.0f, 0.0f, 0.0f);
	_EmptyQuad.V2.set(1.0f, 1.0f, 0.0f);
	_EmptyQuad.V3.set(0.0f, 1.0f, 0.0f);
	_EmptyQuad.Uv3.set(0.0f, 0.0f);
	_FullQuad.V0.set(0.0f, 0.0f, 0.0f);
	_FullQuad.V1.set(1.0f, 0.0f, 0.0f);
	_FullQuad.V2.set(1.0f, 1.0f, 0.0f);
	_FullQuad.V3.set(0.0f, 1.0f, 0.0f);
	_FullQuad.Uv3.set(0.0f, 0.0f);
}

CLoadingScreen::~CLoadingScreen()
{
	nlassert(!_Driver);
	nlassert(!_EmptyTexture);
	nlassert(!_FullTexture);
}

void CLoadingScreen::progress(float progressValue)
{
	if (_Driver && !_Driver->isLost())
	{
		_Driver->clearBuffers(CRGBA(0, 0, 0, 255));
		float begin = _RangeBegin + _SideLeft;
		float end = _RangeEnd - _SideRight;
		float range = end - begin;
		float final = begin + (progressValue * range);
		if (final > 1.0f) final = 1.0f;
		_Driver->setMatrixMode2D11();
		if (_EmptyTexture)
		{
			_Driver->drawQuad(_EmptyQuad, _EmptyMaterial);
		}
		if (_FullTexture)
		{
			_FullQuad.V1.x = final;
			_FullQuad.V2.x = final;
			_FullQuad.Uv1.U = _EmptyWidth * final;
			_FullQuad.Uv2.U = _EmptyWidth * final;		
			_Driver->drawQuad(_FullQuad, _FullMaterial);	
		}
		if (_TextContext)
		{
			_TextContext->setKeep800x600Ratio(true);
			for (std::map<uint, _CLoadingMessage>::iterator it = _Messages.begin(); it != _Messages.end(); it++)
			{
				_CLoadingMessage &msg = it->second;
				_TextContext->setColor(msg.Color);
				_TextContext->setFontSize(msg.FontSize);
				_TextContext->setHotSpot(msg.HotSpot);
				_TextContext->printAt(msg.X, msg.Y, msg.Message); // improve this
			}
		}
		_Driver->swapBuffers();
		_EventServer.pump();
	}
}

void CLoadingScreen::addMessage(uint id, float x, float y, NLMISC::CRGBA color, uint32 fontSize, NL3D::UTextContext::THotSpot hotSpot)
{
	nlassert(_Messages.find(id) == _Messages.end());
	_CLoadingMessage msg;
	msg.Color = color;
	msg.FontSize = fontSize;
	msg.HotSpot = hotSpot;
	msg.Message = "";
	msg.X = x;
	msg.Y = y;
	_Messages[id] = msg;
}

void CLoadingScreen::setMessage(uint id, const ucstring &message)
{
	nlassert(_Messages.find(id) != _Messages.end());
	_Messages[id].Message = message;
}

void CLoadingScreen::removeMessage(uint id)
{
	nlassert(id);
	nlassert(_Messages.find(id) != _Messages.end());
	_Messages.erase(id);
}

void CLoadingScreen::addBackground(uint id, const std::string &empty, int sXe, int sYe, int bXe, int bYe, const std::string &full, int sXf, int sYf, int bXf, int bYf, float sL, float sR)
{
	nlassert(_Backgrounds.find(id) == _Backgrounds.end());
	_CLoadingBackground bg;
	bg.EmptyFile = empty;
	bg.EmptyWidth = (float)bXe / (float)sXe;
	bg.EmptyHeight = (float)bYe / (float)sYe;
	bg.FullFile = full;
	bg.FullWidth = (float)bXf / (float)sXf;
	bg.FullHeight = (float)bYf / (float)sYf;
	bg.SideLeft = sL;
	bg.SideRight = sR;
	_Backgrounds[id] = bg;
}

void CLoadingScreen::setBackground(uint id)
{
	if (_Background == id) return;
	if (id) { nlassert(_Backgrounds.find(id) != _Backgrounds.end()); }
	if (_Driver)
	{
		if (_EmptyTexture) 
		{	
			_Driver->deleteTextureFile(_EmptyTexture);
			_EmptyTexture = NULL;
		}
		if (_FullTexture)
		{
			_Driver->deleteTextureFile(_FullTexture);
			_FullTexture = NULL;
		}
		if (id)
		{
			_CLoadingBackground &bg = _Backgrounds[id];
			_EmptyTexture = _Driver->createTextureFile(bg.EmptyFile);
			_EmptyMaterial.setTexture(_EmptyTexture);
			_FullTexture = _Driver->createTextureFile(bg.FullFile);
			_FullMaterial.setTexture(_FullTexture);
		}
	}
	else
	{
		nlassert(!_EmptyTexture);
		nlassert(!_FullTexture);
	}
	if (id)
	{
		_CLoadingBackground &bg = _Backgrounds[id];
		_EmptyWidth = bg.EmptyWidth;
		_EmptyHeight = bg.EmptyHeight;
		_FullWidth = bg.FullWidth;
		_FullHeight = bg.FullHeight;
		_SideLeft = bg.SideLeft;
		_SideRight = bg.SideRight;
		_EmptyQuad.Uv0.set(0.0f, _EmptyHeight);
		_EmptyQuad.Uv1.set(_EmptyWidth, _EmptyHeight);
		_EmptyQuad.Uv2.set(_EmptyWidth, 0.0f);
		_FullQuad.Uv0.set(0.0f, _FullHeight);
		_FullQuad.Uv1.set(_FullWidth, _FullHeight);
		_FullQuad.Uv2.set(_FullWidth, 0.0f);		
	}
	_Background = id;
}

void CLoadingScreen::removeBackground(uint id)
{
	nlassert(id);
	if (id == _Background) setBackground(0);
	nlassert(_Backgrounds.find(id) != _Backgrounds.end());
	_Backgrounds.erase(id);
}

void CLoadingScreen::setDriver(NL3D::UDriver *driver)
{
	if (_Driver)
	{
		_EventServer.removeEmitter(((CDriverUser *)_Driver)->getDriver()->getEventEmitter());
		_Driver->deleteMaterial(_EmptyMaterial);
		_Driver->deleteMaterial(_FullMaterial);
		if (_EmptyTexture) 
		{	
			_Driver->deleteTextureFile(_EmptyTexture);
			_EmptyTexture = NULL;
		}
		if (_FullTexture)
		{
			_Driver->deleteTextureFile(_FullTexture);
			_FullTexture = NULL;
		}
	}
	nlassert(!_FullTexture); nlassert(!_FullTexture);
	_Driver = driver;
	if (_Driver)
	{
		_EventServer.addEmitter(((CDriverUser *)_Driver)->getDriver()->getEventEmitter());
		_EmptyMaterial = _Driver->createMaterial();
		_FullMaterial = _Driver->createMaterial();
		if (_Background)
		{
			_EmptyTexture = _Driver->createTextureFile(_Backgrounds[_Background].EmptyFile);
			_EmptyMaterial.setTexture(_EmptyTexture);
			_FullTexture = _Driver->createTextureFile(_Backgrounds[_Background].FullFile);
			_FullMaterial.setTexture(_FullTexture);
		}
	}
}

} /* namespace SBCLIENT */

/* end of file */
