/**
 * \file input_listener.cpp
 * \brief CInputListener
 * \date 2008-02-09 16:49GMT
 * \author Jan Boon (Kaetemi)
 * CInputListener
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
#include "input_listener.h"

// Project includes
#include "member_callback_impl.h"

// NeL includes
#include <nel/misc/debug.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CInputListener::CInputListener() : _LastId(0), _CurrentId(0)
{
	
}

CInputListener::~CInputListener()
{
	// some simple warnings to make sure that the class is correctly used
	_CInputCallbackMap::iterator 
		it(_Callbacks.begin()), end(_Callbacks.end());
	for (; it != end; ++it)
		nlwarning("Input callback with id '%u' was not properly removed", it->first);
	if (_CurrentId)
		nlwarning("Current input callback with id '%u' was not properly removed", _CurrentId);
}

void CInputListener::operator () (const CEvent &ev)
{
	eventInput(ev, NULL);
}

SBCLIENT_CALLBACK_EVENT_IMPL(CInputListener, eventInput)
{
	if (_CurrentId == 0) return false; // not handled
	if (_Current.Callback) // else return true, NULL is input blocking
	{
		if (!_Current.Callback(_Current.Context, ev, _Current.Tag))
		{
			_CInputCallbackMap::reverse_iterator
				rit(_Callbacks.rbegin()), rend(_Callbacks.rend());
			for (; rit != rend; ++rit)
			{
				if (!rit->second.Callback) return true; // blocking input
				if (rit->second.Callback(rit->second.Callback, ev, rit->second.Tag))
					return true;
			}
			return false; // not handled by any callback
		}
	}
	return true;
}

void CInputListener::takeControl(uint &id, TEventCallback callback, void *context, void *tag)
{
	nlassert(!id);
	id = ++_LastId;
	nlassert(id);
	if (_CurrentId)
	{
		nlassert(_Callbacks.find(_CurrentId) == _Callbacks.end());
		_Callbacks[_CurrentId] = _Current;
	}
	_CurrentId = id;
	_Current.Callback = callback;
	_Current.Context = context;
	_Current.Tag = tag;
}

void CInputListener::dropControl(uint &id)
{
	nlassert(id);
	if (id == _CurrentId)
	{
		if (_Callbacks.empty())
		{
			_CurrentId = 0;
			_Current.Callback = NULL;
		}
		else
		{
			_CInputCallbackMap::iterator 
				it(_Callbacks.end());
			--it;
			_Current = it->second;
			_Callbacks.erase(it);
		}
	}
	else
	{
		_CInputCallbackMap::iterator it(_Callbacks.find(id));
		nlassert(it != _Callbacks.end());
		_Callbacks.erase(it);
	}
	id = 0;
}

void CInputListener::changeControl(uint id, TEventCallback callback, void *context, void *tag)
{
	nlassert(id);
	if (_CurrentId == id)
	{
		_Current.Callback = callback;
		_Current.Context = context;
		_Current.Tag = tag;
	}
	else
	{
		_CInputCallbackMap::iterator it(_Callbacks.find(id));
		nlassert(it != _Callbacks.end());
		it->second.Callback = callback;
		it->second.Context = context;
		it->second.Tag = tag;
	}
}

} /* namespace SBCLIENT */

/* end of file */
