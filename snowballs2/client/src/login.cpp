/**
 * \file login.cpp
 * \brief MLogin
 * \date 2008-02-04 01:10GMT
 * \author Mark Troutt (Mankar)
 * \author Jan Boon (Kaetemi)
 * MLogin, has some code taken from SNB-24
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2007-2008  Mark Troutt (Mankar)
 *                          Jan Boon (Kaetemi)
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

#include <nel/misc/types_nl.h>
#include "login.h"

// #include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_text_context.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/viewport.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

MLogin::MLogin(const std::string &id, NL3D::UDriver *driver, NL3D::UTextContext *textContext, CI18NHelper *i18n, CLoginData *loginData)
: _Driver(driver), _TextContext(textContext), _I18N(i18n), 
_LoginData(loginData), _Config(id), _LogoAngle(0.0f), 
_TypingPassword(false), _Enabled(false), _Selection(0)
{
	// todo: get stuff from config
	_BackgroundTexture = _Driver->createTextureFile("snowballs_login.tga");
	_BackgroundMaterial = _Driver->createMaterial();
	_BackgroundMaterial.setTexture(_BackgroundTexture);
	_BackgroundQuad.V0.set(0.0f, 0.0f, 0.0f);
	_BackgroundQuad.V1.set(1.0f, 0.0f, 0.0f);
	_BackgroundQuad.V2.set(1.0f, 1.0f, 0.0f);
	_BackgroundQuad.V3.set(0.0f, 1.0f, 0.0f);
	float height = 768.0f / 1024.0f;
	float width = 1024.0f / 1024.0f;
	_BackgroundQuad.Uv0.set(0.0f, height);
	_BackgroundQuad.Uv1.set(width, height);
	_BackgroundQuad.Uv2.set(width, 0.0f);
	_BackgroundQuad.Uv3.set(0.0f, 0.0f);

	_OverlayMaterial = _Driver->createMaterial();
	_OverlayMaterial.setColor(CRGBA(0, 0, 0, 192));
	_OverlayMaterial.setBlend(true);
	_OverlayQuad.V0.set(0.0f, 0.0f, 0.0f);
	_OverlayQuad.V1.set(1.0f, 0.0f, 0.0f);
	_OverlayQuad.V2.set(1.0f, 1.0f, 0.0f);
	_OverlayQuad.V3.set(0.0f, 1.0f, 0.0f);

	_SelectTexture = _Driver->createTextureFile("snowballs_login_select.tga");
	_SelectMaterial = _Driver->createMaterial();
	_SelectMaterial.setTexture(_SelectTexture);
	_SelectMaterial.setBlend(true);
	_SelectQuad.Uv0.set(0.0f, 1.0f);
	_SelectQuad.Uv1.set(1.0f, 1.0f);
	_SelectQuad.Uv2.set(1.0f, 0.0f);
	_SelectQuad.Uv3.set(0.0f, 0.0f);

	CViewport viewport;

	//_SnowScene = _Driver->createScene(false);
	//_SnowScene->setAmbientGlobal(CRGBA(255, 255, 255, 255));
	//_SnowScene->enableLightingSystem(true);

	//viewport.init(0.0f, 0.0f, 1.0f, 1.0f);
	//_SnowScene->setViewport(viewport);

	//_Snow = _SnowScene->createInstance("snow.ps");
	//_Snow.setPos(0.0f, 0.0f, 0.0f);

	_LogoScene = _Driver->createScene(false);
	_LogoScene->setAmbientGlobal(CRGBA(255, 255, 255, 255));
	_LogoScene->enableLightingSystem(true);

	viewport.init(0.0f, 0.80f, 0.2f, 0.2f);
	_LogoScene->setViewport(viewport);

	_Logo = _LogoScene->createInstance("nel_logo.shape");
	_Logo.setPos(0.0f, 3.0f, 0.0f);
	_Logo.setTransformMode(UTransformable::RotEuler);
}

MLogin::~MLogin()
{
	_LogoScene->deleteInstance(_Logo);
	_Driver->deleteScene(_LogoScene);
	//_SnowScene->deleteInstance(_Snow);
	//_Driver->deleteScene(_SnowScene);
	_Driver->deleteMaterial(_OverlayMaterial);
	_Driver->deleteTextureFile(_SelectTexture);
	_Driver->deleteMaterial(_SelectMaterial);
	_Driver->deleteTextureFile(_BackgroundTexture);
	_Driver->deleteMaterial(_BackgroundMaterial);
}

void MLogin::setSelectQuad(float x, float y)
{
	float half_width = 256.0f / 1024.0f / 2.0f;
	float half_height = 256.0f / 768.0f / 2.0f;
	_SelectQuad.V0.set(x - half_width, y - half_height, 0.0f);
	_SelectQuad.V1.set(x + half_width, y - half_height, 0.0f);
	_SelectQuad.V2.set(x + half_width, y + half_height, 0.0f);
	_SelectQuad.V3.set(x - half_width, y + half_height, 0.0f);
}

void MLogin::updateSelection(float x, float y)
{
	if (x > 0.850f && x < 0.985f && y > 0.892f && y < 0.985f)
	{
		_Selection = ExitButton;
		setSelectQuad((0.985f + 0.850f) / 2.0f, (0.985f + 0.892f) / 2.0f);
	}
	else if (x > 0.206f && x < 0.454f && y > 0.083f && y < 0.166f)
	{
		_Selection = OfflineButton;
		setSelectQuad((0.206f + 0.454f) / 2.0f, (0.166f + 0.083f) / 2.0f);
	}
	else if (x > 0.585f && x < 0.808f && y > 0.082f && y < 0.159f)
	{
		_Selection = ConnectButton;
		setSelectQuad((0.585f + 0.808f) / 2.0f, (0.082f + 0.159f) / 2.0f);
	}
	else if (x > 0.492f && x < 0.837f && y > 0.372f && y < 0.498f)
	{
		_Selection = UsernameText;
		setSelectQuad((0.492f + 0.837f) / 2.0f, (0.372f + 0.498f) / 2.0f);
	}
	else if (x > 0.489f && x < 0.844f && y > 0.221f && y < 0.339f)
	{
		_Selection = PasswordText;
		setSelectQuad((0.489f + 0.844f) / 2.0f, (0.221f + 0.339f) / 2.0f);
	}
	else _Selection = 0;
}

void MLogin::operator () (const CEvent &event)
{
	if (!_Message.empty()) return;

	if (event == EventCharId)
	{
		CEventChar &ec = (CEventChar &)event;

		switch (ec.Char)
		{
		case KeyRETURN:
			// todo: try to connect!
			break;
		case KeyBACK:
			if(_TypingPassword)
			{
				if (_LoginData->Password.size() != 0 )
				{
					_LoginData->Password.erase(_LoginData->Password.end() - 1);
				}
			}
			else
			{
				if (_LoginData->Username.size() != 0 )
				{
					_LoginData->Username.erase(_LoginData->Username.end() - 1);
				}
			}
			break;	
		case KeyESCAPE:
			// todo: exit!
			break;
		case KeyTAB:
			_TypingPassword = !_TypingPassword;
			break;
		default:			
			if(_TypingPassword)
			{
				_LoginData->Password += (char)ec.Char;
			}
			else
			{
				_LoginData->Username += ec.Char;
			}
		}
	}
	else
	{
		CEventMouse &em = (CEventMouse &)event;
		if (event == EventMouseMoveId)
		{
			updateSelection(em.X, em.Y);
		}
		else if (event == EventMouseUpId)
		{
			if (em.Button && leftButton)
			{
				updateSelection(em.X, em.Y);
				switch (_Selection)
				{
				case UsernameText:
					_TypingPassword = false;
					break;
				case PasswordText:
					_TypingPassword = true;
					break;
				case OfflineButton:
					_LoginData->Version = Offline;
					_LoginData->Password = "";
					_LoginData->FrontEnd = "";
					// switch game state
					break;
				case ConnectButton:
					// try connecting
					break;
				case ExitButton:
					// switch exit state
					break;
				}
			}
		}
	}
}

void MLogin::enable()
{
	nlassert(!_Enabled); _Enabled = true;
	_Driver->EventServer.addListener(EventCharId, this);
	_Driver->EventServer.addListener(EventMouseMoveId, this);
	_Driver->EventServer.addListener(EventMouseUpId, this);
}

void MLogin::disable()
{
	nlassert(_Enabled); _Enabled = false;
	_Driver->EventServer.removeListener(EventCharId, this);
	_Driver->EventServer.removeListener(EventMouseMoveId, this);
	_Driver->EventServer.removeListener(EventMouseUpId, this);
}

void MLogin::updateInterface(void *context, void *tag)
{	
	MLogin *me = (MLogin *)context;
	float temptime = 1.0f / 60.0f;

	// some silly code to add the | and do the stars
	me->_UsernameText = me->_LoginData->Username;
	me->_PasswordStarsText = "";
	for (uint i = 0; i < me->_LoginData->Password.size(); ++i)
		me->_PasswordStarsText += "*";
	if (me->_TypingPassword) me->_PasswordStarsText += "|";
	else me->_UsernameText += "|";
	
	//me->_SnowScene->animate(temptime);

	//updateLoginInterface
	if (!me->_Message.empty())
	{
		me->_LogoAngle += 2.0f * temptime;
		me->_Logo.setRotEuler(0.0f, 0.0f, me->_LogoAngle);
		me->_LogoScene->animate(temptime);
	}
	
	// exit if escape?
}

void MLogin::renderInterface(void *context, void *tag)
{	
	MLogin *me = (MLogin *)context;
	me->_Driver->setMatrixMode2D11();
	me->_Driver->drawQuad(me->_BackgroundQuad, me->_BackgroundMaterial);
	me->_TextContext->setColor(CRGBA(255, 255, 255, 255));
	me->_TextContext->setHotSpot(UTextContext::MiddleMiddle);
	me->_TextContext->setFontSize(48);
	me->_TextContext->printAt(0.675f, 0.4375f, me->_UsernameText);
	me->_TextContext->printAt(0.675f, 0.2875f, me->_PasswordStarsText);
	me->_Driver->clearZBuffer();
	//me->_SnowScene->render();
	if (!me->_Message.empty())
	{
		me->_Driver->drawQuad(me->_OverlayQuad, me->_OverlayMaterial);
		me->_Driver->clearZBuffer();
		me->_LogoScene->render();
		me->_TextContext->setColor(CRGBA(255, 255, 255, 255));
		me->_TextContext->setHotSpot(UTextContext::MiddleMiddle);
		me->_TextContext->setFontSize(32);
		me->_TextContext->printAt(0.5f, 0.5f, me->_Message);
	}
	else if (me->_Selection)
	{
		me->_Driver->drawQuad(me->_SelectQuad, me->_SelectMaterial);
	}
}

} /* namespace SBCLIENT */

/* end of file */
