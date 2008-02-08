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

namespace SBCLIENT {

const NL3D::TGlobalAnimationTime CAnimationOld::TransitionTime = 0.25;

CAnimationOld::CAnimationOld(NLMISC::IProgressCallback &progressCallback, 
NL3D::UDriver *driver, NL3D::UScene *scene, 
TGlobalAnimationTime *globalAnimationTime) : _AnimationSet(NULL), 
_PlayListManager(NULL), _Driver(driver), _Scene(scene),
_GlobalAnimationTime(globalAnimationTime)
{
	nlassert(_Driver);
	nlassert(_Scene);
	nlassert(_GlobalAnimationTime);

	progressCallback.progress(0.00f);
	memset(_AnimIdArray, 0, sizeof(_AnimIdArray));
	_AnimIdArray[0][0].Name = "patterfeet.anim";
	_AnimIdArray[0][0].Loop = false;
	_AnimIdArray[0][1].Name = "";
	_AnimIdArray[1][0].Name = "marche.anim";
	_AnimIdArray[1][0].Loop = false;
	_AnimIdArray[1][1].Name = "";
	_AnimIdArray[2][0].Name = "idle.anim";
	_AnimIdArray[2][0].Loop = true;
	_AnimIdArray[2][1].Name = "";
	_AnimIdArray[3][0].Name = "log_on.anim";
	_AnimIdArray[3][0].Loop = false;
	_AnimIdArray[3][1].Name = "";
	_AnimIdArray[4][0].Name = "log_off.anim";
	_AnimIdArray[4][0].Loop = false;
	_AnimIdArray[4][1].Name = "";
	_AnimIdArray[5][0].Name = "lancelaboule.anim";
	_AnimIdArray[5][0].Loop = false;
	_AnimIdArray[5][1].Name = "";
	_AnimIdArray[6][0].Name = "prepaboule.anim";
	_AnimIdArray[6][0].Loop = false;
	_AnimIdArray[6][1].Name = "";
	_AnimIdArray[7][0].Name = "prepaboulecycle.anim";
	_AnimIdArray[7][0].Loop = true;
	_AnimIdArray[7][1].Name = "";
	_AnimIdArray[8][0].Name = "impact.anim";
	_AnimIdArray[8][0].Loop = false;
	_AnimIdArray[8][1].Name = "";

	_AnimationSet = _Driver->createAnimationSet();
	progressCallback.progress(0.30f);
	// Add all animations in the animation set
	for (uint i = 0; i < sizeof (_AnimIdArray) / sizeof (_AnimIdArray[0]); i++)
	{
		if (_AnimIdArray[i][0].Name[0] != '\0')
		{
			_AnimIdArray[i][0].Id = _AnimationSet->addAnimation(_AnimIdArray[i][0].Name, _AnimIdArray[i][0].Name);
			_AnimIdArray[i][0].Animation = _AnimationSet->getAnimation(_AnimIdArray[i][0].Id);
		}

		if (_AnimIdArray[i][1].Name[0] != '\0')
		{
			_AnimIdArray[i][1].Id = _AnimationSet->addAnimation(_AnimIdArray[i][1].Name, _AnimIdArray[i][1].Name);
			_AnimIdArray[i][1].Animation = _AnimationSet->getAnimation(_AnimIdArray[i][1].Id);
		}
	}
	_AnimationSet->build ();	
	progressCallback.progress(0.60f);

	_PlayListManager = _Scene->createPlayListManager();
	progressCallback.progress(1.00f);
}

CAnimationOld::~CAnimationOld()
{
	_Scene->deletePlayListManager(_PlayListManager);

// The next line doesn t work (say that AnimationSet is not a valid AnimationSet Ptr) so we comment it.
//	_Scene->deleteAnimationSet(_AnimationSet);
}

void CAnimationOld::playAnimation(CEntityOld &entity, CEntityOld::TAnim anim, bool force)
{
	nlassert(anim > -2 && anim < 20);
	nlinfo ("playAnimation() %d", anim);

	// Get the current time
	TGlobalAnimationTime currentTime = *_GlobalAnimationTime;

	// Can't do animation without skeleton
	if (entity.Skeleton.empty())
		return;

	// If the first time we play an animation, creates the animation class
	if (entity.PlayList == NULL)
		createAnimation(entity);

	if (force || entity.AnimQueue.empty())
	{
		computeAnimation(entity, anim);
		
		// clear the animation queue
		//nlinfo ("clearing animation queue");
		while (!entity.AnimQueue.empty())
			entity.AnimQueue.pop();
	}

	nlinfo("pushing animation %d", anim);
	nlinfo("pushing animation %s", _AnimIdArray[anim][0].Name);
	entity.AnimQueue.push (anim);
}

void CAnimationOld::createAnimation(SBCLIENT::CEntityOld &entity)
{
	nlassert(!entity.Instance.empty())
	nlassert(!entity.Skeleton.empty())
	nlassert(_AnimationSet);

	entity.PlayList = _PlayListManager->createPlayList(_AnimationSet);
	entity.PlayList->registerTransform(entity.Instance);
	entity.PlayList->registerTransform(entity.Skeleton);
}

void CAnimationOld::deleteAnimation(SBCLIENT::CEntityOld &entity)
{
	if (entity.PlayList == NULL)
		return;

	_PlayListManager->deletePlayList(entity.PlayList);
	entity.PlayList= NULL;
}

SBCLIENT_CALLBACK_IMPL(CAnimationOld, updateAnimations)
{
	// Get the current time
	TGlobalAnimationTime currentTime = *_GlobalAnimationTime;

	for (CEntitiesOld::CEntityMap::iterator eit = _Entities->Entities.begin(); eit != _Entities->Entities.end(); eit++)
	{
		CEntityOld &entity = (*eit).second;

		if (entity.AnimQueue.empty())
		{
			nlwarning ("empty queue update!!!");
			continue;
		}

		CEntityOld::TAnim currentAnim = entity.AnimQueue.front();
		if (!_AnimIdArray[currentAnim][0].Loop && currentTime >= entity.StartAnimationTime + _AnimIdArray[currentAnim][0].Animation->getEndTime() - TransitionTime / 2)
		{
			// remove the current anim
			entity.AnimQueue.pop();

			if (entity.AnimQueue.empty())
			{
				nlwarning ("empty queue!!!!!!");
				continue;
			}

			CEntityOld::TAnim newAnim = entity.AnimQueue.front();

			computeAnimation(entity, newAnim);
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
	_PlayListManager->animate(currentTime);
}

void CAnimationOld::computeAnimation(CEntityOld &entity, CEntityOld::TAnim anim)
{
	// Get the current time
	TGlobalAnimationTime currentTime = *_GlobalAnimationTime;

	nlinfo("%d playing animation", anim);
	nlinfo("%d playing animation %s ct%lf st%f et%f", anim, _AnimIdArray[anim][0].Name, currentTime, _AnimIdArray[anim][0].Animation->getBeginTime(), _AnimIdArray[anim][0].Animation->getEndTime());

	// Find the new slot for the full animation (0 or 1)
	uint newSlot = entity.NextEmptySlot;
	uint oldSlot = 1 - entity.NextEmptySlot;
	entity.NextEmptySlot = 1 - entity.NextEmptySlot;

	UPlayList::TWrapMode wrapMode = _AnimIdArray[anim][0].Loop ? UPlayList::Repeat : UPlayList::Clamp;
	
	entity.PlayList->setAnimation(newSlot, _AnimIdArray[anim][0].Id);
	entity.PlayList->setTimeOrigin(newSlot, currentTime);
	entity.PlayList->setWeightSmoothness(newSlot, 1.0f);
	entity.PlayList->setWrapMode(newSlot, wrapMode);

	double OldStartTime, OldEndTime;
	double NewStartTime, NewEndTime;

	// Get the starting time of the old animation slot
	entity.PlayList->getStartWeight(oldSlot, OldStartTime);
	
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

	entity.PlayList->setStartWeight(oldSlot, 1.0f, OldStartTime);
	entity.PlayList->setEndWeight(oldSlot, 0.0f, OldEndTime);

	entity.PlayList->setStartWeight(newSlot, 0.0f, NewStartTime);
	entity.PlayList->setEndWeight(newSlot, 1.0f, OldEndTime);

	// Keep in mind what is the last animation id we set
	entity.StartAnimationTime = currentTime;
}

} /* namespace SBCLIENT */

/* end of file */
