/** \file music_playlist_manager.h
 * CMusicPlaylistManager is a small system that manages switching between different 
 * music playlists described in a configuration file. The goal of this class is
 * different from the nel sound CMusicSoundManager which is designed to play music
 * on top of each other, while this class focuses on playing music in a sequence.
 * Use whichever is the most useful in your environment.
 */

#ifndef SBCLIENT_PLAYLIST_MANAGER_H
#define SBCLIENT_PLAYLIST_MANAGER_H
#include <nel/misc/types_nl.h>
#ifdef NL_OS_WINDOWS
#include <nel/misc/config_file.h>
#include <nel/sound/u_audio_mixer.h>
#include "sub_configuration.h"

namespace SBCLIENT {

/**
 * A class to manage multiple music playlists, which can each have their own volume
 * and loop settings, configurable trough a configuration file, or directly trough
 * the constructor. Since there can be only one AudioMixer instance, there can only
 * be one instance of this class. You must create and destroy this one instance
 * yourself, it will not be done automagically.
 * \author Jan Boon
 * \date 2007
 */
class CMusicPlaylistManager : CSubConfiguration
{
	// todo: see if possible to implement class on top of CMusicSoundManager.

	struct CPlaylist
	{
		CPlaylist() : Volume(1.0f), Current(0), Fade(0), Async(true), Loop(false), Music() { }
		CPlaylist(float volume, uint32 fade, bool async, uint8 loop) 
			: Current(-1), Music(0), MusicCount(0),
			Volume(volume), Fade(fade), Async(async), Loop(loop) { }
		~CPlaylist() { delete[] Music; }
		float Volume;
		sint32 Current;
		sint32 Fade;
		bool Async;
		uint8 Loop;
		uint32 MusicCount;
		std::string *Music; // deleted by this
	};

private:
	static CMusicPlaylistManager *_Instance;
	sint32 _Current;
	uint32 PlaylistCount;
	CPlaylist *_Playlists; // deleted by this
	float _CurrentVolume; // actual current volume
	float _TargetVolume; // the volume to reach
	sint32 _TimeVolume; // time left to reach the volume
	NLSOUND::UAudioMixer *_AudioMixer; // not deleted by this
	void init(NLSOUND::UAudioMixer *audioMixer);

public:
	CMusicPlaylistManager(NLSOUND::UAudioMixer *audioMixer, NLMISC::CConfigFile *configFile, const std::string &configPrefix);
	CMusicPlaylistManager(NLSOUND::UAudioMixer *audioMixer, const std::string &configFile, const std::string &configPrefix);
	// todo: constructor without config file
	~CMusicPlaylistManager();
	static CMusicPlaylistManager *getInstance();
	void update(NLMISC::TTime dTime);
	void playMusic(sint32 playlist, sint32 track); // playlist -1 to stop, track -1 means next
	void setVolume(sint32 playlist, float volume);
};

}

#endif /* SBCLIENT_PLAYLIST_MANAGER_H */

#endif /* NL_OS_WINDOWS */
