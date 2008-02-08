/**
 * \file landscape_ig_old.h
 * \brief CLandscapeIGOld
 * \date 2008-02-08 13:53GMT
 * \author Nevrax Ltd.
 * CLandscapeIGOld
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_LANDSCAPE_IG_OLD_H
#define SBCLIENT_LANDSCAPE_IG_OLD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "config_proxy.h"

// NeL includes

// STL includes
#include <vector>

namespace NLMISC {
	class IProgressCallback;
}

namespace NL3D {
	class UScene;
	class UInstanceGroup;
}

namespace SBCLIENT {

/**
 * \brief CLandscapeIGOld
 * \date 2008-02-08 13:53GMT
 * \author Nevrax Ltd.
 * CLandscapeIGOld
 */
class CLandscapeIGOld
{
public:
	// pointers
	std::vector<NL3D::UInstanceGroup *> InstanceGroups; // P
protected:
	// pointers
	NL3D::UScene *_Scene; // p
	
	// instances
	CConfigProxy _Config;
public:
	CLandscapeIGOld(NLMISC::IProgressCallback &progress, const std::string &id, NL3D::UScene *scene);
	virtual ~CLandscapeIGOld();
}; /* class CLandscapeIGOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LANDSCAPE_IG_OLD_H */

/* end of file */
