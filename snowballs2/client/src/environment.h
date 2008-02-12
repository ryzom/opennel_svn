/**
 * \file environment.h
 * \brief CEnvironment
 * \date 2008-02-12 13:50GMT
 * \author Jan Boon (Kaetemi)
 * CEnvironment
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS, Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of OpenNeL Snowballs.
 * OpenNeL Snowballs is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * OpenNeL Snowballs is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenNeL Snowballs; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_ENVIRONMENT_H
#define SBCLIENT_ENVIRONMENT_H
#include <nel/misc/types_nl.h>

// Project includes
#include "config_proxy.h"
#include "lens_flare.h"
#include "member_callback_decl.h"

// NeL includes
#include <nel/3d/u_instance.h>

// STL includes

namespace NL3D {
	class UDriver;
	class UScene;
	class UCloudScape;
}

namespace NLMISC {
	class IProgressCallback;
}

namespace SBCLIENT {
	class CTime;

/**
 * \brief CEnvironment
 * \date 2008-02-12 13:50GMT
 * \author Jan Boon (Kaetemi)
 * CEnvironment
 */
class CEnvironment
{
protected:
	// pointers
	NL3D::UDriver *_Driver; // p
	NL3D::UScene *_Scene; // p
	CTime *_Time; // p
	NL3D::UScene *_SkyScene; // P _Driver
	NL3D::UCloudScape *_Clouds; // P _Scene
	NL3D::UInstance _Snow; // _Scene
	NL3D::UInstance _Sky; // _SkyScene
	
	// instances
	CConfigProxy _Config;
	CLensFlare _LensFlare;
public:
	CEnvironment(NLMISC::IProgressCallback &progress, const std::string &id, NL3D::UDriver *driver, NL3D::UScene *scene, CTime *time);
	virtual ~CEnvironment();

	SBCLIENT_CALLBACK_DECL(updateWeather);
	SBCLIENT_CALLBACK_DECL(renderSky);
	SBCLIENT_CALLBACK_DECL(renderEffects);

	SBCLIENT_CALLBACK_CONFIG_DECL(configSunDirection);
	SBCLIENT_CALLBACK_CONFIG_DECL(configSunAmbient);
	SBCLIENT_CALLBACK_CONFIG_DECL(configSunDiffuse);
	SBCLIENT_CALLBACK_CONFIG_DECL(configSunSpecular);
}; /* class CEnvironment */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_ENVIRONMENT_H */

/* end of file */
