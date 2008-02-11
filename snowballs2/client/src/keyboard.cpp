/**
 * \file keyboard.cpp
 * \brief CKeyboard
 * \date 2008-02-07 18:02GMT
 * \author Jan Boon (Kaetemi)
 * CKeyboard
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
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
#include "keyboard.h"

// Project includes
#include "member_callback_impl.h"

// NeL includes
#include <nel/misc/debug.h>
#include <nel/misc/progress_callback.h>
#include <nel/misc/command.h>
#include <nel/misc/app_context.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CKeyboard::CKeyboard(NLMISC::IProgressCallback &progress, 
const std::string &id, NL3D::UDriver *driver, CInputListener *inputListener) : 
_Driver(driver), _InputListener(inputListener), _Config(id),
_ActionCommandId(0)
{
	progress.progress(0.00f);
	// verify external pointers
	nlassert(_Driver);
	nlassert(_InputListener);

	// add special key actions handlers
	KeyBinder.addActionHandler(_ActionCommandId, 
		_Config.getValue("CommandAction", string("command")), 
		CKeyboard::actionCommand, this, NULL);

	// read configured keys
	_Config.setCallbackAndCall("KeySettings", configKeySettings, this, NULL);

	progress.progress(1.00f);
}

CKeyboard::~CKeyboard()
{
	// release configured keys
	releaseKeys();

	// release special key actions handlers
	KeyBinder.removeActionHandler(_ActionCommandId);
}

void CKeyboard::enable()
{
	// key binder takes control over input
	KeyBinder.takeControl(_Driver, _InputListener);	// move to Enable thingy
}

void CKeyboard::disable()
{
	// key binder drops control over input
	KeyBinder.dropControl();
}

SBCLIENT_CALLBACK_INTERFACE_IMPL(CKeyboard, actionCommand)
{
	ICommand::execute(params, *INelContext::getInstance().getInfoLog());
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CKeyboard, configKeySettings)
{
	if (!_KeyIds.empty()) releaseKeys();
	uint var_size = var.size();
	for (uint i = 0; i < var_size; i += 4)
	{
		vector<string> keys;
		explode(var.asString(i + 3), string("|"), keys, true);
		uint keys_size = keys.size();
		for (uint j = 0; j < keys_size; ++j)
		{
			uint key_id = 0;
			KeyBinder.addKeySetting(
				key_id, 
				var.asString(i + 0), 
				CEventKey::getKeyFromString(keys[j]), 
				CKeyBinder::Any, 
				CKeyBinder::Any, 
				CKeyBinder::Any, 
				var.asString(i + 1));
		}
	}
}

void CKeyboard::releaseKeys()
{
	std::vector<uint>::iterator
		it(_KeyIds.begin()),
		end(_KeyIds.end());
	for (; it != end; ++it)
	{
		KeyBinder.removeKeySetting(*it);
	}
}

} /* namespace SBCLIENT */

/* end of file */
