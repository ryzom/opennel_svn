/** \file snowballs_client.h
 * CSnowballsClient
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
 * 
 * $Id$
 */

#ifndef SBCLIENT_SNOWBALLS_CLIENT_H
#define SBCLIENT_SNOWBALLS_CLIENT_H
#include <nel/misc/types_nl.h>
#include "snowballs_config.h"

#include "loading_screen.h"
#include "function_caller.h"

namespace NLMISC {
	class CFileDisplayer;
	class CConfigFile;
}

namespace NL3D {
	class UDriver;
}

namespace SBCLIENT {
	class CConfigManager;
	class CConfigProxy;
	class CComponentManager;
	class CI18NHelper;
	class CDriverComponent; // temp
	class CLoadingComponent; // temp
	class CHelloWorldComponent; // temp
	class CGraphics;

/**
 * Snowballs client 0.3.
 * \date 2007-2008
 */
class CSnowballsClient
{
private:
	// constants
	static const uint8 Invalid = 0, Load = 1, Reset = 2, Exit = 3;
	static const uint8 Login = 10, Game = 11;

	// server protocol handler versions
	static const uint8 Offline = 0, Snowballs2 = 2, Snowballs5 = 5;

	// pointers
#if SBCLIENT_USE_LOG
	NLMISC::CFileDisplayer *_FileDisplayer; // deleted here
#endif
	// the configuration
	CConfigProxy *_Config; // deleted here
	CConfigManager *_ConfigManager; // deleted here
	// internationalization
	CI18NHelper *_I18NHelper; // deleted here

	// special function ids
	uint _UpdateUtilitiesId;
	// components and their function ids (all deleted here)
	CGraphics *_Graphics;
	uint _UpdateGraphicsDriverId;
	
	// instances
	// the function callers
	CFunctionCaller _UpdateFunctions;
	CFunctionCaller _RenderFunctions;
	// the loading screen that always works
	CLoadingScreen _LoadingScreen;
	// to know which data has been loaded
	bool _LoadedUtils, _LoadedBase, _LoadedLogin, _LoadedIngame, _LoadedConnection;
	bool _EnabledUtils, _EnabledBase, _EnabledLogin, _EnabledIngame, _EnabledConnection;
	// true if the online component needs to be initialized
	uint8 _ServerVersion;
	// set _NextState to switch the current game state
	uint8 _CurrentState, _NextState;
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

	static void updateUtilities(void *context, void *tag);
};

}

#endif /* #ifndef SBCLIENT_SNOWBALLS_CLIENT_H */
