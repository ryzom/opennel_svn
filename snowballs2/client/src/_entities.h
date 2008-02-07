/** \file entities.h
 * Snowballs 2 specific code for managing the entities
 *
 * $Id$
 */

/* Copyright, 2001 Nevrax Ltd.
 *
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef ENTITIES_H
#define ENTITIES_H

//
// Includes
//

#include <string>
#include <map>
#include <queue>

#include <nel/misc/vector.h>
#include <nel/misc/time_nl.h>

#include <nel/3d/animation_time.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_skeleton.h>

#include "_physics.h"
#include "_animation.h"

//
// External definitions
//

//
// Enums
//

// The collision bits used by pacs (dynamic collisions)

//
// External variables
//

// The entity representing the player avatar
extern CEntity								*Self;

// The speed of the player
extern float								PlayerSpeed;
// The speed of the snowball
extern float								SnowballSpeed;

// The entities storage
extern std::map<uint32, CEntity>			Entities;
typedef std::map<uint32, CEntity>::iterator	EIT;

extern uint32 NextEID;

//
// External functions
//

EIT findEntity (uint32 eid, bool needAssert = true);

void	addEntity (uint32 eid, std::string name, CEntity::TType type, const NLMISC::CVector &startPosition, const NLMISC::CVector &serverPosition);
void	removeEntity (uint32 eid);

// when we turn online, we need to clear all offline entities
void	removeAllEntitiesExceptUs ();
void deleteAllEntities();

void initEntities();
void updateEntities();
void releaseEntities();

// Reset the pacs position of an entity (in case pacs went wrong)
void	resetEntityPosition(uint32 eid);
// Process the event when an entity shoots a snowball
void	shotSnowball(uint32 sid, uint32 eid, const NLMISC::CVector &start, const NLMISC::CVector &target, float speed, float deflagRadius);

void	renderEntitiesNames ();


#endif // ENTITIES_H

/* End of entities.h */
