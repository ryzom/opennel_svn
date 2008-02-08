/**
 * \file entities_old.cpp
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

#include <nel/misc/types_nl.h>
#include "entities_old.h"

// Project includes
#include "member_callback_impl.h"
#include "animation_old.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/3d/u_visual_collision_manager.h>
#include <nel/3d/u_visual_collision_entity.h>
#include <nel/pacs/u_move_container.h>
#include <nel/pacs/u_move_primitive.h>
#include <nel/pacs/u_global_retriever.h> // for pacs in update..
#include <nel/misc/vectord.h>
#include <nel/3d/u_scene.h>
#include <nel/misc/progress_callback.h>

// STL includes

#define SBCLIENT_WARNING(__text) message(__FILE__ " : warning SBCLIENT: " __text)

using namespace std;
using namespace NLMISC;
using namespace NL3D;
using namespace NLPACS;

namespace SBCLIENT {

const float CEntitiesOld::PlayerSpeed = 10.0f;	// 6.5 km/h
const float CEntitiesOld::SnowballSpeed = 15.0f;	// 36 km/h

CEntitiesOld::CEntitiesOld(NLMISC::IProgressCallback &progressCallback, 
UScene *scene, NL3D::UVisualCollisionManager *visualCollisionManager, 
UMoveContainer *moveContainer, UGlobalRetriever *globalRetriever, 
CAnimationOld *animation, TGlobalAnimationTime *globalAnimationTime,
TGlobalAnimationTime *globalAnimationTimeDelta) : 
_TestCLS(false), Self(NULL), _Scene(scene), 
_VisualCollisionManager(visualCollisionManager), 
_MoveContainer(moveContainer), _GlobalRetriever(globalRetriever), 
_Animation(animation), LastEID(1000000), 
_GlobalAnimationTime(globalAnimationTime), 
_GlobalAnimationTimeDelta(globalAnimationTimeDelta)
{
	progressCallback.progress(0.0f);
	nlassert(!Self);
	nlassert(_Scene);
	nlassert(_VisualCollisionManager);
	nlassert(_MoveContainer);
	nlassert(_GlobalRetriever);
	nlassert(_Animation);
	nlassert(_GlobalAnimationTime);
	nlassert(_GlobalAnimationTimeDelta);
	_Animation->_Entities = this;
	progressCallback.progress(1.0f);
}

CEntitiesOld::~CEntitiesOld()
{
	removeAll();
}

CEntityOld &CEntitiesOld::getEntity(uint32 eid)
{
	CEntityOld *entity = getEntityPtr(eid);
	if (!entity) nlerror("Entity %u not found", eid);
	return *entity;
}

CEntityOld *CEntitiesOld::getEntityPtr(uint32 eid)
{
	CEntityMap::iterator entity = Entities.find(eid);
	if (entity == Entities.end()) return NULL;
	return &entity->second;
}

void CEntitiesOld::addEntity(uint32 eid, const ucstring &name, CEntityOld::TType type, const CVector &startPosition, const CVector &serverPosition)
{
	nlinfo("adding entity %u", eid);
	TGlobalAnimationTime gt = *_GlobalAnimationTime;

	// Check that the entity doesn't exist yet
	if (getEntityPtr(eid))
		nlerror("Entity %d already exist", eid);

	// Create a new entity
	CEntityMap::iterator eit = (Entities.insert(make_pair(eid, CEntityOld()))).first;
	CEntityOld &entity = (*eit).second;

	// Check that in the case the entity newly created is a Self, there's not a Self yet.
	if (type == CEntityOld::Self)
	{
		if (Self) nlerror("Self entity already created");
		Self = &entity;
	}

	// Set the entity up
	entity.Id = eid;
	entity.Type = type;
	entity.Name = name;
	entity.Position = startPosition;
	entity.Angle = 0.0f;
	entity.ServerPosition = serverPosition;
	entity.VisualCollisionEntity = _VisualCollisionManager->createEntity();

	// setup the move primitive and the mesh instance depending on the type of entity
	switch (type)
	{
	case CEntityOld::Self:
		// create a move primitive associated to the entity
		entity.MovePrimitive = _MoveContainer->addCollisionablePrimitive(0, 1);
		// it's a cylinder
		entity.MovePrimitive->setPrimitiveType(UMovePrimitive::_2DOrientedCylinder);
		// the entity should slide against obstacles
		entity.MovePrimitive->setReactionType(UMovePrimitive::Slide);
		// do not generate event if there is a collision
		entity.MovePrimitive->setTriggerType(UMovePrimitive::NotATrigger);
		// which entity should collide against me
		entity.MovePrimitive->setCollisionMask(CEntityOld::OtherCollisionBit | CEntityOld::SnowballCollisionBit | CEntityOld::StaticCollisionBit);
		// the self collision bit
		entity.MovePrimitive->setOcclusionMask(CEntityOld::SelfCollisionBit);
		// the self is an obstacle
		entity.MovePrimitive->setObstacle(true);
		// the size of the cylinder
		entity.MovePrimitive->setRadius(1.0f);
		entity.MovePrimitive->setHeight(1.8f);
		// only use one world image, so use insert in world image 0
		entity.MovePrimitive->insertInWorldImage(0);
		// retrieve the start position of the entity
		entity.MovePrimitive->setGlobalPosition(CVectorD(startPosition.x, startPosition.y, startPosition.z), 0);

		// create instance of the mesh character
		entity.Instance = _Scene->createInstance("gnu.shape");
		entity.Skeleton = _Scene->createSkeleton("gnu.skel");
		// use the instance on the skeleton
		entity.Skeleton.bindSkin(entity.Instance);

		// Allow the skeleton to cast shadows.
		entity.Skeleton.enableCastShadowMap(true);

		entity.Instance.hide();

		//#pragma SBCLIENT_WARNING("SBCLIENT_MOUSE_LISTENER")
		//entity.Angle = MouseListener->getOrientation();
		entity.Angle = 0.0f;

		// setup final parameters
		entity.Speed = PlayerSpeed;
		entity.Particule = _Scene->createInstance("appear.ps");
		entity.setState(CEntityOld::Appear, gt);
		_Animation->playAnimation(entity, CEntityOld::LogInAnim);
		_Animation->playAnimation(entity, CEntityOld::IdleAnim);

		break;
	case CEntityOld::Other:
		entity.MovePrimitive = _MoveContainer->addCollisionablePrimitive(0, 1);
		entity.MovePrimitive->setPrimitiveType(UMovePrimitive::_2DOrientedCylinder);
		entity.MovePrimitive->setReactionType(UMovePrimitive::Slide);
		entity.MovePrimitive->setTriggerType(UMovePrimitive::NotATrigger);
		entity.MovePrimitive->setCollisionMask(CEntityOld::OtherCollisionBit | CEntityOld::SelfCollisionBit | CEntityOld::SnowballCollisionBit);
		entity.MovePrimitive->setOcclusionMask(CEntityOld::OtherCollisionBit);
		entity.MovePrimitive->setObstacle(true);
		entity.MovePrimitive->setRadius(1.0f);
		entity.MovePrimitive->setHeight(1.8f);
		entity.MovePrimitive->insertInWorldImage(0);
		entity.MovePrimitive->setGlobalPosition(CVectorD(startPosition.x, startPosition.y, startPosition.z), 0);

		entity.Instance = _Scene->createInstance("gnu.shape");
		entity.Skeleton = _Scene->createSkeleton("gnu.skel");
		entity.Skeleton.bindSkin(entity.Instance);
		entity.Skeleton.enableCastShadowMap(true);
		entity.Instance.hide();

		entity.Speed = PlayerSpeed;
		entity.Particule = _Scene->createInstance("appear.ps");
		entity.setState(CEntityOld::Appear, gt);
		_Animation->playAnimation(entity, CEntityOld::LogInAnim);
		_Animation->playAnimation(entity, CEntityOld::IdleAnim);

		break;
	case CEntityOld::Snowball:
		entity.MovePrimitive = NULL;

		// allows collision snapping to the ceiling
		entity.VisualCollisionEntity->setCeilMode(true);

		entity.Instance = _Scene->createInstance("snowball.shape");
		entity.Skeleton = NULL;
		entity.Speed = SnowballSpeed;

		#pragma SBCLIENT_WARNING("SBCLIENT_SOUND")
		//playSound (entity, SoundId);
		entity.setState(CEntityOld::Normal, gt);
		break;
	}

	if (!entity.Skeleton.empty())
		entity.Skeleton.setPos(startPosition);

	entity.Instance.setPos(startPosition);

	#pragma SBCLIENT_WARNING("SBCLIENT_SOUND")
	//if (entity.Source != NULL)
	//	entity.Source->setPosition(startPosition);

	if (!entity.Particule.empty())
		entity.Particule.setPos(startPosition);
}

void CEntitiesOld::removeEntity(uint32 eid)
{
	nlinfo ("removing entity %u", eid);
	TGlobalAnimationTime gt = *_GlobalAnimationTime;
	
	// look for the entity
	CEntityOld *entity = getEntityPtr(eid);
	if (!entity)
	{
		nlwarning("CEntitiesOld::removeEntity: Entity %u not found", eid);
		return;
	}
	
	// if there is a particle system linked, delete it
	if (!entity->Particule.empty())
	{
		_Scene->deleteInstance(entity->Particule);
		entity->Particule = NULL;
	}
	
	entity->Particule = _Scene->createInstance("disappear.ps");
	entity->Particule.setPos(entity->Position);
	
	_Animation->playAnimation(*entity, CEntityOld::LogOffAnim, true);
	entity->setState(CEntityOld::Disappear, gt);
}

void CEntitiesOld::deleteEntity(CEntityOld &entity)
{
	if (!entity.Particule.empty())
	{
		_Scene->deleteInstance(entity.Particule);
		entity.Particule = NULL;
	}

	_Animation->deleteAnimation(entity);

	if (!entity.Skeleton.empty())
	{
		entity.Skeleton.detachSkeletonSon(entity.Instance);
		_Scene->deleteSkeleton(entity.Skeleton);
		entity.Skeleton = NULL;
	}

	if (!entity.Instance.empty())
	{
		_Scene->deleteInstance(entity.Instance);
		entity.Instance = NULL;
	}

	if (entity.VisualCollisionEntity != NULL)
	{
		_VisualCollisionManager->deleteEntity(entity.VisualCollisionEntity);
		entity.VisualCollisionEntity = NULL;
	}

	if (entity.MovePrimitive != NULL)
	{
		_MoveContainer->removePrimitive(entity.MovePrimitive);
		entity.MovePrimitive = NULL;
	}

	#pragma SBCLIENT_WARNING("SBCLIENT_SOUND")
	//deleteSound (entity);

	nlinfo("Remove the entity %u from the Entities list", entity.Id);
	CEntityMap::iterator eit = Entities.find(entity.Id);
	nlassert(eit != Entities.end());
	Entities.erase(eit);
}

void CEntitiesOld::removeAll()
{
	CEntityMap::iterator eit, nexteit;
	for (eit = Entities.begin(); eit != Entities.end(); )
	{
		nexteit = eit; nexteit++;
		CEntityOld &entity = (*eit).second;
		deleteEntity(entity);
		eit = nexteit;
	}
	Self = NULL;
	LastEID = 1000000;
}

void CEntitiesOld::stateAppear(CEntityOld &entity)
{
	TGlobalAnimationTime gt = *_GlobalAnimationTime;

	// after 1 second, show the instance
	if (gt > entity.StateStartTime + 1.0)
	{
		if (entity.Instance.getVisibility() != UTransform::Show)
			entity.Instance.show();
	}

	// after 3 seconds, delete the particle system (if any)
	// and pass the entity into the Normal state
	if (gt > entity.StateStartTime + 3.0)
	{
		if (!entity.Particule.empty())
		{
			_Scene->deleteInstance(entity.Particule);
			entity.Particule = NULL;
		}
		
		entity.setState(CEntityOld::Normal, gt);
	}

	if (entity.MovePrimitive != NULL)
		entity.MovePrimitive->move(CVector(0,0,0), 0);
}

void CEntitiesOld::stateDisappear(CEntityOld &entity)
{
	TGlobalAnimationTime gt = *_GlobalAnimationTime;

	// after 1 second, remove the mesh and all collision stuff
	if (gt > entity.StateStartTime + 1.0)
	{
		if (entity.Instance.getVisibility() != UTransform::Hide)
		{
			if (entity.Type == CEntityOld::Self)
			{
				if (!Self) nlerror("Self entity doesn't exist");
				Self = NULL;
			}

			entity.Instance.hide();
		}
	}

	// after 5 seconds, remove the particle system and the entity entry
	if (gt > entity.StateStartTime + 3.0)
	{
		deleteEntity(entity);
	}
	else
	{
		if (entity.MovePrimitive != NULL)
			entity.MovePrimitive->move(CVector(0,0,0), 0);
	}
}

void CEntitiesOld::stateNormal(CEntityOld &entity)
{
	#pragma SBCLIENT_WARNING("SBCLIENT_TIME")
	TGlobalAnimationTime dt = *_GlobalAnimationTimeDelta;
	TGlobalAnimationTime gt = *_GlobalAnimationTime;
	CVector	oldPos;
	CVector	newPos;

	oldPos = entity.Position;
	CVector	pDelta = entity.Position - entity.ServerPosition;
	CVector	pDeltaOri = pDelta;
	pDelta.z = 0.0f;

	// -- -- simple random bots =)  share with server

	// find a new random server position
	if (entity.Type == CEntityOld::Other && entity.AutoMove)
	{
		switch (entity.BotState)
		{
		case 0:
			// choose something to do
			if (frand(1.0f) > 0.5f)
				entity.BotState = 5;
			else
				entity.BotState = 2;
			break;
		case 1:
			// walk
			if (pDelta.norm() < 0.1f || gt - entity.BotStateStart > 3.0)
			{
				// reached the point
				entity.BotState = 0;
			}
			else
			{
//				entity.IsWalking = true;
//				entity.IsAiming = false;
			}
			break;
		case 2:
			// aim
			entity.IsWalking = false;
			entity.IsAiming = true;
			entity.BotStateStart = gt;
			entity.BotState = 3;
			break;
		case 3:
			// wait to shoot
			entity.IsWalking = false;
			entity.IsAiming = true;
			if (gt - entity.BotStateStart > 1.0)
			{
				entity.BotState = 4;
				entity.BotStateStart = gt;
				CVector	AimingPosition = entity.Position + CVector(0.0f, 0.0f, 2.0f);
				CVector	direction = CVector((float)(cos(entity.Angle)), (float)(sin(entity.Angle)), 0.3f).normed();
				#pragma SBCLIENT_WARNING("SBCLIENT_LANDSCAPE")
				//CVector AimedTarget = getTarget(AimingPosition, direction, 100);
				//shotSnowball(++LastEID, entity.Id, AimingPosition, AimedTarget, SnowballSpeed, 3.0f);
			}
			break;
		case 4:
			// shoot
			entity.IsWalking = false;
			entity.IsAiming = false;
			if (gt - entity.BotStateStart > 1.0)
			{
				entity.BotState = 5;
				entity.BotStateStart = gt;
			}
			break;
		case 5:
			// choose a direction
			float EntityMaxSpeed = 10.0f;
			entity.AuxiliaryAngle += frand(1.5f)-0.75f;
			entity.ServerPosition += CVector((float)cos(entity.AuxiliaryAngle),
											 (float)sin(entity.AuxiliaryAngle),
											 0.0f)*EntityMaxSpeed;
			entity.BotState = 1;
			entity.BotStateStart = gt;
			break;
		}
	}


	if (entity.Type == CEntityOld::Snowball && gt >= entity.Trajectory.getStopTime())
	{
/*
		CVector	tp(1140,-833,30);
		nlinfo("dist=%f", (entity.Position-tp).norm());
		if ((entity.Position-tp).norm()<30.0f)
		{
			static UInstance t = NULL;
			if (t != NULL)
			{
				Scene->deleteInstance (t);
			}
			t = Scene->createInstance("pills.ps");
			t->setScale (10,10,10);
			CVector tp2 = tp;
			tp2.z+=20;
			t->setPos (tp2);
			nlinfo("touche");
		}
*/
		removeEntity(entity.Id);
	}



	// control the character animation
	if (entity.Type != CEntityOld::Snowball)
	{
		if (entity.IsAiming && !entity.WasAiming)
		{
			// start aiming
			_Animation->playAnimation(entity, CEntityOld::PrepareSnowBall, true);
			_Animation->playAnimation(entity, CEntityOld::PrepareSnowBallCycle, false);
		}
		else if (entity.WasAiming && !entity.IsAiming)
		{
/*			// end aiming
			playAnimation (entity, ThrowSnowball, true);

			if (entity.IsWalking)
				playAnimation (entity, WalkAnim);
			else
				playAnimation (entity, IdleAnim);
*/		}
		else if (entity.WasAiming && entity.IsAiming)
		{
			// currently aiming => do northing
		}
		else if (!entity.WasWalking && entity.IsWalking)
		{
			_Animation->playAnimation(entity, CEntityOld::PrepareWalkAnim, true);
			_Animation->playAnimation(entity, CEntityOld::WalkAnim);
		}
		else if (entity.WasWalking && !entity.IsWalking)
		{
			_Animation->playAnimation(entity, CEntityOld::IdleAnim, true);
		}

		entity.WasAiming = entity.IsAiming;
		entity.WasWalking = entity.IsWalking;
	}


	entity.ImmediateSpeed = CVector::Null;

	if (entity.Type == CEntityOld::Self)
	{
		// get new position
		//#pragma SBCLIENT_WARNING("SBCLIENT_MOUSE_LISTENER")
		UGlobalPosition	gPos;
		entity.MovePrimitive->getGlobalPosition(gPos, 0);
		newPos = _GlobalRetriever->getGlobalPosition(gPos);
		//newPos = MouseListener->getPosition();
		// get new orientation
		#pragma SBCLIENT_WARNING("SBCLIENT_MOUSE_LISTENER")
		//entity.Angle = MouseListener->getOrientation();

		// Interpolate the character orientation towards the server angle
		// for smoother movements
		float sweepDistance = entity.AuxiliaryAngle - entity.InterpolatedAuxiliaryAngle;
		if (sweepDistance > (float)Pi)
		{
			sweepDistance -= (float)Pi * 2.0f;
			entity.InterpolatedAuxiliaryAngle += (float)Pi * 2.0f;
		}
		if (sweepDistance < -(float)Pi)
		{
			sweepDistance += (float)Pi * 2.0f;
			entity.InterpolatedAuxiliaryAngle -= (float)Pi * 2.0f;
		}
		float sweepAngle = 4.0f * sweepDistance;
		entity.InterpolatedAuxiliaryAngle += (float)(sweepAngle * dt);
		if ((entity.AuxiliaryAngle - entity.InterpolatedAuxiliaryAngle) * sweepAngle <= 0.0)
			entity.InterpolatedAuxiliaryAngle = entity.AuxiliaryAngle;
		entity.Angle += entity.InterpolatedAuxiliaryAngle;

		// tell the move container how much the entity should move
		// move container tells us now
	/*	if (entity.IsWalking)
		{
			entity.ImmediateSpeed = (newPos-oldPos)/(float)dt;
			if (_TestCLS) entity.MovePrimitive->setGlobalPosition(newPos, 0);
			else entity.MovePrimitive->move(entity.ImmediateSpeed, 0);
		}*/
	}
	else if (entity.Type == CEntityOld::Other)
	{
		// go to the server position with linear interpolation
		// -- -- useful for speed limiting on frontend service
		// -- -- random note: also, get rid of the position service, 
		//       and move the snowball physics to a more useful service

		// Interpolate orientation for smoother motions
		// AuxiliaryAngle -> the server imposed angle
		// InterpolatedAuxiliaryAngle -> the angle showed by the entity
		float sweepDistance = entity.AuxiliaryAngle-entity.InterpolatedAuxiliaryAngle;
		if (sweepDistance > (float)Pi)
		{
			sweepDistance -= (float)Pi*2.0f;
			entity.InterpolatedAuxiliaryAngle += (float)Pi*2.0f;
		}
		if (sweepDistance < -(float)Pi)
		{
			sweepDistance += (float)Pi*2.0f;
			entity.InterpolatedAuxiliaryAngle -= (float)Pi*2.0f;
		}
		float	sweepAngle = 4.0f*sweepDistance;
		entity.InterpolatedAuxiliaryAngle += (float)(sweepAngle*dt);
		if ((entity.AuxiliaryAngle-entity.InterpolatedAuxiliaryAngle)*sweepAngle <= 0.0)
			entity.InterpolatedAuxiliaryAngle = entity.AuxiliaryAngle;

		entity.Angle = entity.InterpolatedAuxiliaryAngle;

//		if (entity.IsWalking)
		if (pDelta.norm() > 0.1f)
		{
			pDelta.normalize();
			entity.ImmediateSpeed = pDelta*(-entity.Speed);
			entity.MovePrimitive->move(entity.ImmediateSpeed, 0);
			entity.IsWalking = true;
		}
		else
		{
			entity.IsWalking = false;
		}
	}
	else if (entity.Type == CEntityOld::Snowball)
	{
		// go to the server position using trajectory interpolation
		CVector newPos = entity.Trajectory.eval(gt);
		if (newPos != entity.Position)
		{
			entity.Position = entity.Trajectory.eval(gt);
			entity.Instance.show ();
		}
	}
	else
	{
		// automatic speed
		newPos = oldPos;
	}
}

