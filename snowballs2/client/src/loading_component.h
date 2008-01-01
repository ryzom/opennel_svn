/** \file loading_component.h
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

#ifndef SBCLIENT_LOADING_COMPONENT_H
#define SBCLIENT_LOADING_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "component.h"

namespace SBCLIENT {
	class CComponentManager;
	class CLoading;

/**
 * Loading screen component.
 * Gets the loading screen from the IProgressCallback thingy,
 * and updates it's driver information, etc.
 * \author Name
 * \date Year
 */
class CLoadingComponent : public IComponent
{
protected:
	// pointers
	CLoading *_Loading; // not deleted here
	
	// instances
	// ...
public:	
	/// Basic constructor of a component.
	/// Requires a pointer to the componentmanager and a unique name.
	CLoadingComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CLoadingComponent();

	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
};

}

#endif /* #ifndef SBCLIENT_LOADING_COMPONENT_H */
