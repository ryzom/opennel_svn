/**
 * \file scene_component.cpp
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

#include <nel/misc/types_nl.h>
#include "scene_component.h"

#include "driver_component.h"
#include "time_component.h"

#include <nel/misc/debug.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_driver.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CSceneComponent::CSceneComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
	: IConfigurableComponent(manager, instanceId, progressCallback)
{
	// get the existing driver component, 
	// cannot change at runtime for now; asserts by itself
	CDriverComponent &dc = (CDriverComponent &)getInstance(
		_Config.getVar("DriverInstanceId").asString());

	// get the driver from the component
	_Driver = dc.getDriver(); // asserts by itself

	// get the existing time component
	// cannot change at runtime; asserts by itself
	_TimeComponent = &((CTimeComponent &)getInstance(
		_Config.getVar("TimeInstanceId").asString()));

	// create a scene
	_Scene = _Driver->createScene(false);
	nlassert(_Scene);
}

CSceneComponent::~CSceneComponent()
{
	// destroy the scene
	_Driver->deleteScene(_Scene);	
}

void CSceneComponent::update()
{
	_Scene->animate(_TimeComponent->AnimationTime);
}

void CSceneComponent::render()
{
	_Scene->render();
}

void CSceneComponent::config(const string &varName, CConfigFile::CVar &var)
{
	nlwarning("Unknown IConfigurableComponent config(varName, var) call");
}

void CSceneComponent::componentUp(IComponent *component)
{
	nlwarning("Unknown IComponent componentUp(component) call");
}

void CSceneComponent::componentDown(IComponent *component)
{
	nlwarning("Unknown IComponent componentDown(component) call");
}

UScene *CSceneComponent::getScene()
{
	nlassert(_Scene); return _Scene;
}

} /* namespace SBCLIENT */

/* end of file */
