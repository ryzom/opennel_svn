/**
 * \file animation_old.cpp
 * \brief CAnimationOld
 * \date 2008-02-07 11:20GMT
 * \author Nevrax Ltd.
 * Animation interface between the game and NeL
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
#include "animation_old.h"

// Project includes
#include "member_callback_impl.h"
#include "entities_old.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/misc/progress_callback.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_animation_set.h>
#include <nel/3d/u_play_list_manager.h>
#include <nel/3d/u_play_list.h>
#include <nel/3d/u_animation.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NL3D;

//#define SBCLIENT_ANIMATIONS_DISABLED
//#define SBCLIENT_ANIMATIONS_ORIGINAL

namespace SBCLIENT {

#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
enum EAnim { NoAnim = -1, PrepareWalkAnim, WalkAnim, IdleAnim, LogInAnim, LogOffAnim, ThrowSnowball, PrepareSnowBall, PrepareSnowBallCycle, HitAnim };
typedef std::map<uint32, CEntityOld>::iterator	EIT;

UAnimationSet *AnimationSet = NULL;
UPlayListManager *PlayListManager = NULL;

struct Anim
{
	char		*Name;
	bool		 Loop;
	uint		 Id;
	UAnimation	*Animation;
};

Anim AnimIdArray[][2] =
{
	{ { "patterfeet.anim", false, 0, NULL }, { "", false, 0, NULL } },
	{ { "marche.anim", true, 0, NULL }, { "", false, 0, NULL } },
	{ { "idle.anim", true, 0, NULL }, { "", false, 0, NULL } },
	{ { "log_on.anim", false, 0, NULL }, { "", false, 0, NULL } },
	{ { "log_off.anim", false, 0, NULL }, { "", false, 0, NULL } },
	{ { "lancelaboule.anim", false, 0, NULL }, { "", false, 0, NULL } },
	{ { "prepaboule.anim", false, 0, NULL }, { "", false, 0, NULL } },
	{ { "prepaboulecycle.anim", true, 0, NULL }, { "", false, 0, NULL } },
	{ { "impact.anim", false, 0, NULL }, { "", false, 0, NULL } },
};
#endif

const NL3D::TGlobalAnimationTime CAnimationOld::TransitionTime = 0.25;

CAnimationOld::CAnimationOld(NLMISC::IProgressCallback &progressCallback, 
NL3D::UDriver *driver, NL3D::UScene *scene, 
TGlobalAnimationTime *globalAnimationTime) : _AnimationSet(NULL), 
_PlayListManager(NULL), _Driver(driver), _Scene(scene),
_GlobalAnimationTime(globalAnimationTime)
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	nlassert(_Driver);
	nlassert(_Scene);
	nlassert(_GlobalAnimationTime);

	progressCallback.progress(0.00f);
	memset(_AnimIdArray, 0, sizeof(_AnimIdArray));
	_AnimIdArray[0].Name = "patterfeet.anim";
	_AnimIdArray[0].Loop = false;
	_AnimIdArray[1].Name = "marche.anim";
	_AnimIdArray[1].Loop = false;
	_AnimIdArray[2].Name = "idle.anim";
	_AnimIdArray[2].Loop = true;
	_AnimIdArray[3].Name = "log_on.anim";
	_AnimIdArray[3].Loop = false;
	_AnimIdArray[4].Name = "log_off.anim";
	_AnimIdArray[4].Loop = false;
	_AnimIdArray[5].Name = "lancelaboule.anim";
	_AnimIdArray[5].Loop = false;
	_AnimIdArray[6].Name = "prepaboule.anim";
	_AnimIdArray[6].Loop = false;
	_AnimIdArray[7].Name = "prepaboulecycle.anim";
	_AnimIdArray[7].Loop = true;
	_AnimIdArray[8].Name = "impact.anim";
	_AnimIdArray[8].Loop = false;

	_AnimationSet = _Driver->createAnimationSet();
	progressCallback.progress(0.30f);
	// Add all animations in the animation set
	for (uint i = 0; i < sizeof (_AnimIdArray) / sizeof (_AnimIdArray[0]); i++)
	{
		if (_AnimIdArray[i].Name[0] != '\0')
		{
			_AnimIdArray[i].Id = _AnimationSet->addAnimation(
				_AnimIdArray[i].Name, _AnimIdArray[i].Name);
			_AnimIdArray[i].Animation = _AnimationSet->getAnimation(
				_AnimIdArray[i].Id);
		}
	}
	_AnimationSet->build();	
	progressCallback.progress(0.60f);

	_PlayListManager = _Scene->createPlayListManager();
	progressCallback.progress(1.00f);
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	AnimationSet = _Driver->createAnimationSet ();
	
	// Add all animations in the animation set
	for (uint i = 0; i < sizeof (AnimIdArray) / sizeof (AnimIdArray[0]); i++)
	{
		if (AnimIdArray[i][0].Name[0] != '\0')
		{
			AnimIdArray[i][0].Id = AnimationSet->addAnimation (AnimIdArray[i][0].Name, AnimIdArray[i][0].Name);
			AnimIdArray[i][0].Animation = AnimationSet->getAnimation (AnimIdArray[i][0].Id);
		}

		if (AnimIdArray[i][1].Name[0] != '\0')
		{
			AnimIdArray[i][1].Id = AnimationSet->addAnimation (AnimIdArray[i][1].Name, AnimIdArray[i][1].Name);
			AnimIdArray[i][1].Animation = AnimationSet->getAnimation (AnimIdArray[i][1].Id);
		}
	}
	AnimationSet->build ();

	PlayListManager = _Scene->createPlayListManager ();
#endif
}

CAnimationOld::~CAnimationOld()
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	nlassert(_PlayListManager);
	_Scene->deletePlayListManager(_PlayListManager);

//// The next line doesn t work (say that AnimationSet is not a valid AnimationSet Ptr) so we comment it.
////	_Scene->deleteAnimationSet(_AnimationSet);
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	_Scene->deletePlayListManager (PlayListManager);

// The next line doesn t work (say that AnimationSet is not a valid AnimationSet Ptr) so we comment it.
//	Scene->deleteAnimationSet (AnimationSet);
#endif
}

void CAnimationOld::playAnimation(CEntityOld &entity, CEntityOld::TAnim anim, bool force)
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	// Get the current time
	TGlobalAnimationTime gt = *_GlobalAnimationTime;

	nlassert(anim > -2 && anim < 20);
	nlinfo("playAnimation() %d at %lf", anim, gt);

	// Can't do animation on entity without skeleton
	if (entity.Skeleton.empty()) 
	{
		nlwarning("Can't do animation on entity %u '%s' without skeleton", entity.Id, entity.Name.toUtf8().c_str());
		return;
	}

	// If the first time we play an animation, creates the animation class
	if (!entity.PlayList) createAnimation(entity);

	if (force || entity.AnimQueue.empty())
	{
		computeAnimation(entity, anim);
		
		// clear the animation queue
		nlinfo("clearing animation queue");
		while (!entity.AnimQueue.empty())
			entity.AnimQueue.pop();
	}

	nlinfo("pushing animation %d", anim);
	nlinfo("pushing animation %s", _AnimIdArray[anim].Name);
	entity.AnimQueue.push(anim);
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	nlassert (anim > -2 && anim < 20);
//	nlinfo ("playAnimation() %d", anim);

	// Get the current time
	CAnimationTime currentTime = CAnimationTime(CTime::getLocalTime ())/1000.0f;

	// Can't do animation without skeleton
	if (entity.Skeleton.empty())
		return;

	// If the first time we play an animation, creates the animation class
	if (entity.PlayList == NULL)
		createAnimation (entity);

	if (force || entity.AnimQueue.empty())
	{
		computeAnimation (entity, anim);
		
		// clear the animation queue
		//nlinfo ("clearing animation queue");
		while (!entity.AnimQueue.empty())
			entity.AnimQueue.pop ();
	}

//	nlinfo ("pushing animation %d", anim);
//	nlinfo ("pushing animation %s", AnimIdArray[anim][0].Name);
	entity.AnimQueue.push (anim);
#endif
}

void CAnimationOld::createAnimation(SBCLIENT::CEntityOld &entity)
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	nlassert(!entity.PlayList);
	nlassert(!entity.Instance.empty());
	nlassert(!entity.Skeleton.empty());
	nlassert(_AnimationSet);

	entity.PlayList = _PlayListManager->createPlayList(_AnimationSet);
	entity.PlayList->registerTransform(entity.Instance);
	entity.PlayList->registerTransform(entity.Skeleton);
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	nlassert (!entity.Instance.empty() && !entity.Skeleton.empty() && AnimationSet != NULL);

	entity.PlayList = PlayListManager->createPlayList (AnimationSet);
	entity.PlayList->registerTransform (entity.Instance);
	entity.PlayList->registerTransform (entity.Skeleton);
#endif
}

void CAnimationOld::deleteAnimation(SBCLIENT::CEntityOld &entity)
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	if (entity.PlayList == NULL) 
	{
		nlwarning("Entity %s does not have a playlist", entity.Name.toUtf8().c_str());
		return;
	}

	_PlayListManager->deletePlayList(entity.PlayList);
	entity.PlayList = NULL;
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	if (entity.PlayList == NULL)
		return;

	PlayListManager->deletePlayList (entity.PlayList);
	entity.PlayList= NULL;
#endif
}

SBCLIENT_CALLBACK_IMPL(CAnimationOld, updateAnimations)
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	// Get the current time
	TGlobalAnimationTime gt = *_GlobalAnimationTime;

	CEntitiesOld::CEntityMap::iterator
		eit(_Entities->Entities.begin()),
		end(_Entities->Entities.end());
	for (; eit != end; ++eit)
	{
		CEntityOld &entity = eit->second;

		if (entity.AnimQueue.empty())
		{
			nlwarning("empty queue update!!!");
			continue;
		}

		CEntityOld::TAnim currentAnim = entity.AnimQueue.front();
		if (!_AnimIdArray[currentAnim].Loop 
			&& gt >= entity.StartAnimationTime 
			+ _AnimIdArray[currentAnim].Animation->getEndTime() 
			- TransitionTime / 2)
		{
			// remove the current anim
			nlinfo("remove the current anim");
			entity.AnimQueue.pop();

			if (entity.AnimQueue.empty())
			{
				nlwarning("empty queue!!!!!!");
				continue;
			}

			CEntityOld::TAnim newAnim = entity.AnimQueue.front();

#if 1
			computeAnimation(entity, newAnim);
#else
			nlinfo("playing animation %s ct%f st%f et%f", 
				_AnimIdArray[newAnim].Name, gt, 
				_AnimIdArray[newAnim].Animation->getBeginTime(), 
				_AnimIdArray[newAnim].Animation->getEndTime());
			// setup the new anim
			entity.PlayList->setAnimation(0, _AnimIdArray[newAnim].Id);
			entity.PlayList->setTimeOrigin(0, gt);
			entity.PlayList->setStartWeight(0, 1.0f, gt);
			entity.PlayList->setEndWeight(0, 1.0f, gt + TransitionTime);
			entity.PlayList->setWeightSmoothness(0, 1.0f);

			if (_AnimIdArray[newAnim].Loop)
				entity.PlayList->setWrapMode(0, UPlayList::Repeat);
			else
				entity.PlayList->setWrapMode(0, UPlayList::Clamp);

			entity.StartAnimationTime = gt;
#endif
		}
	}

	// compute new animation position depending of the current time
	_PlayListManager->animate(gt);
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	// Get the current time
	CAnimationTime currentTime = CAnimationTime(CTime::getLocalTime ())/1000.0f;

	for (EIT eit = _Entities->Entities.begin (); eit != _Entities->Entities.end (); eit++)
	{
		CEntityOld	&entity = (*eit).second;

		if (entity.AnimQueue.empty ())
		{
//			nlwarning ("empty queue update!!!");
			continue;
		}

		EAnim currentAnim = (EAnim)entity.AnimQueue.front ();
		if (!AnimIdArray[currentAnim][0].Loop && currentTime >= entity.StartAnimationTime + AnimIdArray[currentAnim][0].Animation->getEndTime () - TransitionTime/2)
		{
			// remove the current anim
			entity.AnimQueue.pop ();

			if (entity.AnimQueue.empty ())
			{
//				nlwarning ("empty queue!!!!!!");
				continue;
			}

			EAnim newAnim = (EAnim)entity.AnimQueue.front ();

			computeAnimation (entity, (CEntityOld::TAnim)newAnim);
/*
			nlinfo ("playing animation %s ct%f st%f et%f", AnimIdArray[newAnim][0].Name, currentTime, AnimIdArray[newAnim][0].Animation->getBeginTime (), AnimIdArray[newAnim][0].Animation->getEndTime ());
			// setup the new anim
			entity.PlayList->setAnimation (0, AnimIdArray[newAnim][0].Id);
			entity.PlayList->setTimeOrigin (0, currentTime);
			entity.PlayList->setStartWeight (0, 1.0f, currentTime);
			entity.PlayList->setEndWeight (0, 1.0f, currentTime+TransitionTime);
			entity.PlayList->setWeightSmoothness (0, 1.0f);

			if (AnimIdArray[newAnim][0].Loop)
				entity.PlayList->setWrapMode (0, UPlayList::Repeat);
			else
				entity.PlayList->setWrapMode (0, UPlayList::Clamp);

			entity.StartAnimationTime = currentTime;
*/		}
	}

	// compute new animation position depending of the current time
	PlayListManager->animate (double(CTime::getLocalTime ())/1000.0f);
