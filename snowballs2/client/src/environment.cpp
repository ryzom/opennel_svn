/**
 * \file environment.cpp
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

#include <nel/misc/types_nl.h>
#include "environment.h"

// Project includes
#include "member_callback_impl.h"
#include "time.h"

// NeL includes
#include <nel/misc/debug.h>
#include <nel/misc/progress_callback.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_cloud_scape.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CEnvironment::CEnvironment(IProgressCallback &progress, const string &id, 
UDriver *driver, UScene *scene, CTime *time) : _Config(id), _Driver(driver), 
_Scene(scene), _LensFlare(driver, scene->getCam()), _Time(time)
{
	progress.progress(0.00f);
	nlassert(_Driver);
	nlassert(_Scene); // ..
	
	// todo create scene here
	_Scene->enableLightingSystem(true);
	_Config.setCallbackAndCall("SunDirection", configSunDirection, this, NULL);
	_Config.setCallbackAndCall("SunAmbient", configSunAmbient, this, NULL);
	_Config.setCallbackAndCall("SunDiffuse", configSunDiffuse, this, NULL);
	_Config.setCallbackAndCall("SunSpecular", configSunSpecular, this, NULL);

	// create the sky scene
	_SkyScene = _Driver->createScene(false);
	UCamera sky_camera = _SkyScene->getCam();
	sky_camera.setTransformMode(UTransformable::DirectMatrix);
	// set the very same frustum as the main camera
	sky_camera.setFrustum(_Scene->getCam().getFrustum());

	// create the sky instance
	_Sky = _SkyScene->createInstance("sky.shape");
	_Sky.setTransformMode(UTransformable::DirectMatrix);
	_Sky.setMatrix(CMatrix::Identity);
	
	// create the cloud scape
	_Clouds = _Scene->createCloudScape();
	// create a default cloud scape setup
	SCloudScapeSetup css;
	// speed em up a bit
	//css.WindSpeed *= 4.0f;
	//css.CloudSpeed *= 4.0f;
	// the cloud scape makes a copy of the css
	_Clouds->init(&css);
	// some settings
	_Clouds->setQuality(160);
	_Clouds->setNbCloudToUpdateIn80ms(1);

	// create the snowing particle system
	_Snow = _Scene->createInstance("snow.ps");
	_Snow.setTransformMode(UTransformable::DirectMatrix);

	// set the fog (temp)
	_Driver->enableFog(false);
	_Driver->setupFog(100.0f, 250.0f, CRGBA(80, 77, 118));

	progress.progress(1.00f);
}

CEnvironment::~CEnvironment()
{
	_Scene->deleteInstance(_Snow);
	_Scene->deleteCloudScape(_Clouds);
	_SkyScene->deleteInstance(_Sky);
	_Driver->deleteScene(_SkyScene);
	// _Driver->deleteScene(_Scene);
}

SBCLIENT_CALLBACK_IMPL(CEnvironment, updateWeather)
{
	// animate the clouds
	_Clouds->anim(_Time->AnimationDelta);

	// update the position of the sky camera
	UCamera &scene_camera = _Scene->getCam();
	CMatrix sky_camera_matrix = scene_camera.getMatrix();
	sky_camera_matrix.setPos(CVector::Null);
	_SkyScene->getCam().setMatrix(sky_camera_matrix);

	// animate the sky scene
	_SkyScene->animate(_Time->AnimationTime);
	
	// set the new position of the snow emitter
	// todo: character position!
	CMatrix	mat = CMatrix::Identity;
	mat.setPos(scene_camera.getMatrix().getPos());
	_Snow.setMatrix(mat);
}

SBCLIENT_CALLBACK_IMPL(CEnvironment, renderSky)
{
	// render the sky scene
	_SkyScene->render();

	// must clear z-buffer for incoming rendering
	// todo: material of sky without z buffer?
	_Driver->clearZBuffer();

	// render the clouds
	_Clouds->render();
}

SBCLIENT_CALLBACK_IMPL(CEnvironment, renderEffects)
{
	_LensFlare.show();
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CEnvironment, configSunAmbient)
{
	//_Scene->setSunAmbient(CRGBA(255, 255, 255));
	_Scene->setSunAmbient(CRGBA(
		var.asInt(0), var.asInt(1), var.asInt(2)));
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CEnvironment, configSunDiffuse)
{
	//_Scene->setSunDiffuse(CRGBA(255, 255, 255));
	_Scene->setSunDiffuse(CRGBA(
		var.asInt(0), var.asInt(1), var.asInt(2)));
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CEnvironment, configSunSpecular)
{
	//_Scene->setSunSpecular(CRGBA(255, 255, 255));
	_Scene->setSunSpecular(CRGBA(
		var.asInt(0), var.asInt(1), var.asInt(2)));
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CEnvironment, configSunDirection)
{
	// todo
	CVector direction = CVector(-2.935f, 0.107f, -1.22f);
	_Scene->setSunDirection(direction);
	_LensFlare.setSunDirection(direction);
}

} /* namespace SBCLIENT */

/* end of file */
