/**
 * \file string_identifier.h
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

#ifndef SBCLIENT_STRING_IDENTIFIER_H
#define SBCLIENT_STRING_IDENTIFIER_H
#include <nel/misc/types_nl.h>

#include "evil_singleton_decl.h"

#include <map>

namespace SBCLIENT {

/**
 * \brief CStringIdentifier
 * \date 2008-02-03 15:53GMT
 * \author Jan Boon (Kaetemi)
 * Maps unique strings to unique uints.
 */
class CStringIdentifier
{
protected:
	// pointers
	// ...
	
	// instances
	std::map<std::string, uint> _Identifiers;
	uint _LastId;
public:
	CStringIdentifier();
	virtual ~CStringIdentifier();

	static uint get(const std::string &id);

	SBCLIENT_EVIL_SINGLETON_DECL(SBCLIENT::CStringIdentifier);
}; /* class CStringIdentifier */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_STRING_IDENTIFIER_H */

/* end of file */
