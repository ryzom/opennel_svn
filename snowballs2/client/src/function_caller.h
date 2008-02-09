/**
 * \file function_caller.h
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

#ifndef SBCLIENT_FUNCTION_CALLER_H
#define SBCLIENT_FUNCTION_CALLER_H
#include <nel/misc/types_nl.h>

#include "member_callback_type.h"

#include <set>
#include <map>
#include <vector>

namespace SBCLIENT {

/**
 * \brief CFunctionCaller (slave)
 * \date 2008-01-09 00:44GMT
 * \author Jan Boon (Kaetemi)
 * CFunctionCaller is used to call functions in order of priority.
 */
class CFunctionCaller
{
	/** Structure containing information about a callable function */
	struct CFunctionInfo
	{
		CFunctionInfo() { }
		CFunctionInfo(TCallback function, void *context, void *tag, uint id, sint priority)
			: Function(function), Context(context), Tag(tag), Id(id), Priority(priority) { }
		/** The function to be called */
		TCallback Function;
		/** Usually an instance of a class */
		void *Context;
		/** Anything that needs to tag along */
		void *Tag;
		/** Unique identifier */
		uint Id;
		/** Priority of this thing */
		sint Priority;
	};
	struct CFunctionInfoOrder { bool operator()( 
		const CFunctionInfo& _Left, const CFunctionInfo& _Right) const {
		return (_Left.Priority > _Right.Priority); } // higher first
	};
	typedef std::multiset<CFunctionInfo, CFunctionInfoOrder> CFunctionInfos;
	typedef std::map<uint, CFunctionInfo> CFunctionMap;
	typedef std::vector<CFunctionInfos::iterator> CFunctionWaitList;
protected:
	// pointers
	// ...
	
	// instances
	CFunctionInfos _Functions;
	CFunctionMap _Disabled;
	CFunctionWaitList _Erase;
	uint _LastId;
	CFunctionInfos::iterator _Current;
public:
	CFunctionCaller();
	virtual ~CFunctionCaller();

	/** 
		Adds a function. Higher priority called first.
		Can only be called 2^sizeof(uint) times.
		Returns a unique id that should be used to remove the function.
		Added functions are enabled by default.
	*/
	uint add(TCallback function, void *context, void *tag, sint priority);
	/** Removes the function with 'id' from list. */
	void remove(uint id);
	/** Calls all functions */
	void execute();
	// NOTE: Function that can filter the called functions between
	// two given priorities might be useful at some point.
	// void execute(sint fromPriority, sint toPriority);
	/** Stops execution */
	void abort();

	/** Enable or disable a function, does nothing if no state change */
	void enable(uint id, bool enable);
	/** Enable a function, does nothing if already enabled */
	void enable(uint id);
	/** Switch a function between enabled and disabled */
	bool flip(uint id);
	/** Disable a function, does nothing if already disabled */
	void disable(uint id);

	// NOTE: The following should only be used during development.
	// These functions will give warnings when they actually do something.
	/** Removes 'function's from the list (only one if 'all' false). */
	void removeF(TCallback function, bool all);
	/** Removes functions with 'tag' from the list (only one if 'all' false). */
	void removeT(void *tag, bool all);
	/** Removes functions in 'context' from the list (only one if 'all' false). */
	void removeC(void *context, bool all);
	/** Removes everything */
	void removeA();
	// ---

	// NOTE: You can use tag to 'group' different functions, and easily
	// remove the whole bunch of them using removeT.

	// NOTE: Some things to see if it works correctly.
	static bool test();
	static void test(void *context, void *tag);
}; /* class CFunctionCaller */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_FUNCTION_CALLER_H */

/* end of file */
