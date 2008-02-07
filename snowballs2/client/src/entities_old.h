/**
 * \file entities_old.h
 * \brief CEntitiesOld
 * \date 2008-02-07 10:55GMT
 * \author Nevrax Ltd.
 * CEntitiesOld
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_ENTITIES_OLD_H
#define SBCLIENT_ENTITIES_OLD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "entity_old.h"

// NeL includes

// STL includes
#include <map>

namespace SBCLIENT {

/**
 * \brief CEntitiesOld
 * \date 2008-02-07 10:55GMT
 * \author Jan Boon (Kaetemi)
 * \author Nevrax Ltd.
 * CEntitiesOld
 */
class CEntitiesOld
{
public:
	// types
	typedef std::map<uint32, CEntityOld> CEntityMap;

	// pointers
	CEntityOld *Self; // points to entity in map
	
	// instances
	// A map of entities. All entities are later referred by their unique id
	CEntityMap Entities;	

	//////// The size of the world, in meter
	//////float WorldWidth = 20 * 160;
	//////float WorldHeight = 6 * 160;

	// Entity Id, only used offline
	uint32 NextEID;

	// The speed settings
	static const float PlayerSpeed;
	static const float SnowballSpeed;

	// these variables are set with the config file

	//////// Setup for the name up the character
	//////float EntityNameSize;
	//////CRGBA EntityNameColor;

	bool _TestCLS;
public:
	CEntitiesOld();
	virtual ~CEntitiesOld();
}; /* class CEntitiesOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_ENTITIES_OLD_H */

/* end of file */
