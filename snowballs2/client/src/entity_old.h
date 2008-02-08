/**
 * \file entity_old.h
 * \brief CEntityOld
 * \date 2008-02-07 10:58GMT
 * \author Nevrax Ltd.
 * CEntityOld
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

#ifndef SBCLIENT_ENTITY_OLD_H
#define SBCLIENT_ENTITY_OLD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "trajectory_old.h"

// NeL includes
#include <nel/misc/vector.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_skeleton.h>
#include <nel/3d/animation_time.h>

// STL includes
#include <queue>

namespace NLSOUND {
	class USource;
}

namespace NLPACS {
	class UMovePrimitive;
}

namespace NL3D {
	class UVisualCollisionEntity;
	class UPlayList;
}

namespace SBCLIENT {

/**
 * \brief CEntityOld
 * \date 2008-02-07 10:58GMT
 * \author Nevrax Ltd.
 * An entity that will move through the landscape.
 * The possible entities are the Self (the player's avatar), the Other and the Snowball
 */
class CEntityOld
{
public:
	/// Collision bits for pacs
	enum TCollisionBit
	{
		SelfCollisionBit = 1,
		OtherCollisionBit = 2,
		SnowballCollisionBit = 4,
		StaticCollisionBit = 8
	};

	/// The state enum of the entity
	enum TState
	{ 
		Appear, 
		Normal, 
		Disappear
	};	

	/// The type enum of the entity
	enum TType
	{ 
		Self, 
		Other, 
		Snowball
	};

	/// Animation types
	enum TAnim
	{ 
		NoAnim = -1, 
		PrepareWalkAnim = 0, 
		WalkAnim = 1, 
		IdleAnim = 2, 
		LogInAnim = 3, 
		LogOffAnim = 4, 
		ThrowSnowball = 5, 
		PrepareSnowBall = 6, 
		PrepareSnowBallCycle = 7, 
		HitAnim = 8,
	};

	/// Create a default entity
	CEntityOld();
	virtual ~CEntityOld();

	/// The id of the entity
	uint32 Id;

	/// The name of the entity
	ucstring Name;

	/// Contain the target position for this entity
	NLMISC::CVector ServerPosition;

	/// Contain the current position of the entity
	NLMISC::CVector Position;

	/// The immediate speed of the entity
	NLMISC::CVector ImmediateSpeed;

	/// The maximum speed of the entity
	float Speed;
	/// The angle of the entity
	float Angle;
	/// Various angle controls for the interpolation
	float AuxiliaryAngle, InterpolatedAuxiliaryAngle;

	/// The trajectory (only for snowballs, defined in physics.h)
	CTrajectoryOld Trajectory;

	/// The state of this entity
	TState State;
	/// The date of the beginning of this state
	NL3D::TGlobalAnimationTime StateStartTime;

	/// The type of this entity
	TType Type;

	/// Is it an auto-moving entity
	bool AutoMove;

	/// The PACS move primitive
	NLPACS::UMovePrimitive *MovePrimitive;
	/// The collision entity (for ground snapping)
	NL3D::UVisualCollisionEntity *VisualCollisionEntity;
	/// The mesh instance associated to this entity
	NL3D::UInstance Instance;
	/// The skeleton binded to the instance
	NL3D::USkeleton Skeleton;
	/// The particle system (for appear and disappear effects)
	NL3D::UInstance Particule;

	/// The sound source associated to the entity
	NLSOUND::USource *Source;

	bool IsWalking;
	bool WasWalking;
	bool IsAiming;
	bool WasAiming;

	/// Playlist linked to this entity
	//EAnim CurrentAnim;
	uint NextEmptySlot;
	NL3D::UPlayList *PlayList;
	std::queue<TAnim> AnimQueue;
	NL3D::TGlobalAnimationTime StartAnimationTime;

	uint BotState;
	NL3D::TGlobalAnimationTime BotStateStart;
	
	/// Set the state of the entity (Appear, Normal, Disappear)
	void setState(TState state, NL3D::TGlobalAnimationTime gt);
}; /* class CEntityOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_ENTITY_OLD_H */

/* end of file */
