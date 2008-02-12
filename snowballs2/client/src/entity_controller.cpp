/**
 * \file entity_controller.cpp
 * \brief CEntityController
 * \date 2008-02-10 14:26GMT
 * \author Jan Boon (Kaetemi)
 * CEntityController
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
#include "entity_controller.h"

// Project includes

// NeL includes
// #include <nel/misc/debug.h>

// STL includes

using namespace std;
// using namespace NLMISC;

namespace SBCLIENT {

CEntityController::CEntityController()
{
	
}

CEntityController::~CEntityController()
{
	
}

void CEntityController::update()
{
	// todo:
	// - read rotate bools
	// - update rotation
	// - read move bools
	// - update move (using direction set by rotation)
	// (rotation is the actial direction the character is facing)
}

} /* namespace SBCLIENT */

/* end of file */
