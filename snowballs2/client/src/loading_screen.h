/** \file loading_screen.h
 * CLoadingScreen
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

#ifndef SBCLIENT_LOADING_SCREEN_H
#define SBCLIENT_LOADING_SCREEN_H
#include <nel/misc/types_nl.h>
#include <nel/misc/progress_callback.h>

namespace NL3D {
class UDriver;
class UTextContext;
}

namespace SBCLIENT {

/**
 * Loading screen.
 * \date 2007-2008
 */
class CLoadingScreen : public NLMISC::IProgressCallback
{
protected:
	// pointers
	NL3D::UDriver *_Driver; // not deleted here
	NL3D::UTextContext *_TextContext; // not deleted here
	
	// instances
	std::string _State;
public:
	/// Basic constructor of a component.
	/// Requires a pointer to the componentmanager and a unique name.
	CLoadingScreen();
	virtual ~CLoadingScreen();
	
	void setDriver(NL3D::UDriver *driver) { _Driver = driver; }
	void setTextContext(NL3D::UTextContext *textContext) { _TextContext = textContext; }
};

}

// setDirection(CVector)
// setSpeed(float)
// addForce(CVector) // average of forces (sum(forces).norm)
// removeForce
// average of average of forces and direction multiplied with speed
// ((sum(forces.norm) + direction).norm * speed

#endif /* #ifndef SBCLIENT_LOADING_SCREEN_H */
