/**
 * \file trajectory_old.cpp
 * \brief CTrajectoryOld
 * \date 2008-02-07 11:15GMT
 * \author Nevrax Ltd.
 * CTrajectoryOld
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
#include "trajectory_old.h"

// Project includes

// NeL includes
// #include <nel/misc/debug.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CVector	CTrajectoryOld::eval(NLMISC::TTime t) const
{
	if (t < _StartTime) return _StartPosition;

	float	ft = (float)(t-_StartTime)/(float)(_StopTime-_StartTime);
	ft = min(ft, 1.0f);
	CVector	res = _EndPosition*ft + _StartPosition*(1.0f-ft);
	res.z += 0.3f*_Distance*_Distance/90.0f*(float)sin(Pi*ft);
	return res;
}

CVector	CTrajectoryOld::evalSpeed(NLMISC::TTime t) const
{
	if (t < _StartTime) return _StartPosition;

	float	ft = (float)(t-_StartTime)/(float)(_StopTime-_StartTime);
	ft = min(ft, 1.0f);
	CVector res = (_EndPosition-_StartPosition).normed()*_Speed;
	res.z += 0.3f*_Distance*_Distance/90.0f*(float)Pi*(float)cos(Pi*ft);
	return res;
}

} /* namespace SBCLIENT */

/* end of file */
