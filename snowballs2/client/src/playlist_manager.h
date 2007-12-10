

#ifndef SBCLIENT_PLAYLIST_MANAGER_H
#define SBCLIENT_PLAYLIST_MANAGER_H
#include <nel/misc/types_nl.h>
#ifdef NL_OS_WINDOWS
#include <nel/misc/config_file.h>
#include <nel/sound/u_audio_mixer.h>
// #include <nel/sound/u_listener.h>
// #include <nel/sound/u_source.h>

namespace SBCLIENT {

class CPlaylistManager
{
	struct CPlaylist
	{
		CPlaylist() : Volume(1.0f), Current(0), Fade(0), Async(true), Loop(false), Music() { }
		CPlaylist(float volume, uint32 fade, bool async, bool loop) : Current(-1), Music(),
			Volume(volume), Fade(fade), Async(async), Loop(loop) { }
		~CPlaylist() { delete[] Music; }
		float Volume;
		sint32 Current;
		sint32 Fade;
		bool Async;
		bool Loop;
		std::string *Music; // deleted by this
	};

private:
	static CPlaylistManager *_Instance;
	sint32 _Current;
	CPlaylist *_Playlists; // deleted by this
	float _CurrentVolume; // actual current volume
	float _TargetVolume; // the volume to reach
	sint32 _TimeVolume; // time left to reach the volume
	std::string _ConfigPrefix;
	NLMISC::CConfigFile *_ConfigFile; // not deleted by this
	NLSOUND::UAudioMixer *_AudioMixer; // not deleted by this

public:
	CPlaylistManager(NLSOUND::UAudioMixer *audioMixer, NLMISC::CConfigFile *configFile, const std::string &configPrefix);
	~CPlaylistManager();
	static CPlaylistManager *getInstance();
	void update(NLMISC::TTime dTime);
	void playMusic(sint32 playlist, sint32 track); // playlist -1 to stop, track -1 means next
	void setVolume(sint32 playlist, float volume);
};

}

#endif /* SBCLIENT_PLAYLIST_MANAGER_H */

#endif /* NL_OS_WINDOWS */
