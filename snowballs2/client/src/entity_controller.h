/**
 * \file entity_controller.h
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

#ifndef SBCLIENT_ENTITY_CONTROLLER_H
#define SBCLIENT_ENTITY_CONTROLLER_H
#include <nel/misc/types_nl.h>

// Project includes

// NeL includes
#include <nel/misc/vector.h>

// STL includes

namespace SBCLIENT {

/**
 * \brief CEntityController
 * \date 2008-02-10 14:26GMT
 * \author Jan Boon (Kaetemi)
 * CEntityController
 */
class CEntityController
{
public:
	// set
	bool MoveForward, MoveBackward, MoveLeft, MoveRight;
	bool RotateLeft, RotateRight;
	float Speed;
	// get
	NLMISC::CVector Move;
	float Rotation;
public:
	CEntityController();
	virtual ~CEntityController();

	void update();
}; /* class CEntityController */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_ENTITY_CONTROLLER_H */

/* end of file */
