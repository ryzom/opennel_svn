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
	nlassert(!_InputListener);
	nlassert(!_Driver);
	{
		_CKeyActionHandlerMap::iterator
			it(_KeyActionHandlers.begin()),
			end(_KeyActionHandlers.end());
		for (; it != end; ++it)
			nlwarning("KEYB: key action handler not released, killing binding '%s'", it->second.BindingId.c_str());
	}
	{
		_CKeyStateHandlerMap::iterator
			it(_KeyStateHandlers.begin()),
			end(_KeyStateHandlers.end());
		for (; it != end; ++it)
			nlwarning("KEYB: key state handler not released, killing binding '%s'", it->second.BindingId.c_str());
	}
	{
		_CKeySettingMap::iterator
			it(_KeySettings.begin()),
			end(_KeySettings.end());
		for (; it != end; ++it)
			nlwarning("KEYB: key setting not released, killing binding '%s'", it->second.BindingId.c_str());
	}
	{
		_CKeyActionBindingMultiMap::iterator
			it(_KeyActionBindings.begin()),
			end(_KeyActionBindings.end());
		for (; it != end; ++it)
			nlwarning("KEYB: key action binding not released, killing binding '%s'", it->second.KeySetting.BindingId.c_str());
	}
	{
		_CKeyStateBindingMultiMap::iterator
			it(_KeyStateBindings.begin()),
			end(_KeyStateBindings.end());
		for (; it != end; ++it)
			nlwarning("KEYB: key state binding not released, killing binding '%s'", it->second.KeySetting.BindingId.c_str());
	}
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

void CKeyBinder::addActionHandler(uint &id, const std::string &bindingId, TInterfaceCallback callback, void *context, void *tag)
{
	nlassert(!id);
	id = ++_LastId;
	nlassert(id);
	nlassert(callback);

	// create structure
	_CKeyActionHandler handler;
	handler.RegistrationId = id;
	handler.BindingId = bindingId;
	handler.Callback = callback;
	handler.Context = context;
	handler.Tag = tag;

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
				binding.KeySetting = it->second;
				binding.ActionHandler = handler;
				nldebug("KEYB: new action handler, binding '%s' to key '%s'", 
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
	nlassert(keyDown);
	
	// create structure
	_CKeyStateHandler handler;
	handler.RegistrationId = id;
	handler.BindingId = bindingId;
	handler.KeyDown = keyDown;

	// insert registration
	_KeyStateHandlers[id] = handler;

	// create bindings
	{
		_CKeySettingMap::iterator
			it(_KeySettings.begin()), 
			end(_KeySettings.end());
		for (; it != end; ++it)
		{
			if (it->second.BindingId == bindingId)
			{
				_CKeyStateBinding binding;
				binding.KeySetting = it->second;
				binding.StateHandler = handler;
				nldebug("KEYB: new state handler, binding '%s' to key '%s'", 
					bindingId.c_str(), 
					CEventKey::getStringFromKey(binding.KeySetting.Key).c_str());
				_KeyStateBindings.insert(make_pair(
					handler.KeyDown, // pointer to bool
					binding));
			}
		}
	}
}

void CKeyBinder::removeStateHandler(uint &id)
{
	nlassert(id);

	// erase from registration
	{
		_CKeyStateHandlerMap::iterator
			it(_KeyStateHandlers.find(id));
		nlassert(it != _KeyStateHandlers.end());
		*(it->second.KeyDown) = false;
		_KeyStateHandlers.erase(it);
	}
	
	// erase all bindings
	{
		_CKeyStateBindingMultiMap::iterator
			it(_KeyStateBindings.begin()), 
			next(it), 
			end(_KeyStateBindings.end());
		while (it != end)
		{
			++next;
			if (it->second.StateHandler.RegistrationId == id)
				_KeyStateBindings.erase(it);
			it = next;
		}
	}

	id = 0;
}

//CEventKey::getKeyFromString(const std::string &str);
void CKeyBinder::addKeySetting(uint &id, const std::string &bindingId, const NLMISC::TKey &key, TKeyState ctrl, TKeyState shift, TKeyState alt, const std::string &parameters)
{
	nlassert(!id);
	id = ++_LastId;
	nlassert(id);
		
	// create structure
	_CKeySetting setting;
	setting.RegistrationId = id;
	setting.BindingId = bindingId;
	setting.Key = key;
	setting.Ctrl = ctrl;
	setting.Shift = shift;
	setting.Alt = alt;
	setting.Parameters = parameters;

	// insert registration
	_KeySettings[id] = setting;

	// create bindings
	{
		_CKeyActionHandlerMap::iterator
			it(_KeyActionHandlers.begin()), 
			end(_KeyActionHandlers.end());
		for (; it != end; ++it)
		{
			if (it->second.BindingId == bindingId)
			{
				_CKeyActionBinding binding;
				binding.KeySetting = setting;
				binding.ActionHandler = it->second;
				nldebug("KEYB: new key setting '%s', binding '%s' to action", 
					bindingId.c_str(), 
					CEventKey::getStringFromKey(key).c_str());
				_KeyActionBindings.insert(make_pair(
					key, 
					binding));
			}
		}
	}
	{
		_CKeyStateHandlerMap::iterator
			it(_KeyStateHandlers.begin()), 
			end(_KeyStateHandlers.end());
		for (; it != end; ++it)
		{
			if (it->second.BindingId == bindingId)
			{
				_CKeyStateBinding binding;
				binding.KeySetting = setting;
				binding.StateHandler = it->second;
				nldebug("KEYB: new key setting '%s', binding '%s' to state", 
					bindingId.c_str(), 
					CEventKey::getStringFromKey(key).c_str());
				_KeyStateBindings.insert(make_pair(
					binding.StateHandler.KeyDown, // pointer to bool
					binding));
			}
		}
	}
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
		_CKeyStateBindingMultiMap::iterator
			it(_KeyStateBindings.begin()), 
			next(it), 
			end(_KeyStateBindings.end());
		while (it != end)
		{
			++next;
			*(it->second.StateHandler.KeyDown) = false;
			if (it->second.KeySetting.RegistrationId == id)
				_KeyStateBindings.erase(it);
			it = next;
		}
	}

	id = 0;
}

