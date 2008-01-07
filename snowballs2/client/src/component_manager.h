/** \file component_manager.h
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

#ifndef SBCLIENT_COMPONENT_MANAGER_H
#define SBCLIENT_COMPONENT_MANAGER_H
#include <nel/misc/types_nl.h>

#include <map>

namespace NL3D {
	class UDriver;
}

namespace NLMISC {
	class CConfigFile;
}

namespace SBCLIENT {
	class IComponent;

// todo: implement the component manager
/**
 * Component manager.
 * \date 2007-2008
 */
class CComponentManager
{
protected:	
	// pointers
	NLMISC::CConfigFile *_ConfigFile; // not deleted here
	NL3D::UDriver *_Driver; // not deleted here, can be NULL, temp
	
	// instances
	std::list<IComponent *>	_Components;
	std::list<IComponent *>	_Updates;
	std::list<IComponent *>	_Renders;
	std::map<std::string, std::list<IComponent *>> _Notifiers;
public:
	CComponentManager(NLMISC::CConfigFile *configFile);
	virtual ~CComponentManager();

	void update();
	void render();

	void registerUpdate(IComponent *component, sint32 priority);
	void unregisterUpdate(IComponent *component);
	void registerRender(IComponent *component, sint32 priority);
	void unregisterRender(IComponent *component);

	/// Called when a component is fully initialized,
	/// other components will be notified.
	/// (not called by component itself, but by the creator)
	/// (you don't need to call it if the instance isn't shared)
	void registerComponent(IComponent *component);
	/// Called when a component will be destroyed,
	/// other components will be notified.
	/// (not called by the component itself, but by the creator)
	/// Also automagically removes the component from update and render.
	void unregisterComponent(IComponent *component);

	/// Called by the driver component.
	/// Only used internally.
	void setDriver(NL3D::UDriver *driver);

	/// Called by components to get the base config file.
	/// They can automatically switch to a diffent one.
	NLMISC::CConfigFile *getConfig() { return _ConfigFile; }

	/// Tells the source IComponent when target comes up or goes down
	/// Automagically tells so when target is already up or too.
	/// Does nothing if target is an empty string.
	void registerNotifier(IComponent *source, const std::string &target);
	/// This will call componentDown if the target is still up.
	void unregisterNotifier(IComponent *source, const std::string &target);
};

}

#endif /* #ifndef SBCLIENT_COMPONENT_MANAGER_H */
