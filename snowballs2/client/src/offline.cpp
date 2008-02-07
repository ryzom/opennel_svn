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

// NeL includes
// #include <nel/misc/debug.h>

// STL includes

using namespace std;
// using namespace NLMISC;

namespace SBCLIENT {

COffline::COffline()
{
	//		_HasOffline = true;

//		// another bunch of temp stuff
//#if SBCLIENT_WITH_SOUND
//		playMusic(SBCLIENT_MUSIC_WAIT);
//#endif
//
//		uint32 id = ++NextEID;
//		ucstring Login = ucstring("Entity" + toString(id));
//
//		// Creates the self entity
//		displayLoadingState("Creating offline entity");
//		addEntity(id, Login.toUtf8(), 
//			CEntity::Self, CVector(
//					ConfigFile->getVar("StartPoint").asFloat(0),
//					ConfigFile->getVar("StartPoint").asFloat(1),
//					ConfigFile->getVar("StartPoint").asFloat(2)),
//				CVector(
//					ConfigFile->getVar("StartPoint").asFloat(0),
//					ConfigFile->getVar("StartPoint").asFloat(1),
//					ConfigFile->getVar("StartPoint").asFloat(2)));
//
//		displayLoadingState("Load Landscape");
//		LandscapeComponent->loadAllZonesAround();
//
//		// Display a local welcome message
//		addLine(">>>>> Welcome to Snowballs!");
//		addLine(">>>>> Press SHIFT-ESC to exit the game.");
//
//		displayLoadingState("Ready!");
//	
//#if SBCLIENT_WITH_SOUND
//		playMusic(SBCLIENT_MUSIC_BACKGROUND);
//#endif
}

COffline::~COffline()
{
	//	if (_HasOffline)
//	{
//		//// temp
//		//deleteAllEntities();
//		
//		_HasOffline = false;
//	}
}

} /* namespace SBCLIENT */

/* end of file */
