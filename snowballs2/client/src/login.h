/**
 * \file login.h
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

#ifndef SBCLIENT_LOGIN_H
#define SBCLIENT_LOGIN_H
#include <nel/misc/types_nl.h>
#include <nel/misc/ucstring.h>

#include "config_proxy.h"
#include "member_callback_decl.h"
#include "ls_client.h"

#include <nel/misc/quad.h>
#include <nel/misc/geom_ext.h>
#include <nel/misc/event_listener.h>
#include <nel/3d/u_material.h>
#include <nel/3d/u_instance.h>

namespace NL3D {
	class UDriver;
	class UTextContext;
	class UTextureFile;
	class UScene;
}

namespace SBCLIENT {
	class CI18NHelper;
	class CTime;

/**
 * \brief CLogin
 * \date 2008-02-04 01:10GMT
 * \author Mark Troutt (Mankar)
 * \author Jan Boon (Kaetemi)
 * CLogin, has some code taken from SNB-24
 */
class CLogin : public NLMISC::IEventListener
{
public:
	// server protocol handler versions
	static const uint8 Offline = 0, Snowballs3 = 3, Snowballs5 = 5;
	struct CLoginData
	{
		CLoginData() : Version(Offline) { }
		ucstring Username;
		std::string FrontEnd;
		NLNET::CLoginCookie LoginCookie;
		uint8 Version;
		ucstring Message;
	};
protected:
	static const uint8 ExitButton = 1, ConnectButton = 2, OfflineButton = 3;
	static const uint8 UsernameText = 4, PasswordText = 5;
	// pointers
	SBCLIENT::CTime *_Time; // p
	NL3D::UDriver *_Driver; // not deleted here
	NL3D::UTextContext *_TextContext; // not deleted here	
	CI18NHelper *_I18N; // not deleted here
	CLoginData *_LoginData; // not deleted here
	NL3D::UScene *_SnowScene; // deleted here
	NL3D::UInstance _Snow; // deleted here
	NL3D::UScene *_LogoScene; // deleted here
	NL3D::UInstance _Logo; // deleted here
	NL3D::UMaterial _BackgroundMaterial; // deleted here
	NL3D::UTextureFile *_BackgroundTexture; // deleted here
	NL3D::UMaterial _SelectMaterial; // deleted here
	NL3D::UTextureFile *_SelectTexture; // deleted here
	NL3D::UMaterial _OverlayMaterial; // deleted here
	
	// instances
	NLMISC::CQuadUV _BackgroundQuad;
	NLMISC::CQuadUV _SelectQuad;
	NLMISC::CQuad _OverlayQuad;
	CConfigProxy _Config;
	float _LogoAngle;
	ucstring _UsernameText;
	ucstring _PasswordStarsText;
	bool _TypingPassword;
	bool _Enabled;
	uint8 _Selection;
	ucchar _PasswordText[NLMISC::MaxCStringSize];
	uint _PasswordTextPos;
	double _TimeOut;
	CLSClient _LSClient;
public:
	CLogin(const std::string &id, SBCLIENT::CTime *time, NL3D::UDriver *driver, NL3D::UTextContext *textContext, CI18NHelper *i18n, CLoginData *loginData);
	virtual ~CLogin();

	void setSelectQuad(float x, float y);
	void updateSelection(float x, float y);

	void operator () (const NLMISC::CEvent &event);

	void enable();
	void disable();

	SBCLIENT_CALLBACK_DECL(updateNetwork);
	SBCLIENT_CALLBACK_DECL(updateInterface);
	SBCLIENT_CALLBACK_DECL(renderInterface);

protected:
	void connect();
	SBCLIENT_CALLBACK_DECL(cbAuthenticateUser);
	SBCLIENT_CALLBACK_DECL(cbSelectShard);
}; /* class CLogin */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LOGIN_H */

/* end of file */
