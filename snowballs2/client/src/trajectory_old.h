/**
 * \file trajectory_old.h
 * \brief CTrajectoryOld
 * \date 2008-02-07 11:15GMT
 * \author Nevrax Ltd.
 * Physics computation for particles (and thus snowballs)
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

#ifndef SBCLIENT_TRAJECTORY_OLD_H
#define SBCLIENT_TRAJECTORY_OLD_H
#include <nel/misc/types_nl.h>

// Project includes

// NeL includes
#include <nel/misc/vector.h>
#include <nel/misc/time_nl.h>

// STL includes

namespace SBCLIENT {

/**
 * \brief CTrajectoryOld
 * \date 2008-02-07 11:15GMT
 * \author Nevrax Ltd.
 * CTrajectoryOld
 */
class CTrajectoryOld
{
	NLMISC::CVector		_StartPosition;
	NLMISC::CVector		_EndPosition;
	float				_Speed;
	NLMISC::TTime		_StartTime;
	NLMISC::TTime		_StopTime;
	float				_Distance;

public:
	void				init(const NLMISC::CVector &position, const NLMISC::CVector &target, float speed, NLMISC::TTime startTime)
	{
		_StartPosition = position;
		_EndPosition = target;
		_Speed = speed;
		_StartTime = startTime;
		_Distance = (_EndPosition-_StartPosition).norm();
		_StopTime = (NLMISC::TTime)(_Distance/_Speed*1000.0+_StartTime);
	}

	NLMISC::CVector		eval(NLMISC::TTime t) const;
	NLMISC::CVector		evalSpeed(NLMISC::TTime t) const;

	NLMISC::TTime		getStartTime() const { return _StartTime; }
	NLMISC::CVector		getStartPosition() const { return _StartPosition; }
	NLMISC::TTime		getStopTime() const { return _StopTime; }
}; /* class CTrajectoryOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_TRAJECTORY_OLD_H */

/* end of file */
