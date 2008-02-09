/**
 * \file collisions_old.cpp
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

#include <nel/misc/types_nl.h>
#include "collisions_old.h"

// Project includes
#include "trajectory_old.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/3d/u_landscape.h>
#include <nel/misc/progress_callback.h>
#include <nel/pacs/u_retriever_bank.h>
#include <nel/pacs/u_global_retriever.h>
#include <nel/pacs/u_move_container.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_visual_collision_manager.h>
#include <nel/3d/u_visual_collision_entity.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NLPACS;
using namespace NL3D;

namespace SBCLIENT {

CCollisionsOld::CCollisionsOld(NLMISC::IProgressCallback &progressCallback, const std::string &id, NL3D::UScene *scene, NL3D::ULandscape *landscape)
: _Config(id), _Scene(scene), _Landscape(landscape), _AimingEntity(NULL)
{
	progressCallback.progress(0.0f);
	nlassert(_Scene);
	nlassert(_Landscape);

	// init the retriever bank
	RetrieverBank = URetrieverBank::createRetrieverBank(
		_Config.getVar("RetrieverBankName").asString().c_str());
	progressCallback.progress(0.2f);

	// init the global retriever
	GlobalRetriever = UGlobalRetriever::createGlobalRetriever(
		_Config.getVar("GlobalRetrieverName").asString().c_str(), 
		RetrieverBank);
	progressCallback.progress(0.4f);

	// create the move container
	MoveContainer = UMoveContainer::createMoveContainer(
		GlobalRetriever, 100, 100, 6.0);
	progressCallback.progress(0.6f);

	// create a visual collision manager
	VisualCollisionManager = _Scene->createVisualCollisionManager();
	VisualCollisionManager->setLandscape(_Landscape);
	progressCallback.progress(0.8f);

	// create the aiming entity
	_AimingEntity = VisualCollisionManager->createEntity();
	_AimingEntity->setCeilMode(true);
	// -- -- move this to snowballs specific game task
	// create a move primitive for each instance in the instance group
	//////uint	i, j;
	//////for (j=0; j<InstanceGroups.size(); ++j)
	//////{
	//////	for (i=0; i<InstanceGroups[j]->getNumInstance(); ++i)
	//////	{
	//////		UMovePrimitive	*primitive = MoveContainer->addCollisionablePrimitive(0, 1);
	//////		primitive->setPrimitiveType(UMovePrimitive::_2DOrientedCylinder);
	//////		primitive->setReactionType(UMovePrimitive::DoNothing);
	//////		primitive->setTriggerType(UMovePrimitive::NotATrigger);
	//////		primitive->setCollisionMask(OtherCollisionBit+SelfCollisionBit+SnowballCollisionBit);
	//////		primitive->setOcclusionMask(StaticCollisionBit);
	//////		primitive->setObstacle(true);

	//////		// setup the radius of each mesh in the instance group
	//////		string	name = InstanceGroups[j]->getShapeName(i);
	//////		float rad;

	//////		// -- -- improve this
	//////		     if (strlwr(name) == "pi_po_igloo_a")		rad = 4.5f;
	//////		else if (strlwr(name) == "pi_po_snowman_a")		rad = 1.0f;
	//////		else if (strlwr(name) == "pi_po_pinetree_a")	rad = 2.0f;
	//////		else if (strlwr(name) == "pi_po_tree_a")		rad = 2.0f;
	//////		else if (strlwr(name) == "pi_po_pingoo_stat_a")	rad = 1.0f;
	//////		else if (strlwr(name) == "pi_po_gnu_stat_a")	rad = 1.0f;
	//////		else
	//////		{
	//////			rad = 2.0f;
	//////			nlwarning ("Instance name '%s' doesn't have a good radius for collision", name.c_str());
	//////		}

	//////		primitive->setRadius(rad);
	//////		primitive->setHeight(6.0f);

	//////		primitive->insertInWorldImage(0);
	//////		CVector	pos = InstanceGroups[j]->getInstancePos(i);
	//////		primitive->setGlobalPosition(CVectorD(pos.x, pos.y, pos.z-1.5f), 0);
	//////		InstancesMovePrimitives.push_back(primitive);
	//////	}
	//////}
	progressCallback.progress(1.0f);
}

CCollisionsOld::~CCollisionsOld()
{
	// all move primitives
	for (uint i = 0; i < _InstancesMovePrimitives.size(); ++i)
		MoveContainer->removePrimitive(_InstancesMovePrimitives[i]);
	_InstancesMovePrimitives.clear();

	// delete the aiming entity
	VisualCollisionManager->deleteEntity(_AimingEntity);

	// delete the visual collision manager
	_Scene->deleteVisualCollisionManager(VisualCollisionManager);

	// delete all pacs stuff
	UMoveContainer::deleteMoveContainer(MoveContainer);
	UGlobalRetriever::deleteGlobalRetriever(GlobalRetriever);
	URetrieverBank::deleteRetrieverBank(RetrieverBank);
}

// -- -- mix with following bit of code for higher accuracy
//NLMISC::CVector CSceneryMouse::getLandscape()
//{
//	if (_LandscapeCached) return _LandscapeCache;
//	CViewport v = _Driver->getViewport();
//	CVector pos, dir; -- -- random note: this code gets the landscape position where the mouse is pointing at
//	v.getRayWithPoint(_X * v.getWidth(), _Y * v.getHeight(), pos, dir, _Camera.getMatrix(), _Camera.getFrustum());
//	dir.normalize();
//	dir *= 50;
// -- -- float rc = _Landscape->getRayCollision(pos, pos + dir);
// -- -- _LandscapeCache = pos + (rc * dir);
//	_LandscapeCached = true;
//	return _LandscapeCache;
//}
// -- -- if higher than 50 or something, use code below
CVector CCollisionsOld::getTarget(const CVector &start, const CVector &step, uint numSteps)
{
	CVector	testPos = start;

	uint	i;
	for (i=0; i<numSteps; ++i)
	{
		CVector	snapped = testPos;
		CVector	normal;

		// here use normal to check if we have collision
		if (_AimingEntity->snapToGround(snapped, normal) && (testPos.z - snapped.z) * normal.z < 0.0f)
		{
			testPos -= step * 0.5f;
			break;
		}
		testPos += step;
	}
	return testPos;
}

CVector CCollisionsOld::getTarget(CTrajectoryOld &trajectory, TAnimationTime dtSteps, uint numSteps)
{
	TGlobalAnimationTime gt = trajectory.getStartTime();
	CVector	testPos;

	uint i;
	for (i = 0; i < numSteps; ++i)
	{
		testPos = trajectory.eval(gt);
		CVector	snapped = testPos;
		CVector	normal;

		// here use normal to check if we have collision
		if (_AimingEntity->snapToGround(snapped, normal) && (testPos.z - snapped.z) * normal.z < 0.0f)
		{
			gt -= (dtSteps / 2);
			testPos = trajectory.eval(gt);
			break;
		}
		gt += dtSteps;
	}
	return testPos;
}

} /* namespace SBCLIENT */

/* end of file */
