/**
 * \file input_listener.h
 * \brief CInputListener
 * \date 2008-02-09 16:49GMT
 * \author Jan Boon (Kaetemi)
 * Allows different classes to take over input from each other.
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

#ifndef SBCLIENT_INPUT_LISTENER_H
#define SBCLIENT_INPUT_LISTENER_H
#include <nel/misc/types_nl.h>

// Project includes
#include "member_callback_type.h"
#include "member_callback_decl.h"

// NeL includes
#include <nel/misc/event_listener.h>

// STL includes
#include <map>

namespace SBCLIENT {

/**
 * \brief CInputListener
 * \date 2008-02-09 16:49GMT
 * \author Jan Boon (Kaetemi)
 * CInputListener
 */
class CInputListener : public NLMISC::IEventListener
{
protected:
	// types
	struct _CInputCallback
	{
		_CInputCallback() : Callback(NULL) { }
		_CInputCallback(TEventCallback callback, void *context, void *tag)
			: Callback(callback), Context(context), Tag(tag) { }
		TEventCallback Callback;
		void *Context;
		void *Tag;
	};
	typedef std::map<uint, _CInputCallback> _CInputCallbackMap;

	// instances
	uint _LastId;
	_CInputCallbackMap _Callbacks;
	uint _CurrentId;
	_CInputCallback _Current;
public:
	CInputListener();
	virtual ~CInputListener();

	void operator () (const NLMISC::CEvent &ev);
	SBCLIENT_CALLBACK_EVENT_DECL(eventInput);
	
	/// Take control of input, id must be 0 (asserted), if cb is NULL nothing receives input
	void takeControl(uint &id, TEventCallback callback, void *context, void *tag);
	/// Drop control of input, id cannot be 0 (asserted)
	void dropControl(uint &id);

	/// Check if you have control, returns false if id is 0
	inline bool hasControl(uint id) const { return _CurrentId == id; }

	/// Changes the callback parameters, id cannot be 0 (asserted), id must exist (asserted)
	void changeControl(uint id, TEventCallback callback, void *context, void *tag);
}; /* class CInputListener */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_INPUT_LISTENER_H */

/* end of file */
