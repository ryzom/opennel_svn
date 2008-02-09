/**
 * \file login_data.cpp
 * \brief CLoginData
 * \date 2008-02-09 17:15GMT
 * \author Jan Boon (Kaetemi)
 * CLoginData
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
#include "login_data.h"

// #include <nel/misc/debug.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CLoginData::CLoginData(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
	: IConfigurableComponent(manager, instanceId, progressCallback)
{
	nlwarning("Unknown IComponent CLoginData() call");
}

CLoginData::~CLoginData()
{
	nlwarning("Unknown IComponent ~CLoginData() call");
}

void CLoginData::update()
{
	nlwarning("Unknown IComponent update() call");
}

void CLoginData::render()
{
	nlwarning("Unknown IComponent render() call");
}

void CLoginData::config(const string &varName, CConfigFile::CVar &var)
{
	nlwarning("Unknown IConfigurableComponent config(varName, var) call");
}

void CLoginData::componentUp(IComponent *component)
{
	nlwarning("Unknown IComponent componentUp(component) call");
}

void CLoginData::componentDown(IComponent *component)
{
	nlwarning("Unknown IComponent componentDown(component) call");
}

} /* namespace SBCLIENT */

/* end of file */
