/**
 * \file login.h
 * \brief MLogin
 * \date 2008-02-04 01:10GMT
 * \author Mark Troutt (Mankar)
 * \author Jan Boon (Kaetemi)
 * MLogin
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2007-2008  Mark Troutt (Mankar)
 *                          Jan Boon (Kaetemi)
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

#ifndef SBCLIENT_LOGIN_H
#define SBCLIENT_LOGIN_H
#include <nel/misc/types_nl.h>

namespace SBCLIENT {

/**
 * \brief MLogin
 * \date 2008-02-04 01:10GMT
 * \author Jan Boon (Kaetemi)
 * MLogin
 */
class MLogin
{
	// todo: mix some code in here from SNB-24
protected:
	// pointers
	// ...
	
	// instances
	// ...
public:
	MLogin();
	virtual ~MLogin();
}; /* class MLogin */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LOGIN_H */

/* end of file */
