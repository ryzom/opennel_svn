/**
 * \file graphics.cpp
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

#include <nel/misc/types_nl.h>
#include "graphics.h"

#include "i18n_helper.h"

// #include <nel/misc/debug.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_text_context.h>
#include <nel/misc/progress_callback.h>
#include <nel/misc/path.h>
#include <nel/misc/file.h>
#include <nel/misc/bitmap.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

void (*MGraphics::DriverExit)() = NULL;

MGraphics::MGraphics(NLMISC::IProgressCallback &progressCallback, const std::string &id, CI18NHelper *i18n) 
	: _Config(id), _I18N(i18n)
{
	// create the driver
	Driver = UDriver::createDriver(0, _Config.getValue(
		"Driver", string("OpenGL")) == string("Direct3D"), DriverExit);
	// initialize the window with config file values
	Driver->setDisplay(UDriver::CMode(
		_Config.getValue("ScreenWidth", 800), 
		_Config.getValue("ScreenHeight", 600),
		_Config.getValue("ScreenDepth", 32),
		!_Config.getValue("ScreenFull", false)));
	// set the cache size for the font manager(in bytes)
	Driver->setFontManagerMaxMemory(2097152);
	// register config callbacks
	_Config.setCallbackAndCall("WindowTitle", configWindowTitle, this, NULL);
	progressCallback.progress(0.5f);

	// create the text context
	TextContext = Driver->createTextContext(CPath::lookup(
		_Config.getValue("FontName", string("n019003l.pfb"))));
	TextContext->setKeep800x600Ratio(false); 
	TextContext->setShaded(_Config.getValue("FontShadow", true));
	progressCallback.progress(1.0f);
}

MGraphics::~MGraphics()
{
	_Config.dropCallback("WindowTitle");
	Driver->deleteTextContext(TextContext);
	Driver->release();
	delete Driver;
}

void MGraphics::updateDriver(void *context, void *tag)
{	
	MGraphics *me = (MGraphics *)context;
	me->Driver->EventServer.pump();	
}

void MGraphics::setWindowTitle(const std::string &title)
{
	// get the window title from a label
	setWindowTitle(_I18N->get(title));
}

void MGraphics::setWindowTitle(const ucstring &title)
{
	// set the window title, ucstring is prefered
	// replace this code once the UDriver supports ucstring too ;)
#ifdef NL_OS_WINDOWS
	SetWindowTextW((HWND)Driver->getDisplay(), (LPCWSTR)title.c_str());
#else
	Driver->setWindowTitle(title.toString());	
#endif
}

void MGraphics::saveScreenshot(const string &name, bool jpg)
{
	// empty bitmap
	CBitmap bitmap;
	// copy the driver buffer to the bitmap
	Driver->getBuffer(bitmap);
	// create the file name with number
	string filename = CFile::findNewFile(
		_Config.getValue("ScreenshotPath", "screenshots")
		+ "/" + name + (jpg ? ".jpg" : ".tga"));
	// open the new output file
	COFile outputFile(filename);
	// write the bitmap as a tga or jpg to the file
	if (jpg) bitmap.writeJPG(outputFile, 100);
	else bitmap.writeTGA(outputFile, 24, false);
	// show a message in the logs
	nlinfo("Screenshot '%s' saved", filename.c_str());
}

void MGraphics::configWindowTitle(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag)
{
	MGraphics *me = (MGraphics *)context;
	me->setWindowTitle(var.asString());
}

} /* namespace SBCLIENT */

/* end of file */
