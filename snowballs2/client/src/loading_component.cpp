/**
 * \file loading_component.cpp
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

#include <nel/misc/types_nl.h>
#include "loading_component.h"

// #include <nel/misc/debug.h>
#include "driver_component.h"
#include "loading_screen.h"

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CLoadingComponent::CLoadingComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
	: IConfigurableComponent(manager, instanceId, progressCallback)
{
	_LoadingScreen = (CLoadingScreen *)(&progressCallback);
	registerAndCallConfigCallback("DriverInstanceId");
}

CLoadingComponent::~CLoadingComponent()
{
	nlwarning("Unknown IComponent ~CLoadingComponent() call");
}

void CLoadingComponent::update()
{
	nlwarning("Unknown IComponent update() call");
}

void CLoadingComponent::render()
{
	nlwarning("Unknown IComponent render() call");
}

void CLoadingComponent::config(const string &varName, CConfigFile::CVar &var)
{
	if (varName == "DriverInstanceId")
	{
		unregisterNotifier(_DriverInstanceId);
		_DriverInstanceId = var.asString();
		registerNotifier(_DriverInstanceId);
	}
	else nlwarning("Unknown IConfigurableComponent config(varName, var) call");
}

void CLoadingComponent::componentUp(IComponent *component)
{
	std::string instanceId = component->getInstanceId();
	if (instanceId == _DriverInstanceId)
	{
		CDriverComponent *c = (CDriverComponent *)component;
		_LoadingScreen->setDriver(c->getDriver());
		_LoadingScreen->setTextContext(c->getTextContext());
	}
	else nlwarning("Unknown IComponent componentUp(component) call");
}

void CLoadingComponent::componentDown(IComponent *component)
{
	std::string instanceId = component->getInstanceId();
	if (instanceId == _DriverInstanceId)
	{
		_LoadingScreen->setDriver(NULL);
		_LoadingScreen->setTextContext(NULL);
	}
	else nlwarning("Unknown IComponent componentDown(component) call");
}

} /* namespace SBCLIENT */

/* end of file */
