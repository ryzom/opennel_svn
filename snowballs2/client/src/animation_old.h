/**
 * \file animation_old.h
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

#ifndef SBCLIENT_ANIMATION_OLD_H
#define SBCLIENT_ANIMATION_OLD_H
#include <nel/misc/types_nl.h>

// Project includes
#include "member_callback_decl.h"
#include "entity_old.h"

// NeL includes
#include <nel/3d/animation_time.h>

// STL includes

namespace NLMISC {
	class IProgressCallback;
}

namespace NL3D {
	class UAnimation;
	class UAnimationSet;
	class UPlayListManager;
	class UDriver;
	class UScene;
}

namespace SBCLIENT {
	class CEntitiesOld;

/**
 * \brief CAnimationOld
 * \date 2008-02-07 11:20GMT
 * \author Nevrax Ltd.
 * Animation interface between the game and NeL
 */
class CAnimationOld
{
	friend class CEntitiesOld;
	struct _CAnim
	{
		char *Name;
		bool Loop;
		uint Id;
		NL3D::UAnimation *Animation;
	};
protected:
	// constants
	// Amount of time for the transistion between 2 animations
	static const NL3D::CAnimationTime TransitionTime;

	// pointers
	NL3D::UAnimationSet *_AnimationSet; // P
	NL3D::UPlayListManager *_PlayListManager; // P
	NL3D::UDriver *_Driver; // p
	NL3D::UScene *_Scene; // p
	CEntitiesOld *_Entities; // p
	
	// instances
	_CAnim _AnimIdArray[9][2];
public:
	CAnimationOld(NLMISC::IProgressCallback &progressCallback, NL3D::UDriver *driver, NL3D::UScene *scene);
	virtual ~CAnimationOld();

	/// This function is automatically called by playAnimation() the first time
	void createAnimation(CEntityOld &entity);

	/// Change the animation of an entity. You have to give the animation id
	void playAnimation(CEntityOld &entity, CEntityOld::TAnim anim, bool force = false);

	/// This function must be called before releasing the entity to delete the entity animation
	void deleteAnimation(CEntityOld &entity);

	SBCLIENT_CALLBACK_DECL(updateAnimations);

private:
	
	void computeAnimation(CEntityOld &entity, CEntityOld::TAnim anim);
}; /* class CAnimationOld */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_ANIMATION_OLD_H */

/* end of file */
