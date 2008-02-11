/**
 * \file keyboard.h
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

#ifndef SBCLIENT_KEYBOARD_H
#define SBCLIENT_KEYBOARD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "key_binder.h"
#include "config_proxy.h"
#include "member_callback_decl.h"

// NeL includes

// STL includes
#include <vector>

namespace NL3D {
	class UDriver;
}

namespace NLMISC {
	class IProgressCallback;
}

namespace SBCLIENT {
	class CInputListener;
	class CConfigProxy;

/**
 * \brief CKeyboard
 * \date 2008-02-07 18:02GMT
 * \author Jan Boon (Kaetemi)
 * TASKS:
 * - CKeyBinder::updateInput in KeyBinder
 */
class CKeyboard
{
protected:
	// pointers
	NL3D::UDriver *_Driver; // p
	CInputListener *_InputListener; // p

	// instances
	CConfigProxy _Config;
	uint _ActionCommandId;
	std::vector<uint> _KeyIds;

public:
	// instances
	CKeyBinder KeyBinder;
public:
	CKeyboard(NLMISC::IProgressCallback &progress, const std::string &id, NL3D::UDriver *driver, CInputListener *inputListener);
	virtual ~CKeyboard();

	void enable();
	void disable();

protected:
	SBCLIENT_CALLBACK_INTERFACE_DECL(actionCommand);
	SBCLIENT_CALLBACK_CONFIG_DECL(configKeySettings);
	void releaseKeys();
	static CKeyBinder::TKeyState getKeyState(char c);
}; /* class CKeyboard */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_KEYBOARD_H */

/* end of file */
