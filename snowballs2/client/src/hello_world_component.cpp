/** \file hello_world_component.cpp
 * CHelloWorldComponent
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
#include "hello_world_component.h"

#include "driver_component.h"

#include <nel/3d/u_text_context.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CHelloWorldComponent::CHelloWorldComponent(CComponentManager *manager, 
	const string &instanceId, IProgressCallback &progressCallback)
: IConfigurableComponent(manager, instanceId, progressCallback)
{
	_TextContext = ((CDriverComponent &)getInstance(_Config.getVar("DriverInstanceId").asString())).getTextContext();
	if (_Config.exists("Message")) registerAndCallConfigCallback("Message");
	else
	{
		_Message = i18nGet("HelloWorldNoConfig");
		registerConfigCallback("Message");
	}
}

CHelloWorldComponent::~CHelloWorldComponent()
{
	
}

void CHelloWorldComponent::update()
{
	
}

void CHelloWorldComponent::render()
{
	_TextContext->setHotSpot(UTextContext::TopLeft);
    _TextContext->setColor(CRGBA(255, 255, 255));
    _TextContext->setFontSize(18);
    _TextContext->printAt(0.01f, 0.99f, _Message);
}

void CHelloWorldComponent::config(const string &varName, CConfigFile::CVar &var)
{
	if (varName == "Message") _Message = i18n(var.asString());
	else nlwarning("Unknown IComponent config call");
}

}
