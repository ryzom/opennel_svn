/**
 * \file login.h
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

#ifndef SBCLIENT_LOGIN_H
#define SBCLIENT_LOGIN_H
#include <nel/misc/types_nl.h>
#include <nel/misc/ucstring.h>

#include "config_proxy.h"

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
	class _CLoginKeyboard;
	class _CLoginMouse;

/**
 * \brief MLogin
 * \date 2008-02-04 01:10GMT
 * \author Mark Troutt (Mankar)
 * \author Jan Boon (Kaetemi)
 * MLogin, has some code taken from SNB-24
 */
class MLogin : public NLMISC::IEventListener
{
public:
	// server protocol handler versions
	static const uint8 Offline = 0, Snowballs3 = 3, Snowballs5 = 5;
	struct CLoginData
	{
		CLoginData() : Version(Offline) { }
		ucstring Username;
		std::string Password;
		std::string FrontEnd;
		uint8 Version;
	};
protected:
	static const uint8 ExitButton = 1, ConnectButton = 2, OfflineButton = 3;
	static const uint8 UsernameText = 4, PasswordText = 5;
	// pointers
	NL3D::UDriver *_Driver; // not deleted here
	NL3D::UTextContext *_TextContext; // not deleted here	
	CI18NHelper *_I18N; // not deleted here
	CLoginData *_LoginData; // not deleted here
	//NL3D::UScene *_SnowScene; // deleted here
	//NL3D::UInstance _Snow; // deleted here
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
	ucstring _Message;
	float _LogoAngle;
	ucstring _UsernameText;
	ucstring _PasswordStarsText;
	bool _TypingPassword;
	bool _Enabled;
	uint8 _Selection;
public:
	MLogin(const std::string &id, NL3D::UDriver *driver, NL3D::UTextContext *textContext, CI18NHelper *i18n, CLoginData *loginData);
	virtual ~MLogin();

	void setSelectQuad(float x, float y);
	void updateSelection(float x, float y);

	void operator () (const NLMISC::CEvent &event);

	void enable();
	void disable();

	static void updateInterface(void *context, void *tag);
	static void renderInterface(void *context, void *tag);
}; /* class MLogin */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LOGIN_H */

/* end of file */
