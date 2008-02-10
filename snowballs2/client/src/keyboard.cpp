/**
 * \file keyboard.cpp
 * \brief CKeyboard
 * \date 2008-02-07 18:02GMT
 * \author Jan Boon (Kaetemi)
 * CKeyboard
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
#include "keyboard.h"

// Project includes
#include "config_proxy.h"

// NeL includes
// #include <nel/misc/debug.h>

// STL includes

using namespace std;
// using namespace NLMISC;

namespace SBCLIENT {

CKeyboard::CKeyboard() : _Driver(NULL), _InputListener(NULL), 
_Config(NULL)
{
	assertNULL();
}

CKeyboard::~CKeyboard()
{
	assertNULL(); // yes, assert first, you must call release first
	release(); // and then just to be sure release anyway
}

void CKeyboard::init(NLMISC::IProgressCallback &progress, const std::string &id, NL3D::UDriver *driver, SBCLIENT::CInputListener *inputListener)
{
	assertNULL();
	
	// init external pointers
	_Driver = driver;
	_InputListener = inputListener;

	// init internal pointers
	_Config = new CConfigProxy(id);

	// init key binder
	KeyBinder.init();
	KeyBinder.takeControl(_Driver, _InputListener);

	// read configured keys

	assertINIT();
}

void CKeyboard::release()
{
	// assertINIT should be called in the main client code before releasing

	// release configured keys

	// release key binder
	KeyBinder.dropControl();
	KeyBinder.release();
	
	// release internal pointers
	delete _Config; _Config = NULL;

	// release external pointers
	_Driver = NULL;
	_InputListener = NULL;

	assertNULL();
}

void CKeyboard::assertINIT()
{
	nlassert(_Driver);
	nlassert(_InputListener);
	nlassert(_Config);
	KeyBinder.assertINIT();
}

void CKeyboard::assertNULL()
{
	nlassert(!_Driver);
	nlassert(!_InputListener);
	nlassert(!_Config);
	KeyBinder.assertNULL();
}

} /* namespace SBCLIENT */

/* end of file */
