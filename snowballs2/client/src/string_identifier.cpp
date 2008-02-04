/**
 * \file string_identifier.cpp
 * \brief CStringIdentifier
 * \date 2008-02-03 15:53GMT
 * \author Jan Boon (Kaetemi)
 * CStringIdentifier
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
#include "string_identifier.h"

#include "evil_singleton_impl.h"

#include <nel/misc/debug.h>

using namespace std;
// using namespace NLMISC;

namespace SBCLIENT {

CStringIdentifier::CStringIdentifier() : _LastId(0)
{
	SBCLIENT_EVIL_SINGLETON_CONSTRUCTOR(SBCLIENT::CStringIdentifier);
	_Identifiers[""] = 0;
}

CStringIdentifier::~CStringIdentifier()
{
	SBCLIENT_EVIL_SINGLETON_DESTRUCTOR(SBCLIENT::CStringIdentifier);	
}

uint CStringIdentifier::get(const std::string &id)
{
	CStringIdentifier &me = getInstance();
	std::map<string, uint>::iterator it = me._Identifiers.find(id);
	if (it == me._Identifiers.end())
	{
		++me._LastId;
		me._Identifiers[id] = me._LastId;
		return me._LastId;
	}
	return it->second;
}

SBCLIENT_EVIL_SINGLETON_IMPL(SBCLIENT::CStringIdentifier);

} /* namespace SBCLIENT */

/* end of file */
