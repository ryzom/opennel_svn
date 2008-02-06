/**
 * \file sound.h
 * \brief CSound
 * \date 2008-02-03 20:52GMT
 * \author Jan Boon (Kaetemi)
 * CSound
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * 
 * This file is part of OpenNeL Snowballs.
 * OpenNeL Snowballs is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * OpenNeL Snowballs is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenNeL Snowballs; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_SOUND_H
#define SBCLIENT_SOUND_H
#include <nel/misc/types_nl.h>

#include "config_proxy.h"
#include "member_callback_decl.h"

#include <nel/misc/progress_callback.h>

namespace NLSOUND {
	class UAudioMixer;
}

namespace SBCLIENT {

/**
 * \brief CSound
 * \date 2008-02-03 20:52GMT
 * \author Jan Boon (Kaetemi)
 * CSound
 */
class CSound
{
public:
	// pointers
	NLSOUND::UAudioMixer *AudioMixer; // deleted here
	
	// instances
	// ...
protected:
	// pointers
	// ...
	
	// instances
	CConfigProxy _Config;
public:
	CSound(NLMISC::IProgressCallback &progressCallback, const std::string &id);
	virtual ~CSound();

	SBCLIENT_CALLBACK_DECL(updateSound);
private:
	static void configMaxTracks(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag);
}; /* class CSound */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_SOUND_H */

/* end of file */
