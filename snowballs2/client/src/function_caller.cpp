/**
 * \file function_caller.cpp
 * \brief CFunctionCaller
 * \date 2008-01-09 00:44GMT
 * \author Jan Boon (Kaetemi)
 * CFunctionCaller
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
#include "function_caller.h"

#include <nel/misc/debug.h>

using namespace std;

namespace SBCLIENT {

CFunctionCaller::CFunctionCaller() : _LastId(0)
{
	_Current = _Functions.end();
}

CFunctionCaller::~CFunctionCaller()
{
	nlassert(_Current == _Functions.end());
	if (_Erase.size() > 0)
	{
		for (CFunctionWaitList::iterator w_it = _Erase.begin(); w_it != _Erase.end(); ++w_it)
			_Functions.erase(*w_it);
		_Erase.clear();
	}
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); ++it)
		nlwarning("Function with id '%u' was not properly removed", it->Id);
	for (CFunctionMap::iterator it = _Disabled.begin(); it != _Disabled.end(); ++it)
		nlwarning("Disabled function with id '%u' was not properly removed", it->first);
}

uint CFunctionCaller::add(TCallback function, void *context, void *tag, sint priority)
{
	// increase id and check if not 0 (would happen when going over limit)
	++_LastId; nlassert(_LastId);
	// add to the list, should be sorted by priority automatically
	_Functions.insert(CFunctionInfo(function, context, tag, _LastId, priority));
	// return the used unique id
	return _LastId;
}

void CFunctionCaller::remove(uint id)
{
	// find the one with the id and remove
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); ++it)
	{
		if (it->Id == id)
		{
			if (_Current == it) _Erase.push_back(it);
			else _Functions.erase(it);
			return;
		}
	}
	CFunctionMap::iterator d_it = _Disabled.find(id);
	if (d_it == _Disabled.end())
		nlerror("Function with id '%u' not found", id);
	_Disabled.erase(d_it);
}

void CFunctionCaller::execute()
{
	nlassert(_Current == _Functions.end());
	if (_Erase.size() > 0)
	{
		for (CFunctionWaitList::iterator w_it = _Erase.begin(); w_it != _Erase.end(); ++w_it)
			_Functions.erase(*w_it);
		_Erase.clear();
	}
	for (_Current = _Functions.begin(); _Current != _Functions.end(); ++_Current)
		_Current->Function(_Current->Context, _Current->Tag);
}

void CFunctionCaller::abort()
{
	_Current = _Functions.end();
}

void CFunctionCaller::enable(uint id, bool enable)
{
	if (enable) this->enable(id);
	else disable(id);
}

void CFunctionCaller::enable(uint id)
{
	CFunctionMap::iterator d_it = _Disabled.find(id);
	if (d_it == _Disabled.end())
	{
		for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); ++it)
			if (it->Id == id) return;
		nlerror("Function with id '%u' not found", id);
	}
	for (CFunctionWaitList::iterator w_it = _Erase.begin(); w_it != _Erase.end(); ++w_it)
	{
		if ((*w_it)->Id == id)
		{
			_Erase.erase(w_it);
			_Disabled.erase(d_it);
			return;
		}
	}
	CFunctionInfo info = d_it->second;
	_Disabled.erase(d_it);
	_Functions.insert(info);
}

void CFunctionCaller::disable(uint id)
{
	if (_Disabled.find(id) != _Disabled.end()) return;
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); ++it)
	{
		if (it->Id == id)
		{
			CFunctionInfo info = *it;
			if (_Current == it) _Erase.push_back(it);
			else _Functions.erase(it);
			_Disabled[id] = info;
			return;
		}
	}
	nlerror("Function with id '%u' not found", id);
}

bool CFunctionCaller::flip(uint id)
{
	if (_Disabled.find(id) == _Disabled.end()) 
	{ 
		disable(id);
		return false;
	}
	else 
	{
		enable(id);
		return true;
	}
}

// todo: flip, disable, and the special remove functions!
// also the destructor!

void CFunctionCaller::removeF(TCallback function, bool all)
{
	// find those with function and remove
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end();)
	{
		if (it->Function == function)
		{
			nlwarning("Removing function with id '%u'", it->Id);
			CFunctionInfos::iterator erase = it;
			++it;
			if (_Current == it) _Erase.push_back(it);
			else _Functions.erase(it);
			if (!all) return;
		}
		else ++it;
	}
	for (CFunctionMap::iterator it = _Disabled.begin(); it != _Disabled.end();)
	{
		if (it->second.Function == function)
		{
			nlwarning("Removing disabled function with id '%u'", it->first);
			CFunctionMap::iterator erase = it;
			++it;
			_Disabled.erase(it);
			if (!all) return;
		}
		else ++it;
	}
}

void CFunctionCaller::removeT(void *tag, bool all)
{
	// find those with function and remove
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end();)
	{
		if (it->Tag == tag)
		{
			nlwarning("Removing function with id '%u'", it->Id);
			CFunctionInfos::iterator erase = it;
			++it;
			if (_Current == it) _Erase.push_back(it);
			else _Functions.erase(it);
			if (!all) return;
		}
		else ++it;
	}
	for (CFunctionMap::iterator it = _Disabled.begin(); it != _Disabled.end();)
	{
		if (it->second.Tag == tag)
		{
			nlwarning("Removing disabled function with id '%u'", it->first);
			CFunctionMap::iterator erase = it;
			++it;
			_Disabled.erase(it);
			if (!all) return;
		}
		else ++it;
	}
}

void CFunctionCaller::removeC(void *context, bool all)
{
	// find those with function and remove
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end();)
	{
		if (it->Context == context)
		{
			nlwarning("Removing function with id '%u'", it->Id);
			CFunctionInfos::iterator erase = it;
			++it;
			if (_Current == it) _Erase.push_back(it);
			else _Functions.erase(it);
			if (!all) return;
		}
		else ++it;
	}
	for (CFunctionMap::iterator it = _Disabled.begin(); it != _Disabled.end();)
	{
		if (it->second.Context == context)
		{
			nlwarning("Removing disabled function with id '%u'", it->first);
			CFunctionMap::iterator erase = it;
			++it;
			_Disabled.erase(it);
			if (!all) return;
		}
		else ++it;
	}
}

void CFunctionCaller::removeA()
{
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); ++it)
	{
		nlwarning("Removing function with id '%u'", it->Id);
		if (_Current == it) _Erase.push_back(it);
		else _Functions.erase(it);
	}
	for (CFunctionMap::iterator it = _Disabled.begin(); it != _Disabled.end(); ++it)
		nlwarning("Removing disabled function with id '%u'", it->first);
	_Disabled.clear();
}

bool CFunctionCaller::test()
{
	CFunctionCaller caller;
	uint calls = 0;
	caller.add(test, &calls, (void *)4, -4);
	caller.add(test, &calls, (void *)0, 0);
	caller.add(test, &calls, (void *)6, -6);
	caller.add(test, &calls, (void *)3, -3);
	caller.add(test, &calls, (void *)1, -1);
	uint fake = caller.add(test, &calls, (void *)9, -6);
	caller.add(test, &calls, (void *)2, -2);
	caller.add(test, &calls, (void *)5, -5);
	caller.remove(fake);
	caller.execute();
	return calls == 7;
}

void CFunctionCaller::test(void *context, void *tag)
{
	uint &calls = *((uint *)context);
	uint call = (uint)tag; // works for small numbers
	if (calls != call) calls = -1; // sure to be wrong
	else ++calls;
}

} /* namespace SBCLIENT */

/* end of file */
