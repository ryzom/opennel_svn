/**
* \file igameeventlistener.h
* \date Wed Nov 7 2007
* \author Matt Raykowski
*/

/*
* Copyright Matt Raykowski <matt.raykowski@gmail.com>, (C) 2007
*
* This file is part of OPENNEL.
* OPENNEL is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* OPENNEL is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with OPENNEL; see the file COPYING. If not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/

#ifndef NL_IGAMEEVENTSERVER_H
#define NL_IGAMEEVENTSERVER_H

//
// Standard Includes
//

//
// System Includes
//

//
// OpenNeL Includes
//
#include <nel/misc/smart_ptr.h>

//
// Local Includes
//
#include "gameeventserver.h"

//
// Namespaces
//

namespace NLEM {

class IGameEvent;

/**
* \class igameeventlistener.h
* \brief Describe your class briefly
*/
class IGameEventListener {
public:
	virtual bool observePreGameEvent(NLEM::CGameEventServer::EventPtr event) = 0;
	virtual bool observeGameEvent(NLEM::CGameEventServer::EventPtr event) = 0;
	virtual bool observePostGameEvent(NLEM::CGameEventServer::EventPtr event) = 0;
}; // END OF CLASS IGAMEEVENTLISTENER

}; // END OF NAMESPACE NLEM

#endif // NL_IGAMEEVENTSERVER_H
