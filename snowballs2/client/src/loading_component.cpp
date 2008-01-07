/** \file loading_component.cpp
 * CLoadingComponent
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
#include "loading_component.h"

#include "driver_component.h"
#include "loading_screen.h"

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CLoadingComponent::CLoadingComponent(CComponentManager *manager, 
	const string &instanceId, IProgressCallback &progressCallback)
: IConfigurableComponent(manager, instanceId, progressCallback)
{
	_LoadingScreen = (CLoadingScreen *)(&progressCallback);
	registerAndCallConfigCallback("DriverInstanceId");
}

CLoadingComponent::~CLoadingComponent()
{
	
}

void CLoadingComponent::update()
{
	
}

void CLoadingComponent::render()
{
	
}

void CLoadingComponent::config(const string &varName, CConfigFile::CVar &var)
{
	if (varName == "DriverInstanceId")
	{
		unregisterNotifier(_DriverInstanceId);
		_DriverInstanceId = var.asString();
		registerNotifier(_DriverInstanceId);
	}
	else nlwarning("Unknown IComponent config call");
}

void CLoadingComponent::componentUp(IComponent *component)
{
	std::string instanceId = component->getInstanceId();
	if (instanceId == _DriverInstanceId)
	{
		CDriverComponent *c = (CDriverComponent *)component;
		_LoadingScreen->setDriver(c->getDriver());
		_LoadingScreen->setTextContext(c->getTextContext());
	}
	else nlwarning("Unknown IComponent componentUp call");
}

void CLoadingComponent::componentDown(IComponent *component)
{
	std::string instanceId = component->getInstanceId();
	if (instanceId == _DriverInstanceId)
	{
		_LoadingScreen->setDriver(NULL);
		_LoadingScreen->setTextContext(NULL);
	}
	else nlwarning("Unknown IComponent componentDown call");
}

}
