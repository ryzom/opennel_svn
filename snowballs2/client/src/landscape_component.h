/**
 * \file landscape_component.h
 * \brief CLandscapeComponent
 * \date 2007-12-29 19:37GMT
 * \author Jan Boon (Kaetemi)
 * CLandscapeComponent
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2007-2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS 0.2, Copyright (C) 2001  Nevrax Ltd.
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

#ifndef SBCLIENT_LANDSCAPE_COMPONENT_H
#define SBCLIENT_LANDSCAPE_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace NL3D {
	class ULandscape;
	class UScene;
}
namespace SBCLIENT {

/**
 * \brief CLandscapeComponent
 * \date 2007-12-29 19:37GMT
 * \author Jan Boon (Kaetemi)
 * CLandscapeComponent
 */
class CLandscapeComponent : public IConfigurableComponent
{
protected:
	// pointers
	NL3D::ULandscape *_Landscape; // deleted here
	NL3D::UScene *_Scene; // get in constructor
	
	// instances
	float _cfgVision;
	float _cfgVisionInitial;
public:
	CLandscapeComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CLandscapeComponent();
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	void componentUp(IComponent *component);
	void componentDown(IComponent *component);

	void loadAllZonesAround();
}; /* class CLandscapeComponent */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LANDSCAPE_COMPONENT_H */

/* end of file */
