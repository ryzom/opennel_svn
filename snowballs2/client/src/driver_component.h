/** \file driver_component.h
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

#ifndef SBCLIENT_DRIVER_COMPONENT_H
#define SBCLIENT_DRIVER_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace NL3D {
	class UDriver;
	class UTextContext;
}

namespace SBCLIENT {

class CComponentManager;

/**
 * Driver component.
 * \date 2007-2008
 */
class CDriverComponent : public IConfigurableComponent
{
protected:
	// pointers
	NL3D::UDriver *_Driver; // deleted here
	NL3D::UTextContext *_TextContext; // deleted here, might be moved
	
	// instances
	// ...
public:
	/// Basic constructor of a component.
	/// Requires a pointer to the componentmanager and a unique name.
	CDriverComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CDriverComponent();
	
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	
	void setWindowTitle(const ucstring &title);
	NL3D::UDriver *getDriver();
	static NL3D::UDriver *getDriver(const std::string &instanceId);
	NL3D::UTextContext *getTextContext();
	
	/// Save a screenshot to the screenshot directory.
	/// A number and the .tga file extension is added to the name.
	/// Must be called from an update(), don't call within render().
	void saveScreenshot(const std::string &name, bool jpg);

	/** Function called when user closes the window. */
	static void (*DriverExit)();
};

}

#endif /* #ifndef SBCLIENT_DRIVER_COMPONENT_H */
