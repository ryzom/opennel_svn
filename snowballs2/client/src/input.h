/**
 * \file input.h
 * \brief CInput
 * \date 2008-02-11 09:04GMT
 * \author Jan Boon (Kaetemi)
 * CInput
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

#ifndef SBCLIENT_INPUT_H
#define SBCLIENT_INPUT_H
#include <nel/misc/types_nl.h>

// Project includes
#include "input_listener.h"

// NeL includes

// STL includes

namespace NL3D {
	class UDriver;
}

namespace NLMISC {
	class IProgressCallback;
}

namespace SBCLIENT {

/**
 * \brief CInput
 * \date 2008-02-11 09:04GMT
 * \author Jan Boon (Kaetemi)
 * CInput
 */
class CInput
{
protected:
	// pointers
	NL3D::UDriver *_Driver;
public:
	// instances
	CInputListener Keyboard;
	CInputListener Mouse;
public:
	CInput(NLMISC::IProgressCallback &progress, NL3D::UDriver *driver);
	virtual ~CInput();
}; /* class CInput */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_INPUT_H */

/* end of file */
