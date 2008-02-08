/**
 * \file entities_old.h
 * \brief CEntitiesOld
 * \date 2008-02-07 10:55GMT
 * \author Nevrax Ltd.
 * CEntitiesOld
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

#ifndef SBCLIENT_ENTITIES_OLD_H
#define SBCLIENT_ENTITIES_OLD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "member_callback_decl.h"
#include "entity_old.h"

// NeL includes
#include <nel/3d/animation_time.h>

// STL includes
#include <map>

namespace NL3D {
	class UScene;
	class UVisualCollisionManager;
}

namespace NLMISC {
	class IProgressCallback;
}

namespace NLPACS {
	class UGlobalRetriever;
	class UMoveContainer;
}

namespace SBCLIENT {
	class CAnimationOld;

/**
 * \brief CEntitiesOld
 * \date 2008-02-07 10:55GMT
 * \author Nevrax Ltd.
 * CEntitiesOld
 */
class CEntitiesOld
{
public:
	// types
	typedef std::map<uint32, CEntityOld> CEntityMap;

	// pointers
	CEntityOld *Self; // points to entity in map
	
	// instances
	// A map of entities. All entities are later referred by their unique id
	CEntityMap Entities;

	// Entity Id, only used offline
	uint32 LastEID;

	// The speed settings
	static const float PlayerSpeed;
	static const float SnowballSpeed;

	bool _TestCLS;
protected:
	// pointers
	NL3D::UScene *_Scene; // p
	NL3D::UVisualCollisionManager *_VisualCollisionManager; // p
	NLPACS::UMoveContainer *_MoveContainer; // p
	NLPACS::UGlobalRetriever *_GlobalRetriever; // p
	CAnimationOld *_Animation; // c
	NL3D::TGlobalAnimationTime *_GlobalAnimationTime; // p
	NL3D::TGlobalAnimationTime *_GlobalAnimationTimeDelta; // p
public:
	CEntitiesOld(NLMISC::IProgressCallback &progressCallback, NL3D::UScene *scene, NL3D::UVisualCollisionManager *visualCollisionManager, NLPACS::UMoveContainer *moveContainer, NLPACS::UGlobalRetriever *globalRetriever, CAnimationOld *animation, NL3D::TGlobalAnimationTime *globalAnimationTime, NL3D::TGlobalAnimationTime *globalAnimationTimeDelta);
	virtual ~CEntitiesOld();

	CEntityOld &getEntity(uint32 eid);
	CEntityOld *getEntityPtr(uint32 eid);

	// -- -- things like Creature, Effect, Scenery seem more flexible than Self, Other, Snowball
	// -- -- entitybehavior (animations), entityinteraction (targetable, menu, )
	/// Creates an entity, given its id, its type (Self, Other, Snowball), its start and server positions.
	void addEntity(uint32 eid, const ucstring &name, CEntityOld::TType type, const NLMISC::CVector &startPosition, const NLMISC::CVector &serverPosition);
	
	/// Remove an entity specified by its id
	/// The entity passes into the Disappear state
	void removeEntity(uint32 eid);

	void removeAll();

	///////// Reset the pacs position of an entity (in case pacs went wrong) :O
	//////void resetEntityPosition(uint32 eid);

	/// Process the event when an entity shoots a snowball, creates a temporary snowball entity with sid
	void shotSnowball(uint32 sid, uint32 eid, const NLMISC::CVector &start, const NLMISC::CVector &target, float speed, float deflagRadius);

	SBCLIENT_CALLBACK_DECL(updateEntities);

protected:
	/// Effectively remove the entity (don't play animation)
	void deleteEntity(CEntityOld &entity);

	/// What to do when the entity appears
	void stateAppear(CEntityOld &entity);

	/// What to do when the entity disappears
	void stateDisappear(CEntityOld &entity);

	/// Update an entity
	void stateNormal(CEntityOld &entity);
}; /* class CEntitiesOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_ENTITIES_OLD_H */

/* end of file */
