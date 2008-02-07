/**
 * \file login.cpp
 * \brief CLogin
 * \date 2008-02-04 01:10GMT
 * \author Mark Troutt (Mankar)
 * \author Jan Boon (Kaetemi)
 * CLogin, has some code taken from SNB-24
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

#include "member_callback_impl.h"
#include "ls_client.h"
#include "i18n_helper.h"

#include "time.h"

// #include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_text_context.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/viewport.h>
#include <nel/misc/command.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CLogin::CLogin(const std::string &id, SBCLIENT::CTime *time, NL3D::UDriver *driver, NL3D::UTextContext *textContext, CI18NHelper *i18n, CLoginData *loginData)
: _Time(time), _Driver(driver), _TextContext(textContext), _I18N(i18n), 
_LoginData(loginData), _Config(id), _LogoAngle(0.0f), _TimeOut(5.0), 
_TypingPassword(false), _Enabled(false), _Selection(0)
{
	nlassert(_Time);
	nlassert(_Driver);
	nlassert(_TextContext);
	nlassert(_I18N);
	nlassert(_LoginData);

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

	_LoginData->Username = _Config.getValue("Username", ucstring());
	_PasswordText = _Config.getValue("Password", ucstring());
}

CLogin::~CLogin()
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

void CLogin::setSelectQuad(float x, float y)
{
	float half_width = 256.0f / 1024.0f / 2.0f;
	float half_height = 256.0f / 768.0f / 2.0f;
	_SelectQuad.V0.set(x - half_width, y - half_height, 0.0f);
	_SelectQuad.V1.set(x + half_width, y - half_height, 0.0f);
	_SelectQuad.V2.set(x + half_width, y + half_height, 0.0f);
	_SelectQuad.V3.set(x - half_width, y + half_height, 0.0f);
}

void CLogin::updateSelection(float x, float y)
{
	// these numbers are the locations of the buttons ^^
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

void CLogin::operator () (const CEvent &event)
{
	if (!_LoginData->Message.empty())
	{
		if (_TimeOut >= 0.0f && event != EventMouseMoveId)
		{
			_TimeOut = -1.0f;
			_LoginData->Message = "";
		}
		return;
	}

	if (event == EventCharId)
	{
		CEventChar &ec = (CEventChar &)event;

		switch (ec.Char)
		{
		case KeyRETURN:
			connect();
			break;
		case KeyBACK:
			if(_TypingPassword)
			{
				if (_PasswordText.size() != 0 )
				{
					_PasswordText.erase(_PasswordText.end() - 1);
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
			ICommand::execute(_Config.getValue(
				"ExitCommand", string("set_state Exit")), 
				*INelContext::getInstance().getInfoLog());
			break;
		case KeyTAB:
			_TypingPassword = !_TypingPassword;
			break;
		default:			
			if(_TypingPassword)
			{
				_PasswordText += (char)ec.Char;
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
					_LoginData->LoginCookie = NLNET::CLoginCookie(0, 0);
					_LoginData->FrontEnd = "";
					ICommand::execute(_Config.getValue(
						"OfflineCommand", string("set_state Game")), 
						*INelContext::getInstance().getInfoLog());
					break;
				case ConnectButton:
					connect();
					break;
				case ExitButton:
					ICommand::execute(_Config.getValue(
						"ExitCommand", string("set_state Exit")), 
						*INelContext::getInstance().getInfoLog());
					break;
				}
			}
		}
	}
}

void CLogin::enable()
{
	nlassert(!_Enabled); _Enabled = true;
	_Driver->EventServer.addListener(EventCharId, this);
	_Driver->EventServer.addListener(EventMouseMoveId, this);
	_Driver->EventServer.addListener(EventMouseUpId, this);
}

void CLogin::disable()
{
	nlassert(_Enabled); _Enabled = false;
	_Driver->EventServer.removeListener(EventCharId, this);
	_Driver->EventServer.removeListener(EventMouseMoveId, this);
	_Driver->EventServer.removeListener(EventMouseUpId, this);
}

void CLogin::connect()
{
	_LSClient.authenticateUser(cbAuthenticateUser, this, NULL, 
		_Config.getValue("LSHost", string("localhost")), 
		_LoginData->Username, CLSClient::encryptPassword(_PasswordText), 
		_Config.getValue("ClientApplication", string("snowballs")));

	// set message if no error, errors are handled by the callback
	if (_LSClient.LastError.empty())
	{
		_TimeOut = -1.0f;
		_LoginData->Message = _I18N->get("i18nAuthenticatingUser");
		nlinfo(_LoginData->Message.toUtf8().c_str());
	}
}

SBCLIENT_CALLBACK_IMPL(CLogin, cbAuthenticateUser)
{
	if (!_LSClient.LastError.empty())
	{
		_TimeOut = 5.0f;
		_LoginData->Message = ucstring(_LSClient.LastError);
		return;
	}

	CLSClient::CShard *shard = _LSClient.getShard(
		_Config.getValue("ShardId", 300));
	if (!shard) shard = &_LSClient.ShardList[0];		
	if (shard->Version == "SB5") _LoginData->Version = Snowballs5;
	else _LoginData->Version = Snowballs3;
	_LSClient.selectShard(cbSelectShard, this, NULL, shard->ShardId);

	if (_LSClient.LastError.empty())
	{
		_TimeOut = -1.0f;
		_LoginData->Message = ucstring::makeFromUtf8(toString(
			_I18N->get("i18nSelectingShard").toUtf8().c_str(),
			shard->ShardId,
			shard->Name.toUtf8().c_str(),
			shard->NbPlayers));
		nlinfo(_LoginData->Message.toUtf8().c_str());
	}
}

SBCLIENT_CALLBACK_IMPL(CLogin, cbSelectShard)
{
	if (!_LSClient.LastError.empty())
	{
		_TimeOut = 5.0f;
		_LoginData->Message = ucstring(_LSClient.LastError);
		return;
	}
	
	_LoginData->FrontEnd = _LSClient.FrontEnd;
	_LoginData->LoginCookie = _LSClient.LoginCookie;	
	_LoginData->Message = "";
	
	ICommand::execute(_Config.getValue(
		"ConnectCommand", string("set_state Game")), 
		*INelContext::getInstance().getInfoLog());
}

SBCLIENT_CALLBACK_IMPL(CLogin, updateNetwork)
{
	_LSClient.update();
}

SBCLIENT_CALLBACK_IMPL(CLogin, updateInterface)
{
	double timeDelta = _Time->LocalTimeDelta;

	// some silly code to add the | and do the stars
	_UsernameText =_LoginData->Username;
	_PasswordStarsText = "";
	for (uint i = 0; i < _PasswordText.size(); ++i)
		_PasswordStarsText += "*";
	if (_LoginData->Message.empty()) {
	if (_TypingPassword) _PasswordStarsText += "|";
	else _UsernameText += "|"; }
	
	//_SnowScene->animate(temptime);

	if (!_LoginData->Message.empty())
	{
		if (_TimeOut >= 0.0f) 
		{
			_TimeOut -= timeDelta;
			if (_TimeOut <= 0.0f)
			{
				_TimeOut = -1.0f;
				_LoginData->Message = "";
			}
		}
	}

	if (!_LoginData->Message.empty())
	{
		_LogoAngle += 2.0f * (float)timeDelta;
		_Logo.setRotEuler(0.0f, 0.0f, _LogoAngle);
		_LogoScene->animate(timeDelta);
	}
	
}

SBCLIENT_CALLBACK_IMPL(CLogin, renderInterface)
{
	_Driver->setMatrixMode2D11();
	_Driver->drawQuad(_BackgroundQuad, _BackgroundMaterial);
	_TextContext->setColor(CRGBA(255, 255, 255, 255));
	_TextContext->setHotSpot(UTextContext::MiddleMiddle);
	_TextContext->setFontSize(48);
	_TextContext->printAt(0.675f, 0.4375f, _UsernameText);
	_TextContext->printAt(0.675f, 0.2875f, _PasswordStarsText);
	_Driver->clearZBuffer();
	//_SnowScene->render();
	if (!_LoginData->Message.empty())
	{
		_Driver->drawQuad(_OverlayQuad, _OverlayMaterial);
		_Driver->clearZBuffer();
		_LogoScene->render();
		_TextContext->setColor(CRGBA(255, 255, 255, 255));
		_TextContext->setHotSpot(UTextContext::MiddleMiddle);
		if (_LoginData->Message.size() > 96) _TextContext->setFontSize(16);
		else if (_LoginData->Message.size() > 64) _TextContext->setFontSize(24);
		else _TextContext->setFontSize(32);
		_TextContext->printAt(0.5f, 0.5f, _LoginData->Message);
	}
	else if (_Selection)
	{
		_Driver->drawQuad(_SelectQuad, _SelectMaterial);
	}
}

} /* namespace SBCLIENT */

/* end of file */
