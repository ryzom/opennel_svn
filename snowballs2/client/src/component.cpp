/** \file component.cpp
 * IComponent
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
#include "component.h"

#include "component_manager.h"

#include <nel/misc/app_context.h>
#include <nel/misc/i18n.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

std::string IComponent::I18NPrefix = "i18n";
bool IComponent::KeepPrefix = false;

IComponent::IComponent(CComponentManager *manager, const string &instanceId, IProgressCallback &progressCallback)
: _Manager(manager), _InstanceId(instanceId)
{
	// register this with the app context
	// this ensures that the same component configuration isn't init twice
	CApplicationContext::getInstance().setSingletonPointer(
		string("SBCLIENT::IComponent|") + _InstanceId, this);
}

IComponent::~IComponent()
{
	// remove this from the app context
	CApplicationContext::getInstance().releaseSingletonPointer(
		string("SBCLIENT::IComponent|") + _InstanceId, this);
}

IComponent &IComponent::getInstance(const string &instanceId)
{
	IComponent *instance = getInstancePtr(instanceId);
	if (instance == NULL) 
		nlerror("IComponent %s does not exist", instanceId.c_str());
	return *instance;
}

IComponent *IComponent::getInstancePtr(const string &instanceId)
{
	// make use of the nlmisc application context
	IComponent *instance = 
		(IComponent *)CApplicationContext::getInstance()
		.getSingletonPointer(string("SBCLIENT::IComponent|") + instanceId);
	return instance;
}

ucstring IComponent::i18n(const string &label)
{
	if (label.substr(0, 4) == I18NPrefix)
	{
		if (KeepPrefix) return CI18N::get(label);
		return CI18N::get(label.substr(4, label.size() - 4));
	}
	return ucstring(label);
}

ucstring IComponent::i18nGet(const string &label)
{
	return CI18N::get(label);
}

void IComponent::registerNotifier(const std::string &target)
{
	_Manager->registerNotifier(this, target);
}

void IComponent::unregisterNotifier(const std::string &target)
{
	_Manager->unregisterNotifier(this, target);
}

}
