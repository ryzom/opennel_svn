/**
 * \file loading_screen.h
 * \brief CLoadingScreen
 * \date 2008-01-09 15:30GMT
 * \author Jan Boon (Kaetemi)
 * CLoadingScreen
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
 * \brief CLoadingScreen
 * \date 2008-01-09 15:30GMT
 * \author Jan Boon (Kaetemi)
 * CLoadingScreen
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
	CLoadingScreen();
	virtual ~CLoadingScreen();
	
	void setDriver(NL3D::UDriver *driver) { _Driver = driver; }
	void setTextContext(NL3D::UTextContext *textContext) { _TextContext = textContext; }
}; /* class CLoadingScreen */

} /* namespace SBCLIENT */

// setDirection(CVector)
// setSpeed(float)
// addForce(CVector) // average of forces (sum(forces).norm)
// removeForce
// average of average of forces and direction multiplied with speed
// ((sum(forces.norm) + direction).norm * speed

#endif /* #ifndef SBCLIENT_LOADING_SCREEN_H */

/* end of file */
