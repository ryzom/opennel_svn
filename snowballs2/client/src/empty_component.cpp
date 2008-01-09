/**
 * \file empty_component.cpp
 * \brief CEmptyComponent
 * \date 2008-01-09 14:27GMT
 * \author Author (Username)
 * CEmptyComponent
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Author (Username)
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
#include "empty_component.h"

// #include <nel/misc/debug.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CEmptyComponent::CEmptyComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
	: IConfigurableComponent(manager, instanceId, progressCallback)
{
	nlwarning("Unknown IComponent CEmptyComponent() call");
}

CEmptyComponent::~CEmptyComponent()
{
	nlwarning("Unknown IComponent ~CEmptyComponent() call");
}

void CEmptyComponent::update()
{
	nlwarning("Unknown IComponent update() call");
}

void CEmptyComponent::render()
{
	nlwarning("Unknown IComponent render() call");
}

void CEmptyComponent::config(const string &varName, CConfigFile::CVar &var)
{
	nlwarning("Unknown IConfigurableComponent config(varName, var) call");
}

void CEmptyComponent::componentUp(IComponent *component)
{
	nlwarning("Unknown IComponent componentUp(component) call");
}

void CEmptyComponent::componentDown(IComponent *component)
{
	nlwarning("Unknown IComponent componentDown(component) call");
}

} /* namespace SBCLIENT */

/* end of file */
