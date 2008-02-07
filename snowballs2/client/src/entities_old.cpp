/**
 * \file entities_old.cpp
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

#include <nel/misc/types_nl.h>
#include "entities_old.h"

// Project includes

// NeL includes
// #include <nel/misc/debug.h>

// STL includes

using namespace std;
// using namespace NLMISC;

namespace SBCLIENT {

const float CEntitiesOld::PlayerSpeed = 10.0f;	// 6.5 km/h
const float CEntitiesOld::SnowballSpeed = 15.0f;	// 36 km/h

CEntitiesOld::CEntitiesOld() : _TestCLS(false), Self(NULL), 
NextEID(1000000)
{
	
}

CEntitiesOld::~CEntitiesOld()
{
	
}

} /* namespace SBCLIENT */

/* end of file */
