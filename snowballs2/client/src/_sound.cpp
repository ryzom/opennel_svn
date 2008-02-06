/** \file sound.cpp
 * Sound interface between the game and NeL
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

//
// Includes
//

#include <cmath>
#include <vector>

#include <nel/misc/vector.h>
#include <nel/misc/command.h>

#include <nel/sound/u_audio_mixer.h>
#include <nel/sound/u_listener.h>
#include <nel/sound/u_source.h>

#include "_sound.h"
#include "_entities.h"
#include "music_playlist_manager.h"

//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLSOUND;

//
// Variables
//

UAudioMixer *AudioMixer = NULL;
TSoundId SoundId;
const vector<TSoundId> *SoundIdArray;
#ifdef NL_OS_WINDOWS
static SBCLIENT::CMusicPlaylistManager *PlaylistManager = NULL;
#endif
static bool SoundEnabled;

//
// Functions
//

#ifdef NL_OS_WINDOWS
void initSound2();
void releaseSound2();
#endif



#ifdef NL_OS_WINDOWS

void playMusic(sint32 playlist, sint32 track)
{
	if (SoundEnabled)
		PlaylistManager->playMusic(playlist, track);
}

void setMusicVolume(sint32 playlist, float volume)
{
	if (SoundEnabled)
		PlaylistManager->setVolume(playlist, volume);
}

NLMISC_COMMAND(music_bg,"background music","")
{
	if (args.size() != 0) return false;
	playMusic(SBCLIENT_MUSIC_BACKGROUND);
	return true;
}

NLMISC_COMMAND(music_bg_beat,"background music with beat","")
{
	if (args.size() != 0) return false;
	PlaylistManager->playMusic(SBCLIENT_MUSIC_BACKGROUND_BEAT);
	return true;
}

NLMISC_COMMAND(music_wait,"loading music","")
{
	if (args.size() != 0) return false;
	PlaylistManager->playMusic(SBCLIENT_MUSIC_WAIT);
	return true;
}

NLMISC_COMMAND(music_login,"login screen music","")
{
	if (args.size() != 0) return false;
	PlaylistManager->playMusic(SBCLIENT_MUSIC_LOGIN);
	return true;
}

#endif