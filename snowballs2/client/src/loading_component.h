/**
 * \file loading_component.h
 * \brief CLoadingComponent
 * \date 2008-01-01 22:30GMT
 * \author Jan Boon (Kaetemi)
 * CLoadingComponent
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_LOADING_COMPONENT_H
#define SBCLIENT_LOADING_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace SBCLIENT {
	class CComponentManager;
	class CLoadingScreen;

/**
 * \brief CLoadingComponent
 * \date 2008-01-01 22:30GMT
 * \author Jan Boon (Kaetemi)
 * Loading screen component.
 * Gets the loading screen from the IProgressCallback thingy,
 * and updates it's driver information, etc.
 */
class CLoadingComponent : public IConfigurableComponent
{
protected:
	// pointers
	CLoadingScreen *_LoadingScreen; // not deleted here
	
	// instances
	std::string _DriverInstanceId;
public:
	CLoadingComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CLoadingComponent();
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	void componentUp(IComponent *component);
	void componentDown(IComponent *component);
}; /* class CLoadingComponent */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LOADING_COMPONENT_H */

/* end of file */
