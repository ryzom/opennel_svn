/**
 * \file key_binder.cpp
 * \brief CKeyBinder
 * \date 2008-02-09 21:27GMT
 * \author Jan Boon (Kaetemi)
 * CKeyBinder
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
#include "key_binder.h"

// Project includes
#include "input_listener.h"
#include "member_callback_impl.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CKeyBinder::CKeyBinder() : _InputListener(NULL), _EventInputId(0), 
_Driver(NULL), _LastId(0)
{
	
}

CKeyBinder::~CKeyBinder()
{
	
}

void CKeyBinder::takeControl(UDriver *driver, CInputListener *inputListener)
{
	nlassert(!_Driver);
	_Driver = driver;
	nlassert(_Driver);

	nlassert(!_InputListener);
	_InputListener = inputListener;
	nlassert(_InputListener);
	_InputListener->takeControl(_EventInputId, eventInput, this, NULL);
}

void CKeyBinder::dropControl()
{
	nlassert(_InputListener);
	_InputListener->dropControl(_EventInputId);
	_InputListener = NULL;
	_Driver = NULL;
}

void CKeyBinder::addActionHandler(uint &id, const std::string &bindingId, TInterfaceCallback callback, void *context, void *tag, const std::string &parameters)
{
	nlassert(!id);
	id = ++_LastId;
	nlassert(id);

	// create structure
	_CKeyActionHandler handler;
	handler.RegistrationId = id;
	handler.BindingId = bindingId;
	handler.Callback = callback;
	handler.Context = context;
	handler.Tag = tag;
	handler.Parameters = parameters;

	// insert registration
	_KeyActionHandlers[id] = handler;

	// create bindings
	{
		_CKeySettingMap::iterator
			it(_KeySettings.begin()), 
			end(_KeySettings.end());
		for (; it != end; ++it)
		{
			if (it->second.BindingId == bindingId)
			{
				_CKeyActionBinding binding;
				binding.ActionHandler = handler;
				binding.KeySetting = it->second;
				nldebug("KEYB: new action, binding '%s' to key '%s'", 
					bindingId.c_str(), 
					CEventKey::getStringFromKey(binding.KeySetting.Key).c_str());
				_KeyActionBindings.insert(make_pair(
					binding.KeySetting.Key, 
					binding));
			}
		}
	}
}

void CKeyBinder::removeActionHandler(uint &id)
{
	nlassert(id);
	
	// erase from registration
	{
		_CKeyActionHandlerMap::iterator
			it(_KeyActionHandlers.find(id));
		nlassert(it != _KeyActionHandlers.end());	
		_KeyActionHandlers.erase(it);
	}
	
	// erase all bindings
	{
		_CKeyActionBindingMultiMap::iterator
			it(_KeyActionBindings.begin()), 
			next(it), 
			end(_KeyActionBindings.end());
		while (it != end)
		{
			++next;
			if (it->second.ActionHandler.RegistrationId == id)
				_KeyActionBindings.erase(it);
			it = next;
		}
	}
	
	id = 0;
}

void CKeyBinder::addStateHandler(uint &id, const std::string &bindingId, bool *keyDown)
{
	nlassert(!id);
	id = ++_LastId;
	nlassert(id);
	
	// ...
}

void CKeyBinder::removeStateHandler(uint &id)
{
	nlassert(id);

	// erase from registration
	{
		_CKeyStateHandlerMap::iterator
			it(_KeyStateHandlers.find(id));
		nlassert(it != _KeyStateHandlers.end());	
		_KeyStateHandlers.erase(it);
	}
	
	// erase all bindings
	{
		_CKeyStateBindingVector::iterator
			it(_KeyStateBindings.begin()), 
			next(it), 
			end(_KeyStateBindings.end());
		while (it != end)
		{
			++next;
			if (it->StateHandler.RegistrationId == id)
				_KeyStateBindings.erase(it);
			it = next;
		}
	}

	id = 0;
}

//CEventKey::getKeyFromString(const std::string &str);
void CKeyBinder::addKeySetting(uint &id, const std::string &bindingId, NLMISC::TKey &key, TKeyState ctrl, TKeyState shift, TKeyState alt)
{
	nlassert(!id);
	id = ++_LastId;
	nlassert(id);
	
	// ...
}

void CKeyBinder::removeKeySetting(uint &id)
{
	nlassert(id);

	// erase from registration
	{
		_CKeySettingMap::iterator
			it(_KeySettings.find(id));
		nlassert(it != _KeySettings.end());	
		_KeySettings.erase(it);
	}
	
	// erase all bindings to actions
	{
		_CKeyActionBindingMultiMap::iterator
			it(_KeyActionBindings.begin()), 
			next(it), 
			end(_KeyActionBindings.end());
		while (it != end)
		{
			++next;
			if (it->second.KeySetting.RegistrationId == id)
				_KeyActionBindings.erase(it);
			it = next;
		}
	}

	// erase all bindings to states
	{
		_CKeyStateBindingVector::iterator
			it(_KeyStateBindings.begin()), 
			next(it), 
			end(_KeyStateBindings.end());
		while (it != end)
		{
			++next;
			if (it->KeySetting.RegistrationId == id)
				_KeyStateBindings.erase(it);
			it = next;
		}
	}

	id = 0;
}

SBCLIENT_CALLBACK_EVENT_IMPL(CKeyBinder, eventInput)
{
	if (!_Driver || !_InputListener) return false;

	// ev

	return true;
}

SBCLIENT_CALLBACK_IMPL(CKeyBinder, updateInput)
{
	if (!_Driver || !_InputListener) return;

	//driver->AsyncListener.isKeyDown
	
	//_Driver->AsyncListener.isk
}

} /* namespace SBCLIENT */

/* end of file */
