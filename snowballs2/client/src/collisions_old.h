/**
 * \file collisions_old.h
 * \brief CCollisionsOld
 * \date 2008-02-07 14:37GMT
 * \author Nevrax Ltd.
 * CCollisionsOld
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

#ifndef SBCLIENT_COLLISIONS_OLD_H
#define SBCLIENT_COLLISIONS_OLD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "config_proxy.h"

// NeL includes
#include <nel/misc/vector.h>
#include <nel/3d/animation_time.h>

// STL includes
#include <vector>

namespace NLPACS {
	class URetrieverBank;
	class UGlobalRetriever;
	class UMoveContainer;
	class UMovePrimitive;
}

namespace NL3D {
	class UVisualCollisionManager;
	class UVisualCollisionEntity;
	class UScene;
	class ULandscape;
}

namespace NLMISC {
	class IProgressCallback;
}

namespace SBCLIENT {
	class CTrajectoryOld;

/**
 * \brief CCollisionsOld
 * \date 2008-02-07 14:37GMT
 * \author Nevrax Ltd.
 * CCollisionsOld
 */
class CCollisionsOld
{
public:
	// pointers
	/// The retriever bank used in the world
	NLPACS::URetrieverBank *RetrieverBank; // P
	/// The global retriever used for pacs
	NLPACS::UGlobalRetriever *GlobalRetriever; // P
	/// The move container used for dynamic collisions
	NLPACS::UMoveContainer *MoveContainer; // P
	/// The collision manager for ground snapping
	NL3D::UVisualCollisionManager *VisualCollisionManager; // P
	
protected:
	// pointers
	NL3D::UScene *_Scene;
	NL3D::ULandscape *_Landscape;
	/// The collision primitive for the instances in the landscape
	std::vector<NLPACS::UMovePrimitive *> _InstancesMovePrimitives; // P
	/// Used to trace the landscape
	NL3D::UVisualCollisionEntity *_AimingEntity;
	// instances
	CConfigProxy _Config;
public:
	CCollisionsOld(NLMISC::IProgressCallback &progressCallback, const std::string &id, NL3D::UScene *scene, NL3D::ULandscape *landscape);
	virtual ~CCollisionsOld();

	NLMISC::CVector getTarget(const NLMISC::CVector &start, const NLMISC::CVector &step, uint numSteps);
	NLMISC::CVector	getTarget(CTrajectoryOld &trajectory, NL3D::TAnimationTime dtSteps, uint numSteps);
}; /* class CCollisionsOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_COLLISIONS_OLD_H */

/* end of file */
