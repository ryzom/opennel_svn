/**
 * \file landscape_ig_old.cpp
 * \brief CLandscapeIGOld
 * \date 2008-02-08 13:53GMT
 * \author Nevrax Ltd. (Nevrax Ltd.)
 * CLandscapeIGOld
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2001  Nevrax Ltd. (Nevrax Ltd.)
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

#include <nel/misc/types_nl.h>
#include "landscape_ig_old.h"

// Project includes

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/misc/progress_callback.h>
#include <nel/misc/time_nl.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_instance_group.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CLandscapeIGOld::CLandscapeIGOld(IProgressCallback &progress, 
const std::string &id, UScene *scene) : _Config(id), _Scene(scene)
{
	progress.progress(0.f);
	nlassert(_Scene);

	CConfigFile::CVar igv = _Config.getVar("InstanceGroups");
	TTime ot = CTime::getLocalTime();
	for (uint i = 0; i < igv.size(); ++i)
	{
		UInstanceGroup *inst = UInstanceGroup::createInstanceGroup(igv.asString(i));
		if (!inst) 
		{
			nlwarning("Instance group '%s' not found", igv.asString (i).c_str());
		}
		else
		{
			inst->addToScene(*_Scene);
			InstanceGroups.push_back(inst);
		}
		TTime nt = CTime::getLocalTime();
		if (nt - ot > 100) // min time diff for showing progress
			progress.progress((float)(i + 1) / (float)igv.size());
		ot = nt;
	}
	progress.progress(1.f);
}

CLandscapeIGOld::~CLandscapeIGOld()
{
	vector<NL3D::UInstanceGroup *>::iterator 
		it(InstanceGroups.begin()), end(InstanceGroups.end());
	for (; it != end; ++it)
	{
		UInstanceGroup *inst = (*it);
		inst->removeFromScene(*_Scene);
		delete inst;
	}
	InstanceGroups.clear();
}

} /* namespace SBCLIENT */

/* end of file */
