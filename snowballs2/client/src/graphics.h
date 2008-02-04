/**
 * \file graphics.h
 * \brief MGraphics
 * \date 2008-01-21 23:40GMT
 * \author Jan Boon (Kaetemi)
 * MGraphics
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS, Copyright (C) 2001  Nevrax Ltd.
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

#ifndef SBCLIENT_GRAPHICS_H
#define SBCLIENT_GRAPHICS_H
#include <nel/misc/types_nl.h>

#include "config_proxy.h"

#include <nel/misc/ucstring.h>

namespace NL3D {
	class UDriver;
	class UTextContext;
}

namespace NLMISC {
	class IProgressCallback;
}

namespace SBCLIENT {
	class CI18NHelper;

/**
 * \brief MGraphics
 * \date 2008-01-21 23:40GMT
 * \author Jan Boon (Kaetemi)
 * MGraphics
 */
class MGraphics
{
protected:
	// instances
	CConfigProxy _Config;

	// pointers
	CI18NHelper *_I18N; // not deleted here
public:
	// pointers
	NL3D::UDriver *Driver; // deleted here
	NL3D::UTextContext *TextContext; // deleted here, might be moved
public:
	MGraphics(NLMISC::IProgressCallback &progressCallback, const std::string &id, CI18NHelper *i18n);
	virtual ~MGraphics();

	static void updateDriver(void *context, void *tag);

	/// Set the window title, accepts an i18n label.
	void setWindowTitle(const std::string &title);
	/// Set the window title directly to an ucstring.
	void setWindowTitle(const ucstring &title);

	/// Save a screenshot to the screenshot directory.
	/// A number and the .tga file extension is added to the name.
	/// Must be called from an update(), don't call within render().
	void saveScreenshot(const std::string &name, bool jpg);

	/** Function called when user closes the window. Set before class init. */
	static void (*DriverExit)();

private:
	static void configWindowTitle(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag);
}; /* class MGraphics */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_GRAPHICS_H */

/* end of file */
