/** \file configurable_component.h
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

#ifndef SBCLIENT_CONFIGURABLE_COMPONENT_H
#define SBCLIENT_CONFIGURABLE_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "sub_configuration.h"
#include "component.h"

namespace SBCLIENT {

/**
 * Base class for components that can be identified uniquely by name,
 * and make use of a configuration file to initialize themselves.
 * \author Jan Boon
 * \date 2007
 */
class IConfigurableComponent : public IComponent
{
protected:
	// pointers
	// ...
	
	// instances
	CSubConfiguration _Config;
public:	
	IConfigurableComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	virtual ~IConfigurableComponent();

	/// Force update of config value.
	/// Calls the virtual config callback of the component.
	void config(const std::string &varName);
protected:
	void registerConfigCallback(const std::string &varName);
	void registerAndCallConfigCallback(const std::string &varName);
	void unregisterConfigCallback(const std::string &varName);
	virtual void config(const std::string &varName, NLMISC::CConfigFile::CVar &var) { }
	static void _config(NLMISC::CConfigFile::CVar &var);
};

}

#endif /* #ifndef SBCLIENT_CONFIGURABLE_COMPONENT_H */
