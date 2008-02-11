/**
 * \file input.cpp
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

#include <nel/misc/types_nl.h>
#include "input.h"

// Project includes

// NeL includes
#include <nel/misc/debug.h>
#include <nel/misc/progress_callback.h>
#include <nel/3d/u_driver.h>
#include <nel/misc/events.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CInput::CInput(NLMISC::IProgressCallback &progress, NL3D::UDriver *driver) : _Driver(driver)
{
	progress.progress(0.00f);
	nlassert(_Driver);

	_Driver->EventServer.addListener(EventKeyDownId, &Keyboard);
	_Driver->EventServer.addListener(EventKeyUpId, &Keyboard);
	_Driver->EventServer.addListener(EventCharId, &Keyboard);

	_Driver->EventServer.addListener(EventMouseMoveId, &Mouse);
	_Driver->EventServer.addListener(EventMouseDownId, &Mouse);
	_Driver->EventServer.addListener(EventMouseUpId, &Mouse);
	_Driver->EventServer.addListener(EventMouseDblClkId, &Mouse);
	_Driver->EventServer.addListener(EventMouseWheelId, &Mouse);
	progress.progress(1.00f);
}

CInput::~CInput()
{
	_Driver->EventServer.removeListener(EventKeyDownId, &Keyboard);
	_Driver->EventServer.removeListener(EventKeyUpId, &Keyboard);
	_Driver->EventServer.removeListener(EventCharId, &Keyboard);

	_Driver->EventServer.removeListener(EventMouseMoveId, &Mouse);
	_Driver->EventServer.removeListener(EventMouseDownId, &Mouse);
	_Driver->EventServer.removeListener(EventMouseUpId, &Mouse);
	_Driver->EventServer.removeListener(EventMouseDblClkId, &Mouse);
	_Driver->EventServer.removeListener(EventMouseWheelId, &Mouse);	
}

} /* namespace SBCLIENT */

/* end of file */
