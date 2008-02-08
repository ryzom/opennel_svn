/**
 * \file entity_old.cpp
 * \brief CEntityOld
 * \date 2008-02-07 10:58GMT
 * \author Nevrax Ltd.
 * CEntityOld
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
#include "entity_old.h"

// Project includes

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/misc/time_nl.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CEntityOld::CEntityOld() : Id(0xffffffff), Name("<Unknown>"), 
AutoMove(false), Instance(NULL), Skeleton(NULL), Particule(NULL), 
PlayList(NULL), /*CurrentAnim(NoAnim), */NextEmptySlot(0), Source (NULL), 
Angle(0.0f), AuxiliaryAngle(0.0f), InterpolatedAuxiliaryAngle(0.0f), 
IsWalking(false), WasWalking(false), IsAiming(false), WasAiming(false), 
BotState(0)
{
	
}

CEntityOld::~CEntityOld()
{
	
}

void CEntityOld::setState(TState state, NL3D::TGlobalAnimationTime gt)
{
	State = state;
	StateStartTime = gt;
}

} /* namespace SBCLIENT */

/* end of file */
