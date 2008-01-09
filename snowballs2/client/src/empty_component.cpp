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
	
}

CEmptyComponent::~CEmptyComponent()
{
	
}

void CEmptyComponent::update()
{
	
}

void CEmptyComponent::render()
{
	
}

void CEmptyComponent::config(const string &varName, CConfigFile::CVar &var)
{
	
}

void CEmptyComponent::componentUp(IComponent *component)
{

}

void CEmptyComponent::componentDown(IComponent *component)
{

}

} /* namespace SBCLIENT */

/* end of file */
