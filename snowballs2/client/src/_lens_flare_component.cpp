/**
 * \file lens_flare_component.cpp
 * \brief CLensFlareComponent
 * \date 2008-01-09 20:47GMT
 * \author Jan Boon (Kaetemi)
 * CLensFlareComponent
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS, Copyright (C) 2001  Nevrax Ltd.
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
#include "lens_flare_component.h"

#include "lens_flare.h"
#include "driver_component.h"
#include "scene_component.h"

#include <nel/misc/debug.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CLensFlareComponent::CLensFlareComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
	: IConfigurableComponent(manager, instanceId, progressCallback)
{
	// get the existing driver from the driver component, 
	// cannot change at runtime; asserts by itself
	_Driver = CDriverComponent::getDriver(
		_Config.getVar("DriverInstanceId").asString());

	// get the existing scene from the scene component
	// cannot change at runtime (for now); asserts by itself
	_Scene = ((CSceneComponent &)getInstance(
		_Config.getVar("SceneInstanceId").asString())).getScene();

	// create the lens flare
	_LensFlare = new CLensFlare(_Driver, _Scene->getCam());


	// -- -- getting this from a config file would be more re-usable
	// temp!
	flareTexture1 = _Driver->createTextureFile("flare01.tga");
	flareTexture3 = _Driver->createTextureFile("flare03.tga");
	flareTexture4 = _Driver->createTextureFile("flare04.tga");
	flareTexture5 = _Driver->createTextureFile("flare05.tga");
	flareTexture6 = _Driver->createTextureFile("flare06.tga");
	flareTexture7 = _Driver->createTextureFile("flare07.tga");

	float w = 30/800.0f; // -- ??? screen reso? bad!
	float h = 30/600.0f;

	// shine
	_LensFlare->addFlare(flareTexture3, w, h, 1.f, 16.f);
	_LensFlare->addFlare(flareTexture1, w, h, 1.f, 6.f);
	_LensFlare->addFlare(flareTexture6, w, h, 1.3f, 1.2f);
	_LensFlare->addFlare(flareTexture7, w, h, 1.0f, 3.f);
	_LensFlare->addFlare(flareTexture6, w, h, 0.5f, 4.f);
	_LensFlare->addFlare(flareTexture5, w, h, 0.2f, 2.f);
	_LensFlare->addFlare(flareTexture7, w, h, 0.0f, 0.8f);
	_LensFlare->addFlare(flareTexture7, w, h, -0.25f, 2.f);
	_LensFlare->addFlare(flareTexture1, w, h, -0.4f, 1.f);
	_LensFlare->addFlare(flareTexture4, w, h, -1.0f, 12.f);
	_LensFlare->addFlare(flareTexture5, w, h, -0.6f, 6.f);
}

CLensFlareComponent::~CLensFlareComponent()
{
	nlassert(_LensFlare); delete _LensFlare;
	_Driver->deleteTextureFile(flareTexture1); flareTexture1 = NULL;
	_Driver->deleteTextureFile(flareTexture3); flareTexture3 = NULL;
	_Driver->deleteTextureFile(flareTexture4); flareTexture4 = NULL;
	_Driver->deleteTextureFile(flareTexture5); flareTexture5 = NULL;
	_Driver->deleteTextureFile(flareTexture6); flareTexture6 = NULL;
	_Driver->deleteTextureFile(flareTexture7); flareTexture7 = NULL;
}

void CLensFlareComponent::update()
{
	nlwarning("Unknown IComponent update() call");
}

void CLensFlareComponent::render()
{
	_LensFlare->show();
}

void CLensFlareComponent::config(const string &varName, CConfigFile::CVar &var)
{
	nlwarning("Unknown IConfigurableComponent config(varName, var) call");
}

void CLensFlareComponent::componentUp(IComponent *component)
{
	nlwarning("Unknown IComponent componentUp(component) call");
}

void CLensFlareComponent::componentDown(IComponent *component)
{
	nlwarning("Unknown IComponent componentDown(component) call");
}

void CLensFlareComponent::setSunDirection(const NLMISC::CVector &direction)
{
	_LensFlare->setSunDirection(direction);
}

} /* namespace SBCLIENT */

/* end of file */
