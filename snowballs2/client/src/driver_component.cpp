/** \file driver_component.cpp
 * CDriverComponent
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * $Id$
 */

#include <nel/misc/types_nl.h>
#include "driver_component.h"

#include "component_manager.h"

#include <nel/3d/u_driver.h>
#include <nel/3d/u_text_context.h>
#include <nel/misc/path.h>
#include <nel/misc/file.h>
#include <nel/misc/bitmap.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CDriverComponent::CDriverComponent(CComponentManager *manager, 
	const string &instanceId, IProgressCallback &progressCallback)
: IConfigurableComponent(manager, instanceId, progressCallback)
{
	// create the driver
	_Driver = UDriver::createDriver(0, 
		_Config.getValue("Driver", string("OpenGL")) == string("Direct3D"));
	// initialize the window with config file values
	_Driver->setDisplay(UDriver::CMode(
		_Config.getValue("ScreenWidth", 800), 
		_Config.getValue("ScreenHeight", 600),
		_Config.getValue("ScreenDepth", 32),
		!_Config.getValue("ScreenFull", false)));
	// set the cache size for the font manager(in bytes)
	_Driver->setFontManagerMaxMemory(2097152);
	// let the manager know a driver exists, temp
	_Manager->setDriver(_Driver);
	// register config callbacks
	registerAndCallConfigCallback("WindowTitle");
	progressCallback.progress(0.5f);

	// create the text context
	_TextContext = _Driver->createTextContext(CPath::lookup(
		_Config.getValue("FontName", string("n019003l.pfb"))));
	_TextContext->setKeep800x600Ratio(false); 
	_TextContext->setShaded(_Config.getValue("FontShadow", true));
	progressCallback.progress(1.0f);
}

CDriverComponent::~CDriverComponent()
{
	unregisterConfigCallback("WindowTitle");
	_Manager->setDriver(NULL);
	_Driver->deleteTextContext(_TextContext);
	_Driver->release();
	delete _Driver;
}

void CDriverComponent::update()
{
	_Driver->EventServer.pump();
}

void CDriverComponent::render()
{
	
}

void CDriverComponent::config(const string &varName, CConfigFile::CVar &var)
{
	if (varName == "WindowTitle") setWindowTitle(i18n(var.asString()));
	else nlwarning("Unknown configuration callback '%s'", var.Name.c_str());
}

void CDriverComponent::setWindowTitle(const ucstring &title)
{
#ifdef NL_OS_WINDOWS
	SetWindowTextW((HWND)_Driver->getDisplay(), (LPCWSTR)title.c_str());
#endif
}

void CDriverComponent::saveScreenshot(const string &name, bool jpg)
{
	// empty bitmap
	CBitmap bitmap;
	// copy the driver buffer to the bitmap
	_Driver->getBuffer(bitmap);
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

}
