/**
 * \file key_binder.h
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

#ifndef SBCLIENT_KEY_BINDER_H
#define SBCLIENT_KEY_BINDER_H
#include <nel/misc/types_nl.h>

// Project includes
#include "member_callback_decl.h"
#include "member_callback_type.h"

// NeL includes
#include <nel/misc/events.h>

// STL includes
#include <map>
#include <vector>

namespace NL3D {
	class UDriver;
}

namespace SBCLIENT {
	class CInputListener;

/**
 * \brief CKeyBinder
 * \date 2008-02-09 21:27GMT
 * \author Jan Boon (Kaetemi)
 * CKeyBinder
 */
class CKeyBinder
{
public:
	// types
	enum TKeyState
	{
		Any,
		Down,
		Up
	};
protected:
	// types	
	struct _CKeyActionHandler // input listener event
	{
		/// A unique identifier for the registration
		uint RegistrationId;

		/// A unique identifier for the binding
		std::string BindingId;

		/// The function to call when this action is triggered
		TInterfaceCallback Callback;

		/// Usually the class that owns the functions
		void *Context;

		/// Any other user data
		void *Tag;
	};
	
	struct _CKeyStateHandler // driver is key down
	{
		/// A unique identifier for the registration
		uint RegistrationId;

		/// A unique identifier for the binding
		std::string BindingId;

		/// The bool to set every frame depending on the state of the key
		bool *KeyDown;
	};
	
	struct _CKeySetting // set by configuration or whatever
	{
		/// A unique identifier for the registration
		uint RegistrationId;

		/// A unique identifier for the binding
		std::string BindingId;

		/// The key
		NLMISC::TKey Key;

		/// Key states
		TKeyState Ctrl;
		TKeyState Shift;
		TKeyState Alt;

		/// Extra parameters for the action
		std::string Parameters;
	};
	
	typedef std::map<uint, _CKeyActionHandler> _CKeyActionHandlerMap;
	typedef std::map<uint, _CKeyStateHandler> _CKeyStateHandlerMap;
	typedef std::map<uint, _CKeySetting> _CKeySettingMap;
	
	struct _CKeyActionBinding // binds keyinfo to keyactionhandler
	{
		_CKeySetting KeySetting;
		_CKeyActionHandler ActionHandler;
	};
	typedef std::multimap<NLMISC::TKey, _CKeyActionBinding> _CKeyActionBindingMultiMap;
	
	struct _CKeyStateBinding // binds keyinfo to keystatehandler
	{
		_CKeySetting KeySetting;
		_CKeyStateHandler StateHandler;
	};
	typedef std::multimap<bool *, _CKeyStateBinding> _CKeyStateBindingMultiMap;
	
	// pointers
	CInputListener *_InputListener;
	NL3D::UDriver *_Driver;
	
	// instances
	/// Identifier of event in _InputListener
	uint _EventInputId;
	/// Holds all the registrations of _CKeyActionHandlers
	_CKeyActionHandlerMap _KeyActionHandlers;
	/// Holds all the registrations of _CKeyStateHandlers
	_CKeyStateHandlerMap _KeyStateHandlers;
	/// Holds all the registrations of _CKeySettings
	_CKeySettingMap _KeySettings;
	/// Holds bindings between settings and handlers, sorted by key
	_CKeyActionBindingMultiMap _KeyActionBindings;
	/// Holds bindings between settings and handlers, sorted by state handler bool pointer
	_CKeyStateBindingMultiMap _KeyStateBindings;
	/// The last assigned registration id, does not reset on init
	uint _LastId;
public:
	CKeyBinder();
	virtual ~CKeyBinder();

	void takeControl(NL3D::UDriver *driver, CInputListener *inputListener);
	void dropControl();
	
	void addActionHandler(uint &id, const std::string &bindingId, TInterfaceCallback callback, void *context, void *tag);
	void removeActionHandler(uint &id);
	
	void addStateHandler(uint &id, const std::string &bindingId, bool *keyDown);
	void removeStateHandler(uint &id);
	
	void addKeySetting(uint &id, const std::string &bindingId, const NLMISC::TKey &key, TKeyState ctrl, TKeyState shift, TKeyState alt, const std::string &parameters);
	void removeKeySetting(uint &id);
	
	/// Called when to check the driver for keys that are down
	SBCLIENT_CALLBACK_DECL(updateInput);
	
protected:
	/// Called by input listener
	SBCLIENT_CALLBACK_EVENT_DECL(eventInput);
	
	// add iskeydown handler (bool *) per frame X (b shift, b ctrl)
	// -- maybe also handle mouse (buttons) in this class?
	// add event key up down press handler (TEventCallback) X (b shift, b ctrl)
	// bind key to handler X
	// update with driver, check hasControl on input listener
	// add to an input listener
}; /* class CKeyBinder */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_KEY_BINDER_H */

/* end of file */
