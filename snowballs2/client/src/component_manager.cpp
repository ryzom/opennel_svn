/** \file component_manager.cpp
 * CComponentManager
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
#include "component_manager.h"

#include "component.h"

#include <nel/misc/config_file.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CComponentManager::CComponentManager(CConfigFile *configFile)
{
	ConfigFile = configFile;
}

CComponentManager::~CComponentManager()
{
	
}

void CComponentManager::update()
{
	// call all update thingies
}

void CComponentManager::render()
{
	// call all render thingies
}

void CComponentManager::registerUpdate(IComponent *component)
{

}

void CComponentManager::unregisterUpdate(IComponent *component)
{

}

void CComponentManager::registerRender(IComponent *component)
{

}

void CComponentManager::unregisterRender(IComponent *component)
{

}

}