//////void CEntitiesOld::resetEntityPosition(uint32 eid)
//////{
//////	uint32 sbid = ++LastEID;
//////	EIT eit = findEntity (eid);
//////
//////	CEntity	&entity = (*eit).second;
//////
//////	UGlobalPosition	gPos;
//////	// get the global position
//////	gPos = GlobalRetriever->retrievePosition(entity.Position);
//////	// convert it in a vector 3d
//////	entity.Position = GlobalRetriever->getGlobalPosition(gPos);
//////	// get the good z position
//////	gPos.LocalPosition.Estimation.z = 0.0f;
//////	entity.Position.z = GlobalRetriever->getMeanHeight(gPos);
//////
//////	// snap to the ground
//////	if (entity.VisualCollisionEntity != NULL && !GlobalRetriever->isInterior(gPos))
//////		entity.VisualCollisionEntity->snapToGround(entity.Position);
//////
//////	if (entity.MovePrimitive != NULL)
//////		entity.MovePrimitive->setGlobalPosition(CVector(entity.Position.x, entity.Position.y, entity.Position.z), 0);
//////}

void CEntitiesOld::shotSnowball(uint32 sid, uint32 eid, const CVector &start, const CVector &target, float speed, float deflagRadius)
{
	TGlobalAnimationTime gt = *_GlobalAnimationTime;

	// get direction
	CVector direction = (target - start).normed();

	// create a new snowball entity
	addEntity(sid, ucstring(), CEntityOld::Snowball, start, target);
	CEntityOld &snowball = getEntity(sid);

	snowball.AutoMove = 1;
	snowball.Trajectory.init(start, target, speed, gt + 1.0);
	snowball.Instance.hide();

	CEntityOld	&entity = getEntity(eid);

	// end aiming
	_Animation->playAnimation(entity, CEntityOld::ThrowSnowball, true);

	if (entity.IsWalking)
	{
		_Animation->playAnimation(entity, CEntityOld::PrepareWalkAnim, true);
		_Animation->playAnimation(entity, CEntityOld::WalkAnim);
	}
	else
		_Animation->playAnimation(entity, CEntityOld::IdleAnim);
}

