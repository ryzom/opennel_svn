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

// NeL includes

// STL includes

namespace NL3D {
	class UDriver;
}

namespace SBCLIENT {

/**
 * \brief CKeyBinder
 * \date 2008-02-09 21:27GMT
 * \author Jan Boon (Kaetemi)
 * CKeyBinder
 */
class CKeyBinder
{
protected:
	// pointers
	// ...
	
	// instances
	// ...
public:
	CKeyBinder();
	virtual ~CKeyBinder();

	void blah();

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
