/** \file configurable_component.cpp
 * IConfigurableComponent
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
#include "configurable_component.h"

#include "component_manager.h"

#include <nel/misc/app_context.h>
#include <nel/misc/i18n.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

IConfigurableComponent::IConfigurableComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
: IComponent(manager, instanceId, progressCallback)
{
	// initialize the config file manager
	_Config.setConfigFile(manager->getConfig(), instanceId);
}

IConfigurableComponent::~IConfigurableComponent()
{
	// ...
}

void IConfigurableComponent::registerConfigCallback(const string &varName)
{
	_Config.setCallback(varName, _config);
}

void IConfigurableComponent::registerAndCallConfigCallback(const string &varName)
{
	registerConfigCallback(varName);
	config(varName);
}

void IConfigurableComponent::unregisterConfigCallback(const string &varName)
{
	_Config.setCallback(varName, NULL);
}

void IConfigurableComponent::config(const std::string &varName)
{
	config(varName, _Config.getVar(varName));
}

void IConfigurableComponent::_config(NLMISC::CConfigFile::CVar &var)
{
	vector<string> nameVector;
	explode(var.Name, string("_"), nameVector, false);
	uint nameVectorSize = nameVector.size();
	std::string instanceId = nameVector[0];
	IConfigurableComponent *instance = (IConfigurableComponent *)getInstancePtr(instanceId);
	uint i = 1;
	while (!instance)
	{
		if (i > nameVectorSize - 2) // need extra for var name
		{
			nlwarning("Config var callback %s not linked to any IComponent", var.Name.c_str());
			return;
		}
		instanceId = instanceId + string("_") + nameVector[i];
		++i;
	}
	std::string varName = nameVector[i]; ++i;
	while (i < nameVectorSize)
	{
		varName = varName + string("_") + nameVector[i];
		++i;
	}
	instance->config(varName, var);
}

}