SBCLIENT_CALLBACK_IMPL(CEntitiesOld, updateEntities)
{
	// compute the delta t that has elapsed since the last update (in seconds)
	#pragma SBCLIENT_WARNING("SBCLIENT_TIME")
	TGlobalAnimationTime dt = *_GlobalAnimationTimeDelta;
	TGlobalAnimationTime gt = *_GlobalAnimationTime;
	CEntityMap::iterator eit;

	// move entities
	for (eit = Entities.begin(); eit != Entities.end(); eit++)
	{
		CEntityOld &entity = (*eit).second;

		switch (entity.State)
		{
		case CEntityOld::Appear:
			stateAppear(entity);
			break;
		case CEntityOld::Normal:
			stateNormal(entity);
			break;
		case CEntityOld::Disappear:
			stateDisappear(entity);
			break;
		default:
			nlstop;
			break;
		}
	}

	// WHAT ? WHAT ? WHAT ? :p
	// evaluate collisions
	_MoveContainer->evalCollision(dt, 0);
	#pragma SBCLIENT_WARNING("SBCLIENT_PACS")
	// we will only check collisions on self

	// snap entities to the ground
	for (eit = Entities.begin (); eit != Entities.end (); eit++)
	{
		CEntityOld	&entity = (*eit).second;
		UGlobalPosition	gPos;

		if (entity.MovePrimitive != NULL)
		{
			// get the global position in pacs coordinates system
			entity.MovePrimitive->getGlobalPosition(gPos, 0);
			// convert it in a vector 3d
			entity.Position = _GlobalRetriever->getGlobalPosition(gPos);
			// get the good z position
			gPos.LocalPosition.Estimation.z = 0.0f;
			entity.Position.z = _GlobalRetriever->getMeanHeight(gPos);

			// check position retrieving
/*
			UGlobalPosition gPosCheck;
			gPosCheck = GlobalRetriever->retrievePosition(entity.Position);
			if (gPos.InstanceId != gPosCheck.InstanceId ||
				gPos.LocalPosition.Surface != gPosCheck.LocalPosition.Surface)
			{
				nlwarning("Checked UGlobalPosition differs from store");
//				gPos.InstanceId = gPosCheck.InstanceId;
//				gPos.LocalPosition.Surface = gPosCheck.LocalPosition.Surface;
			}
*/
			// snap to the ground
			if (!_GlobalRetriever->isInterior(gPos))
				entity.VisualCollisionEntity->snapToGround(entity.Position);

			if (entity.Type == CEntityOld::Other &&
				(entity.ServerPosition - entity.Position) * entity.ImmediateSpeed < 0.0f)
			{
				//nlinfo("detected over entity %d", entity.Id);
				entity.ServerPosition.z = entity.Position.z;
				entity.Position = entity.ServerPosition;
				if (!_GlobalRetriever->isInterior(gPos))
					entity.VisualCollisionEntity->snapToGround(entity.Position);
				entity.MovePrimitive->setGlobalPosition(CVectorD(entity.Position.x, entity.Position.y, entity.Position.z), 0);
			}

		}

		if (!entity.Instance.empty())
		{
			CVector	jdir;
			switch (entity.Type)
			{
			case CEntityOld::Self:
				jdir = CVector(-(float)cos(entity.Angle), -(float)sin(entity.Angle), 0.0f);
				break;
			case CEntityOld::Other:
				jdir = CVector(-(float)cos(entity.Angle), -(float)sin(entity.Angle), 0.0f);
				break;
			case CEntityOld::Snowball:
				jdir = entity.Trajectory.evalSpeed(gt).normed();
				break;
			}

			if (!entity.Skeleton.empty())
			{
				entity.Skeleton.setPos(entity.Position);
				entity.Skeleton.setRotQuat(jdir);
			}

			entity.Instance.setPos(entity.Position);
			entity.Instance.setRotQuat(jdir);
		}

		#pragma SBCLIENT_WARNING("SBCLIENT_SOUND")
		//if (entity.Source != NULL)
		//	entity.Source->setPosition (entity.Position);

		if (!entity.Particule.empty())
		{
			entity.Particule.setPos(entity.Position);
		}

		if (entity.Type == CEntityOld::Self)
		{
			#pragma SBCLIENT_WARNING("SBCLIENT_MOUSE_LISTENER")
			//MouseListener->setPosition(entity.Position);
		}
	}
}

} /* namespace SBCLIENT */

/* end of file */
