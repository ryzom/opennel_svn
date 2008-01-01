/** \file snowballs_client.h
 * CSnowballsClient
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * $Id$
 */

#ifndef SBCLIENT_SNOWBALLS_CLIENT_H
#define SBCLIENT_SNOWBALLS_CLIENT_H
#include <nel/misc/types_nl.h>
#include "snowballs_config.h"

namespace NLMISC {
	class CFileDisplayer;
	class CConfigFile;
}

namespace NL3D {
	class UDriver;
}

namespace SBCLIENT {
	class CComponentManager;
	class CLoadingScreen;
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

	// pointers
#if SBCLIENT_USE_LOG
	NLMISC::CFileDisplayer *_FileDisplayer; // deleted here
#endif
	CComponentManager *_ComponentManager; // deleted here
	NLMISC::CConfigFile *_ConfigFile; // deleted here
	CLoadingScreen *_LoadingScreen; // deleted here

	// CDriverComponent // deleted here

	NL3D::UDriver *_Driver; // not deleted here
	
	// instances
	// to know which data has been loaded
	bool _HasCore, _HasLogin, _HasIngame, _HasOnline, _HasOffline;
	// true if the online component needs to be initialized
	bool _PlayOnline;
	// set _NextState to switch the current game state
	uint8 _CurrentState, _NextState;
public:
	CSnowballsClient();
	~CSnowballsClient();
	int run();
private:
	// temp
	void enableCore();
	void disableCore();
	void enableLogin();
	void disableLogin();
	void enableIngame();
	void disableIngame();
	void enableOnline();
	void disableOnline();
	void enableOffline();
	void disableOffline();
	void disableAll();
};

}

#endif /* #ifndef SBCLIENT_SNOWBALLS_CLIENT_H */
