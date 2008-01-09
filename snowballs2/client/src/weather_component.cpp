/**
 * \file weather_component.cpp
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

#include <nel/misc/types_nl.h>
#include "weather_component.h"

#include "driver_component.h"
#include "time_component.h"
#include "scene_component.h"

#include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_light.h>
#include <nel/3d/u_cloud_scape.h>
#include <nel/3d/u_camera.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CWeatherComponent::CWeatherComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
	: IConfigurableComponent(manager, instanceId, progressCallback)
{
	// get the existing driver from the driver component, 
	// cannot change at runtime; asserts by itself
	_Driver = CDriverComponent::getDriver(
		_Config.getVar("DriverInstanceId").asString());

	// get the existing time component
	// cannot change at runtime; asserts by itself
	_TimeComponent = &((CTimeComponent &)getInstance(
		_Config.getVar("TimeInstanceId").asString()));

	// get the existing scene from the scene component
	// cannot change at runtime; asserts by itself
	_GameScene = ((CSceneComponent &)getInstance(
		_Config.getVar("SceneInstanceId").asString())).getScene();

	// create the sun
	_Sun = ULight::createLight(); nlassert(_Sun);
	_Sun->setMode(ULight::DirectionalLight);
	_Driver->enableLight(0);
	// some defaults for now
	_Sun->setAmbiant(CRGBA(255, 255, 255));
	_Sun->setDiffuse(CRGBA(255, 255, 255));
	_Sun->setSpecular(CRGBA(255, 255, 255));
	_Sun->setDirection(CVector(-2.935f, 0.107f, -1.22f));
	_Driver->setLight(0, *_Sun);

	// create the sky scene
	_SkyScene = _Driver->createScene(false);
	UCamera sky_camera = _SkyScene->getCam();
	sky_camera.setTransformMode(UTransformable::DirectMatrix);
	// set the very same frustum as the main camera
	sky_camera.setFrustum(_GameScene->getCam().getFrustum());

	// create the sky instance
	_Sky = _SkyScene->createInstance("sky.shape");
	_Sky.setTransformMode(UTransformable::DirectMatrix);
	_Sky.setMatrix(CMatrix::Identity);

	// create the cloud scape
	_Clouds = _GameScene->createCloudScape();
	// create a default cloud scape setup
	SCloudScapeSetup css;
	// speed em up a bit
	css.WindSpeed *= 4.0f;
	css.CloudSpeed *= 4.0f;
	// the cloud scape makes a copy of the css
	_Clouds->init(&css);
	// some settings
	_Clouds->setQuality(160);
	_Clouds->setNbCloudToUpdateIn80ms(1);

	// create the snowing particle system
	_Snow = _GameScene->createInstance("snow.ps");
	_Snow.setTransformMode (UTransformable::DirectMatrix);
}

CWeatherComponent::~CWeatherComponent()
{
	_GameScene->deleteInstance(_Snow);
	_GameScene->deleteCloudScape(_Clouds);
	_SkyScene->deleteInstance(_Sky);
	_Driver->deleteScene(_SkyScene);
}

void CWeatherComponent::update()
{
	// animate the clouds
	_Clouds->anim(_TimeComponent->AnimationDelta);

	// update the position of the sky camera
	CMatrix sky_camera_matrix = _GameScene->getCam().getMatrix();
	sky_camera_matrix.setPos(CVector::Null);
	_SkyScene->getCam().setMatrix(sky_camera_matrix);

	// animate the sky scene
	_SkyScene->animate(_TimeComponent->AnimationTime);
	
	// set the new position of the snow emitter
	// -- -- see what happens if doing this on the character position instead
	CMatrix	mat = CMatrix::Identity;
	mat.setPos(_GameScene->getCam().getMatrix().getPos());
	_Snow.setMatrix(mat);
}

void CWeatherComponent::render()
{
	// render the sky scene
	_SkyScene->render();

	// must clear z-buffer for incoming rendering
	_Driver->clearZBuffer();

	// render the clouds
	_Clouds->render();
}

void CWeatherComponent::config(const string &varName, CConfigFile::CVar &var)
{
	nlwarning("Unknown IConfigurableComponent config(varName, var) call");
}

void CWeatherComponent::componentUp(IComponent *component)
{
	nlwarning("Unknown IComponent componentUp(component) call");
}

void CWeatherComponent::componentDown(IComponent *component)
{
	nlwarning("Unknown IComponent componentDown(component) call");
}

} /* namespace SBCLIENT */

/* end of file */
