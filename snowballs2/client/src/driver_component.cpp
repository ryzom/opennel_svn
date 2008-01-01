/** \file driver_component.cpp
 * CDriverComponent
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * $Id$
 */

#include <nel/misc/types_nl.h>
#include "driver_component.h"

#include <nel/3d/u_driver.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CDriverComponent::CDriverComponent(
	CComponentManager *manager, const string &instanceId)
: IComponent(manager, instanceId)
{
	
}

CDriverComponent::~CDriverComponent()
{
	
}

void CDriverComponent::update()
{
	
}

void CDriverComponent::render()
{
	
}

void CDriverComponent::config(const string &varName, CConfigFile::CVar &var)
{
	
}

void CDriverComponent::setWindowTitle(const string &title)
{
#ifdef NL_OS_WINDOWS
	SetWindowText((HWND)_Driver->getDisplay(), title.c_str());
#endif
}

}
