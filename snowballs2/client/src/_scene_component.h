/**
 * \file scene_component.h
 * \brief CSceneComponent
 * \date 2008-01-09 14:56GMT
 * \author Jan Boon (Kaetemi)
 * CSceneComponent
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

#ifndef SBCLIENT_SCENE_COMPONENT_H
#define SBCLIENT_SCENE_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace NL3D {
	class UScene;
	class UDriver;
}

namespace SBCLIENT {
	class CTimeComponent;

/**
 * \brief CSceneComponent
 * \date 2008-01-09 14:56GMT
 * \author Jan Boon (Kaetemi)
 * Component used to share a UScene between different other components.
 */
class CSceneComponent : public IConfigurableComponent
{
protected:
	// pointers
	NL3D::UScene *_Scene; // created and deleted here
	NL3D::UDriver *_Driver; // not deleted here
	CTimeComponent *_TimeComponent; // not deleted here
	
	// instances
	// ...
public:
	CSceneComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CSceneComponent();
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	void componentUp(IComponent *component);
	void componentDown(IComponent *component);

	NL3D::UScene *getScene();
}; /* class CSceneComponent */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_SCENE_COMPONENT_H */

/* end of file */