#endif
}

void CAnimationOld::computeAnimation(CEntityOld &entity, CEntityOld::TAnim anim)
{
#ifndef SBCLIENT_ANIMATIONS_DISABLED
	// Get the current time
	TGlobalAnimationTime gt = *_GlobalAnimationTime;
	
	nlinfo("playing animation t(%d) id(%u) name(%s) ct(%lf) st(%f) et(%f)", 
		anim, 
		_AnimIdArray[anim].Id, 
		_AnimIdArray[anim].Name, gt, 
		_AnimIdArray[anim].Animation->getBeginTime(), 
		_AnimIdArray[anim].Animation->getEndTime());
	
	// Find the new slot for the full animation (0 or 1)
	uint newSlot = entity.NextEmptySlot;
	uint oldSlot = 1 - entity.NextEmptySlot;
	entity.NextEmptySlot = oldSlot;
	
	UPlayList::TWrapMode wrapMode = _AnimIdArray[anim].Loop 
		? UPlayList::Repeat : UPlayList::Clamp;
	
	entity.PlayList->setAnimation(newSlot, _AnimIdArray[anim].Id);
	entity.PlayList->setTimeOrigin(newSlot, gt);
	entity.PlayList->setWeightSmoothness(newSlot, 1.0f);
	entity.PlayList->setWrapMode(newSlot, wrapMode);

	TGlobalAnimationTime OldStartTime, OldEndTime;
	TGlobalAnimationTime NewStartTime, NewEndTime;
	
	// Get the starting time of the old animation slot
	entity.PlayList->getStartWeight(oldSlot, OldStartTime);
	
	// Compute the time delta between start of the old animation and now
	TGlobalAnimationTime ani_dt = gt - OldStartTime;

	// Compute the new transition value depending of the current time
	if (ani_dt > TransitionTime)
		ani_dt = TransitionTime;

	OldStartTime = gt - (TransitionTime - ani_dt);
	OldEndTime = gt + ani_dt;
	
	NewStartTime = gt;
	NewEndTime = gt + ani_dt;
	
	nlinfo("old sw%lf ew%lf - new sw%lf ew%lf", 
		OldStartTime, OldEndTime, NewStartTime, NewEndTime);
	
	// Set new weights on the old and the new animation slot
	
	entity.PlayList->setStartWeight(oldSlot, 1.0f, OldStartTime);
	entity.PlayList->setEndWeight(oldSlot, 0.0f, OldEndTime);
	
	entity.PlayList->setStartWeight(newSlot, 0.0f, NewStartTime);
	entity.PlayList->setEndWeight(newSlot, 1.0f, OldEndTime);
	
	// Keep in mind when we set the animation
	entity.StartAnimationTime = gt;
#endif
#ifdef SBCLIENT_ANIMATIONS_ORIGINAL
	// Get the current time
	double currentTime = double (CTime::getLocalTime ())/1000.0f;

//	nlinfo ("%d playing animation", anim);
//	nlinfo ("%d playing animation %s ct%f st%f et%f", anim, AnimIdArray[anim][0].Name, currentTime, AnimIdArray[anim][0].Animation->getBeginTime (), AnimIdArray[anim][0].Animation->getEndTime ());

	// Find the new slot for the full animation (0 or 1)
	uint newSlot = entity.NextEmptySlot;
	uint oldSlot = 1 - entity.NextEmptySlot;
	entity.NextEmptySlot = 1 - entity.NextEmptySlot;

	UPlayList::TWrapMode wrapMode = AnimIdArray[anim][0].Loop ? UPlayList::Repeat : UPlayList::Clamp;
	
	entity.PlayList->setAnimation			(newSlot, AnimIdArray[anim][0].Id);
	entity.PlayList->setTimeOrigin			(newSlot, currentTime);
	entity.PlayList->setWeightSmoothness	(newSlot, 1.0f);
	entity.PlayList->setWrapMode			(newSlot, wrapMode);

	double OldStartTime, OldEndTime;
	double NewStartTime, NewEndTime;

	// Get the starting time of the old animation slot
	entity.PlayList->getStartWeight	(oldSlot, OldStartTime);
	
	// Compute the time delta between start of the old animation and now
	double dt = currentTime - OldStartTime;

	// Compute the new transition value depending of the current time

	if (dt > TransitionTime)
		dt = TransitionTime;

	OldStartTime = currentTime - (TransitionTime - dt);
	OldEndTime = currentTime + dt;
		
	NewStartTime = currentTime;
	NewEndTime = currentTime + dt;

	// Set new weights on the old and the new animation slot

	entity.PlayList->setStartWeight	(oldSlot, 1.0f, OldStartTime);
	entity.PlayList->setEndWeight	(oldSlot, 0.0f, OldEndTime);

	entity.PlayList->setStartWeight	(newSlot, 0.0f, NewStartTime);
	entity.PlayList->setEndWeight	(newSlot, 1.0f, OldEndTime);

	// Keep in mind what is the last animation id we set
	entity.StartAnimationTime = (float)currentTime;
#endif
}

} /* namespace SBCLIENT */

/* end of file */
