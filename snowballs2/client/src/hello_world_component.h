/** \file hello_world_component.h
 * CHelloWorldComponent
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

#ifndef SBCLIENT_HELLO_WORLD_COMPONENT_H
#define SBCLIENT_HELLO_WORLD_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace NL3D {
	class UTextContext;
}

namespace SBCLIENT {
	class CComponentManager;

/**
 * A component that displays hello world by default.
 * The message can be modified in the config at runtime, 
 * and can make use of internationalized strings.
 * \author Name
 * \date Year
 */
class CHelloWorldComponent : public IConfigurableComponent
{
protected:
	// pointers
	NL3D::UTextContext *_TextContext; // not deleted here
	
	// instances
	ucstring _Message;
public:	
	/// Basic constructor of a component.
	/// Requires a pointer to the componentmanager and a unique name.
	CHelloWorldComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CHelloWorldComponent();

	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
};

}

#endif /* #ifndef SBCLIENT_HELLO_WORLD_COMPONENT_H */
