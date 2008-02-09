/**
 * \file offline.cpp
 * \brief COffline
 * \date 2008-02-06 14:43GMT
 * \author Jan Boon (Kaetemi)
 * COffline
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

#include <nel/misc/types_nl.h>
#include "offline.h"

// Project includes
#include "loading_screen.h"
#include "loading.h"
#include "entities_old.h"
#include "landscape.h"
#include "time.h"

// NeL includes
// #include <nel/misc/debug.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

COffline::COffline(CLoadingScreen *loadingScreen, const string &id, 
CLogin::CLoginData *loginData, CLoading *loading, CLandscape *landscape,
CEntitiesOld *entities, CTime *time) : _Config(id), 
_LoadingScreen(loadingScreen), _LoginData(loginData), _Loading(loading), 
_Landscape(landscape), _Entities(entities), _Time(time)
{
	_Loading->setBackground("OldLands");
	
	_Loading->setMessageState("RefreshLandscape");
	_LoadingScreen->setRange(0.00f, 0.75f);
	_LoadingScreen->progress(0.00f);
	CVector start(
		_Config.getVar("StartPoint").asFloat(0), 
		_Config.getVar("StartPoint").asFloat(1), 
		_Config.getVar("StartPoint").asFloat(2));
	_Landscape->RefreshZonesAround = &start;
	_Landscape->refresh(*_LoadingScreen);
		
	_Loading->setMessageState("CreateSelf");
	_LoadingScreen->setRange(0.75f, 1.00f);
	//_Time->updateTime(NULL); // time update needed before doing the last small changes
	_Entities->addEntity(++(_Entities->LastEID),
		_LoginData->Username, CEntityOld::Self, start, start);
	_Landscape->RefreshZonesAround = &_Entities->Self->Position;
	_LoadingScreen->progress(1.00f);
	
	for (uint i = 0; i < 8; ++i)
	{
		uint32 eid = ++(_Entities->LastEID);
		_Entities->addEntity(eid, "Entity" + toString(eid), 
			CEntityOld::Other, start, start);
		_Entities->getEntity(eid).AutoMove = true;
	}
		
	_LoadingScreen->progress(1.00f);
}

COffline::~COffline()
{
	_Entities->removeAll();
}

} /* namespace SBCLIENT */

/* end of file */
