/** \file time_component.cpp
 * CTimeComponent
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * $Id$
 */

#include <nel/misc/types_nl.h>
#include "time_component.h"

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CTimeComponent::CTimeComponent(CComponentManager *manager, 
	const string &instanceId, IProgressCallback &progressCallback)
: IComponent(manager, instanceId, progressCallback), 
  GameTime(0.0), ServerTime(0.0), GameCycle(0), 
  LocalTimeDelta(0.0), ServerTimeDelta(0.0), GameTimeDelta(0.0), 
  FramesPerSecond(0.0f), _SecondsPerCycle(0.0), _GameTimePerCycle(0.0),
  _GameTimePerSecond(0.0), _CyclesPerUpdate(0), _LastCycleUpdate(0.0),
  _GameCycle(0.0), _NewGameCycle(0)
{
	_Time = CTime::getLocalTime();
	LocalTime = (TLocalTime)_Time;
}

CTimeComponent::~CTimeComponent()
{
	
}

void CTimeComponent::update()
{
	TTime time = CTime::getLocalTime();
	TTime delta = time - _Time;
	_Time = time;

	LocalTime = (TLocalTime)time;

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
}

void CTimeComponent::render()
{
	
}

void CTimeComponent::config(const string &varName, CConfigFile::CVar &var)
{
	
}

void CTimeComponent::setGameCycle(TGameCycle gameCycle, TLocalTime secondsPerCycle, TGameTime gameTimePerSecond, TGameCycle cyclesPerUpdate)
{
	_GameCycle = (double)gameCycle;
	GameCycle = gameCycle;
	_NewGameCycle = gameCycle;
	_SecondsPerCycle = secondsPerCycle;
	_GameTimePerSecond = gameTimePerSecond;
	_GameTimePerCycle = gameTimePerSecond * secondsPerCycle;
	_LastCycleUpdate = (TLocalTime)CTime::getLocalTime();
	_CyclesPerUpdate = cyclesPerUpdate;
}

void CTimeComponent::updateGameCycle(TGameCycle gameCycle)
{
	_NewGameCycle = gameCycle;
}

}
