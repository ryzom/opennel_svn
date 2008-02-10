/**
 * \file snowballs_client.h
 * \brief CSnowballsClient
 * \date 2008-02-07 17:21GMT
 * \author Jan Boon (Kaetemi)
 * CSnowballsClient
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

#ifndef SBCLIENT_SNOWBALLS_CLIENT_H
#define SBCLIENT_SNOWBALLS_CLIENT_H
#include <nel/misc/types_nl.h>
#include "snowballs_config.h"

#include "loading_screen.h"
#include "function_caller.h"
#include "string_identifier.h"
#include "member_callback_decl.h"

#include "login.h"
#include "keyboard.h"

namespace NLMISC {
	class CFileDisplayer;
	class CConfigFile;
}

namespace NL3D {
	class UDriver;
}

namespace SBCLIENT {
	struct CCommandWrapper;
	class CConfigManager;
	class CConfigProxy;
	class CComponentManager;
	class CI18NHelper;
	class CLoading;
	class CGraphics;
	class CSound;
	class CLandscape;
	class CLandscapeIGOld;
	class CCollisionsOld;
	class CAnimationOld;
	class CEntitiesOld;
	class COffline;

/**
 * Snowballs client 0.4.0
 * \date 2007-2008
 */
class CSnowballsClient
{
private:
	// identifiers
	uint Invalid, Load, Reset, Exit;
	uint Login, Game;

	// pointers
	// the configuration
	CConfigProxy *_Config; // deleted here
	CConfigManager *_ConfigManager; // deleted here
	// internationalization
	CI18NHelper *_I18NHelper; // deleted here

	// special function ids
	uint _UpdateUtilitiesId;
	uint _UpdateDebugId;
	uint _RenderDebugId;
	// components and their function ids (all deleted here)
	SBCLIENT::CLoading *_Loading; // loading screen
	SBCLIENT::CGraphics *_Graphics; // graphics driver
	uint _GraphicsUpdateDriverId;
	SBCLIENT::CSound *_Sound; // sound driver
	SBCLIENT::CTime *_Time; // time component
	uint _TimeUpdateTimeId;
	uint _SoundUpdateSoundId;
	//SBCLIENT::CInput _Input; // input listener
	//uint _InputUpdateInputId;
	SBCLIENT::CLogin *_Login; // login interface
	uint _LoginUpdateInterfaceId;
	uint _LoginRenderInterfaceId;
	uint _LoginUpdateNetworkId;
	//SBCLIENT::CKeyboard _Keyboard; // ingame keyboard
	uint _KeyboardUpdateInputId;
	SBCLIENT::CLandscape *_Landscape;
	uint _LandscapeUpdateAnimationsId;
	uint _LandscapeUpdateLandscapeId;
	uint _LandscapeRenderSceneId;
	uint _LandscapeInitBloomId;
	uint _LandscapeEndBloomId;
	SBCLIENT::CLandscapeIGOld *_LandscapeIG;
	SBCLIENT::CCollisionsOld *_Collisions;
	SBCLIENT::CAnimationOld *_Animation;
	uint _AnimationUpdateAnimationsId;
	SBCLIENT::CEntitiesOld *_Entities;
	uint _EntitiesUpdateEntitiesId;
	SBCLIENT::COffline *_Offline;
	void *_Snowballs3; // temp
	void *_Snowballs5;

	// commands, must be deleted
	CCommandWrapper *_SetStateCommand;
	CCommandWrapper *_SwitchDebugCommand;
	
	// instances
	// the function callers
	CFunctionCaller _UpdateFunctions;
	CFunctionCaller _RenderFunctions;
	// the loading screen that always works
	CLoadingScreen _LoadingScreen;
	// string identifier
	CStringIdentifier _StringIdentifier;
	// to know which data has been loaded
	bool _LoadedUtils, _LoadedBase, _LoadedLogin, _LoadedIngame, _LoadedConnection;
	bool _EnabledUtils, _EnabledBase, _EnabledLogin, _EnabledIngame, _EnabledConnection;
	// contains information about what frontend to connect to
	CLogin::CLoginData _LoginData;
	// set _NextState to switch the current game state
	uint _CurrentState, _NextState;
public:
	CSnowballsClient();
	~CSnowballsClient();
	int run();
private:
	void loadUtils();
	void unloadUtils();
	void loadBase();
	void unloadBase();
	void loadLogin();
	void unloadLogin();
	void loadIngame();
	void unloadIngame();
	void loadConnection();
	void unloadConnection();
	void unloadAll();

	void enableUtils();
	void disableUtils();
	void enableBase();
	void disableBase();
	void enableLogin();
	void disableLogin();
	void enableIngame();
	void disableIngame();
	void enableConnection();
	void disableConnection();
	void disableAll();

	// commands
	SBCLIENT_CALLBACK_COMMAND_DECL(commandSetState);
	SBCLIENT_CALLBACK_COMMAND_DECL(commandSwitchDebug);

	// config
	SBCLIENT_CALLBACK_CONFIG_DECL(configDebugEnabled);

	// update and render functions
	SBCLIENT_CALLBACK_DECL(updateUtilities);
	SBCLIENT_CALLBACK_DECL(updateDebug);
	SBCLIENT_CALLBACK_DECL(renderDebug);

	// various debug utilities, place in updateDebug and renderDebug
	SBCLIENT_CALLBACK_DECL(renderVersion);
}; /* class CSnowballsClient */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_SNOWBALLS_CLIENT_H */

/* end of file */