SBCLIENT_CALLBACK_EVENT_IMPL(CKeyBinder, eventInput)
{
	if (!_Driver || !_InputListener) return false;
	nlassert(_InputListener->hasControl(_EventInputId));

	// note: EventCharId isn't handled here,
	// you must take control trough the CInputListener
	// if you need to grab those events (used for textboxes).
	// in such implementation you must handle all three
	// key events, since keychar is probly accompanied
	// by a keydown and a keyup event
	TKey key;
	if (ev == EventKeyDownId) key = ((CEventKeyDown &)ev).Key;
	else if (ev == EventKeyUpId) key = ((CEventKeyUp &)ev).Key;
	else if (ev == EventCharId) return false;
	else { nlwarning("Invalid event sent to key binder"); return false; }
	// get ctrl, shift, alt states
	TKeyButton keyButton = ((CEventKey &)ev).Button;

	// callback implementations must check (ev == EventUpId) themselves,
	// depending on how they handle the different key events
	// this way you can have a hold/release team list key configured
	_CKeyActionBindingMultiMap::iterator
		it(_KeyActionBindings.lower_bound(key)),
		end(_KeyActionBindings.upper_bound(key));
	for (; it != end; ++it)
	{
		_CKeySetting &ks = it->second.KeySetting;
		switch (ks.Ctrl)
		{
		case Down: if (!(keyButton & ctrlKeyButton)) { continue; } break;
		case Up: if (keyButton & ctrlKeyButton) { continue; } break;
		}
		switch (ks.Shift)
		{
		case Down: if (!(keyButton & shiftKeyButton)) { continue; } break;
		case Up: if (keyButton & shiftKeyButton) { continue; } break;
		}
		switch (ks.Alt)
		{
		case Down: if (!(keyButton & altKeyButton)) { continue; } break;
		case Up: if (keyButton & altKeyButton) { continue; } break;
		}
		_CKeyActionHandler &ah = it->second.ActionHandler;
		ah.Callback(ah.Context, ks.Parameters, &ev, ah.Tag);
	}
	
	return true;
}

SBCLIENT_CALLBACK_IMPL(CKeyBinder, updateInput)
{
	if (!_Driver || !_InputListener) return;
	if (!_InputListener->hasControl(_EventInputId)) return;

	// store the last one that was down, 
	// to make sure we don't set it back to false
	bool *lastDown = NULL;
	_CKeyStateBindingMultiMap::iterator
		it(_KeyStateBindings.begin()),
		end(_KeyStateBindings.begin());
	for (; it != end; ++it)
	{
		if (_Driver->AsyncListener.isKeyDown(it->second.KeySetting.Key))
		{
			switch (it->second.KeySetting.Ctrl)
			{
			case Down: if (!_Driver->AsyncListener.isKeyDown(KeyCONTROL)) { goto KeyFalse; } break;
			case Up: if (_Driver->AsyncListener.isKeyDown(KeyCONTROL)) { goto KeyFalse; } break;
			}
			switch (it->second.KeySetting.Shift)
			{
			case Down: if (!_Driver->AsyncListener.isKeyDown(KeySHIFT)) { goto KeyFalse; } break;
			case Up: if (_Driver->AsyncListener.isKeyDown(KeySHIFT)) { goto KeyFalse; } break;
			}
			switch (it->second.KeySetting.Alt)
			{
			case Down: if (!_Driver->AsyncListener.isKeyDown(KeyMENU)) { goto KeyFalse; } break;
			case Up: if (_Driver->AsyncListener.isKeyDown(KeyMENU)) { goto KeyFalse; } break;
			}
			*(it->second.StateHandler.KeyDown) = true;
			lastDown = it->second.StateHandler.KeyDown;
			goto KeyOk;
		}
KeyFalse:
		if (lastDown != it->second.StateHandler.KeyDown) 
		{
			// key not down (or ctrl shift alt not ok)
			*(it->second.StateHandler.KeyDown) = false;
		}
KeyOk:;
	}
}

} /* namespace SBCLIENT */

/* end of file */
