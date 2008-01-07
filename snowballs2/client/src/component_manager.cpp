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
#include <nel/3d/u_driver.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CComponentManager::CComponentManager(CConfigFile *configFile)
: _ConfigFile(configFile), _Driver(NULL)
{
	
}

CComponentManager::~CComponentManager()
{
	
}

void CComponentManager::update()
{
	CConfigFile::checkConfigFiles();

	// call all update thingies
	// they can have render loops inside them (for loading screen etc)
	for (std::list<IComponent *>::iterator it = _Updates.begin(); it != _Updates.end(); it++)
		(*it)->update();
}

void CComponentManager::render()
{
	if (_Driver->isLost()) nlSleep(10);
	else
	{
		// clear all buffers
		_Driver->clearBuffers(CRGBA(0, 0, 0, 255));

		// call all render thingies
		for (std::list<IComponent *>::iterator it = _Renders.begin(); it != _Renders.end(); it++)
			(*it)->render();
	
		// swap 3d buffers
		_Driver->swapBuffers();
	}	
}

void CComponentManager::registerUpdate(IComponent *component, sint32 priority)
{
	component->UpdatePriority = priority;
	for (std::list<IComponent *>::iterator it = _Updates.begin(); it != _Updates.end(); it++)
		if ((*it)->UpdatePriority > priority) { _Updates.insert(it, component); return; }
	_Updates.push_back(component);
}

void CComponentManager::unregisterUpdate(IComponent *component)
{
	_Updates.remove(component);
}

void CComponentManager::registerRender(IComponent *component, sint32 priority)
{
	component->RenderPriority = priority;
	for (std::list<IComponent *>::iterator it = _Renders.begin(); it != _Renders.end(); it++)
		if ((*it)->RenderPriority > priority) { _Renders.insert(it, component); return; }
	_Renders.push_back(component);
}

void CComponentManager::unregisterRender(IComponent *component)
{
	_Renders.remove(component);
}

void CComponentManager::registerComponent(IComponent *component)
{
	_Components.push_back(component);
	std::list<IComponent *> notifiers = _Notifiers[component->getInstanceId()];
	for (std::list<IComponent *>::iterator it = notifiers.begin(); it != notifiers.end(); it++)
		(*it)->componentUp(component);
}

void CComponentManager::unregisterComponent(IComponent *component)
{
	_Components.remove(component);
	std::list<IComponent *> notifiers = _Notifiers[component->getInstanceId()];
	for (std::list<IComponent *>::iterator it = notifiers.begin(); it != notifiers.end(); it++)
		(*it)->componentDown(component);
	for (std::list<IComponent *>::iterator it = _Updates.begin(); it != _Updates.end(); it++)
		if ((*it) == component) { _Updates.remove(component); return; }
	for (std::list<IComponent *>::iterator it = _Renders.begin(); it != _Renders.end(); it++)
		if ((*it) == component) { _Renders.remove(component); return; }
}

void CComponentManager::setDriver(NL3D::UDriver *driver)
{
	// make sure the driver isn't set yet
	if (driver) nlassert(!_Driver);
	// set the driver (it can be set back to NULL)
	_Driver = driver;
}

void CComponentManager::registerNotifier(IComponent *source, const std::string &target)
{
	if (!target.empty())
	{
		if (_Notifiers.find(target) == _Notifiers.end()) _Notifiers[target] = std::list<IComponent *>();
		_Notifiers[target].push_back(source);
		IComponent *component = IComponent::getInstancePtr(target);
		if (component) source->componentUp(component);
	}
}

void CComponentManager::unregisterNotifier(IComponent *source, const std::string &target)
{
	if (!target.empty())
	{
		_Notifiers[target].remove(source);
		if (_Notifiers[target].size() == 0) _Notifiers.erase(target);
		IComponent *component = IComponent::getInstancePtr(target);
		if (component) source->componentDown(component);
	}
}

}
