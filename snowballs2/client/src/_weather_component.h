/**
 * \file weather_component.h
 * \brief CWeatherComponent
 * \date 2008-01-09 16:59GMT
 * \author Jan Boon (Kaetemi)
 * CWeatherComponent
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
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

#ifndef SBCLIENT_WEATHER_COMPONENT_H
#define SBCLIENT_WEATHER_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

#include <nel/3d/u_instance.h>

namespace NL3D {
	class UScene;
	class UDriver;
	class ULight;
	class UCloudScape;
}

namespace SBCLIENT {
	class CTimeComponent;
	class CSceneComponent;
	class CLensFlareComponent;

/**
 * \brief CWeatherComponent
 * \date 2008-01-09 16:59GMT
 * \author Jan Boon (Kaetemi)
 * CWeatherComponent
 */
class CWeatherComponent : public IConfigurableComponent
{
protected:
	// pointers
	CTimeComponent *_TimeComponent; // not deleted here
	CLensFlareComponent *_LensFlareComponent; // not deleted here
	NL3D::UDriver *_Driver; // not deleted here
	NL3D::UScene *_GameScene; // not deleted here
	NL3D::UScene *_SkyScene; // created and deleted here trough _Driver
	NL3D::ULight *_Sun; // created and deleted here trough _Driver
	NL3D::UCloudScape *_Clouds; // created and deleted here trough _GameScene

	// instances
	NL3D::UInstance _Sky; // created and deleted here trough _SkyScene
	NL3D::UInstance _Snow; // created and deleted here trough _GameScene
	std::string _LensFlareInstanceId;
public:
	CWeatherComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CWeatherComponent();
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	void componentUp(IComponent *component);
	void componentDown(IComponent *component);
}; /* class CWeatherComponent */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_WEATHER_COMPONENT_H */

/* end of file */
