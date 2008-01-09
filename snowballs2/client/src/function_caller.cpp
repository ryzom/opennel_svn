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
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
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
	
}

CFunctionCaller::~CFunctionCaller()
{
	
}

uint CFunctionCaller::add(void (*function)(void *context, void *tag), void *context, void *tag, sint priority)
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
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); it++)
	{
		if (it->Id == id)
		{
			_Functions.erase(it);
			return;
		}
	}
	nlerror("Function with id '%u' not found", id);
}

void CFunctionCaller::execute()
{
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); it++)
		it->Function(it->Context, it->Tag);
}

void CFunctionCaller::removeF(void (*function)(void *context, void *tag), bool all)
{
	// find those with function and remove
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end();)
	{
		if (it->Function == function)
		{
			nlwarning("Removing function with id '%u'", it->Id);
			CFunctionInfos::iterator erase = it;
			it++;
			_Functions.erase(erase);
			if (!all) return;
		}
		else it++;
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
			it++;
			_Functions.erase(erase);
			if (!all) return;
		}
		else it++;
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
			it++;
			_Functions.erase(erase);
			if (!all) return;
		}
		else it++;
	}
}

void CFunctionCaller::removeA()
{
	for (CFunctionInfos::iterator it = _Functions.begin(); it != _Functions.end(); it++)
		nlwarning("Removing function with id '%u'", it->Id);
	_Functions.clear();
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
