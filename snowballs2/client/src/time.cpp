/**
 * \file time.cpp
 * \brief CTime
 * \date 2008-02-07 17:21GMT
 * \author Jan Boon (Kaetemi)
 * CTime
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
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
#include "time.h"

// Project includes
#include "member_callback_impl.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/misc/progress_callback.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CTime::CTime(IProgressCallback &progressCallback) :
GameTime(0.0), ServerTime(0.0), GameCycle(0), 
LocalTimeDelta(0.0), ServerTimeDelta(0.0), GameTimeDelta(0.0), 
FramesPerSecond(0.0f), _SecondsPerCycle(0.0), _GameTimePerCycle(0.0),
_GameTimePerSecond(0.0), _CyclesPerUpdate(0), _LastCycleUpdate(0.0),
_GameCycle(0.0), _NewGameCycle(0), _AnimateServer(false),
AnimationTime(0.0), AnimationDelta(0.0)
{
	progressCallback.progress(0.0f);

	_Time = NLMISC::CTime::getLocalTime();
	LocalTime = ((TLocalTime)_Time) / 1000.0;
	progressCallback.progress(1.0f);
}

CTime::~CTime()
{
	
}

SBCLIENT_CALLBACK_IMPL(CTime, updateTime)
{
	TTime time = NLMISC::CTime::getLocalTime();
	TTime delta = time - _Time;
	_Time = time;

	TLocalTime localTime = ((TLocalTime)time) / 1000.0;
	LocalTimeDelta = localTime - LocalTime;
	LocalTime = localTime;

	// if the game cycle went up
	if (_NewGameCycle > GameCycle)
	{
		TGameCycle gameCycles = _NewGameCycle - (TGameCycle)_GameCycle;
		GameCycle = _NewGameCycle;
		_SecondsPerCycle = (LocalTime - _LastCycleUpdate) / (TLocalTime)gameCycles;
		_LastCycleUpdate = LocalTime;
	}

	// if the delay is not longer than twice the delay between last update
	if ((TGameTime)_GameCycle < GameCycle + (_CyclesPerUpdate * 2))
	{
		_GameCycle += (LocalTime / _SecondsPerCycle);

		TGameTime gameTime = _GameCycle * _GameTimePerCycle;
		GameTimeDelta = gameTime - GameTime;
		GameTime = gameTime;

		TLocalTime serverTime = gameTime / _GameTimePerSecond;
		ServerTimeDelta = serverTime - ServerTime;
		ServerTime = serverTime;
	}

	_LastCycleUpdate += LocalTimeDelta;


	// component specific	
	if (_AnimateServer) AnimationDelta = ServerTimeDelta;
	else AnimationDelta = LocalTimeDelta;
	AnimationTime += AnimationDelta;

	nlinfo("CTime::updateTime: AnimationTime = %lf", AnimationTime);
}

void CTime::setGameCycle(TGameCycle gameCycle, TLocalTime secondsPerCycle, TGameTime gameTimePerSecond, TGameCycle cyclesPerUpdate)
{
	_GameCycle = (double)gameCycle;
	GameCycle = gameCycle;
	_NewGameCycle = gameCycle;
	_SecondsPerCycle = secondsPerCycle;
	_GameTimePerSecond = gameTimePerSecond;
	_GameTimePerCycle = gameTimePerSecond * secondsPerCycle;
	_LastCycleUpdate = (TLocalTime)NLMISC::CTime::getLocalTime();
	_CyclesPerUpdate = cyclesPerUpdate;
}

void CTime::updateGameCycle(TGameCycle gameCycle)
{
	_NewGameCycle = gameCycle;
}

} /* namespace SBCLIENT */

/* end of file */
