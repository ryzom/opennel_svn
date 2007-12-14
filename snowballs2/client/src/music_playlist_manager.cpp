/** \file music_playlist_manager.h * 
 * CMusicPlaylistManager
 */

#include <nel/misc/types_nl.h>
#ifdef NL_OS_WINDOWS
#include "music_playlist_manager.h"

using namespace std;
using namespace NLMISC;
using namespace NLSOUND;

namespace SBCLIENT {

CMusicPlaylistManager *CMusicPlaylistManager::_Instance = NULL;

void CMusicPlaylistManager::init(UAudioMixer *audioMixer)
{
	nlassert(_Instance == NULL);

	_Instance = this;

	_AudioMixer = audioMixer;
	PlaylistCount = cfgExists("Count")
		? cfgGetVar("Count").asInt() : 0;
	_Playlists = new CPlaylist[PlaylistCount];

	for (uint32 i = 0; i < PlaylistCount; ++i)
	{
		std::string iS = NLMISC::toString("%u_", i);

		_Playlists[i] = CPlaylist(
			cfgExists(iS + "Volume")
			? cfgGetVar(iS + "Volume").asFloat() : 1.0f,
			cfgExists(iS + "Fade")
			? cfgGetVar(iS + "Fade").asInt() : 0,
			cfgExists(iS + "Async")
			? cfgGetVar(iS + "Async").asBool() : true,
			cfgExists(iS + "Loop")
			? (uint8)cfgGetVar(iS + "Loop").asInt() : 0);

		if (cfgExists(iS + "Music"))
		{
			NLMISC::CConfigFile::CVar &plMusic
				= cfgGetVar(iS + "Music");
			_Playlists[i].MusicCount = plMusic.size();
			_Playlists[i].Music = new std::string[_Playlists[i].MusicCount];
			for (uint32 j = 0; j < _Playlists[i].MusicCount; ++j)
				_Playlists[i].Music[j] = plMusic.asString(j);
		}
	}
}
CMusicPlaylistManager::CMusicPlaylistManager(UAudioMixer *audioMixer, const string &configFile, const string &configPrefix)
	: _Current(-1), _CurrentVolume(0.0f), _TargetVolume(0.0f), _TimeVolume(0)
{
	setConfigFile(configFile, configPrefix);
	init(audioMixer);
}

CMusicPlaylistManager::CMusicPlaylistManager(UAudioMixer *audioMixer, CConfigFile *configFile, const string &configPrefix)
	: _Current(-1), _CurrentVolume(0.0f), _TargetVolume(0.0f), _TimeVolume(0)
{
	setConfigFile(configFile, configPrefix);
	init(audioMixer);
}

CMusicPlaylistManager::~CMusicPlaylistManager()
{
	_AudioMixer->stopMusic();
	delete[] _Playlists;
	_Instance = NULL;
}

CMusicPlaylistManager *CMusicPlaylistManager::getInstance()
{
	nlassert(_Instance != NULL);
	return _Instance;
}

void CMusicPlaylistManager::update(NLMISC::TTime dTime)
{
	// kaetemi_todo: add automatic playlist loop for loop mode 2

	if (_CurrentVolume != _TargetVolume)
	{
		float change = (float)dTime / (float)_TimeVolume;
		if (change > 1.0f) { change = 1.0f; _TimeVolume = 0; }
		else { _TimeVolume -= (sint32)dTime; }
		change *= _TargetVolume - _CurrentVolume;
		_CurrentVolume += change;
		_AudioMixer->setMusicVolume(_CurrentVolume);
	}
}

void CMusicPlaylistManager::playMusic(sint32 playlist, sint32 track)
{
	if (playlist == -1)
	{
		_AudioMixer->stopMusic(_Playlists[_Current].Fade);
		_Current = playlist;
	}

	if (playlist == _Current && track == _Playlists[_Current].Current) return;
	if (track == -1) track = _Playlists[playlist].Current + 1;

	if (playlist < 0 || playlist >= (sint32)PlaylistCount)
	{
		nlwarning("Music playlist '%i' doesn't exist", playlist);
		return;
	}
	if (track < 0 || track >= (sint32)_Playlists[playlist].MusicCount)
	{
		nlwarning("Track '%i' doesn't exist in music playlist '%i'", track, playlist);
		return;
	}

	if (_Playlists[playlist].Volume > 0.0f)
	{
		std::string trackS = _Playlists[playlist].Music[track];
		_AudioMixer->playMusic(
			_Playlists[playlist].Music[track], 
			_Current == -1 ? _Playlists[playlist].Fade
			: std::min(_Playlists[playlist].Fade, _Playlists[_Current].Fade), 
			_Playlists[playlist].Async, 
			_Playlists[playlist].Loop == 1);
		_TargetVolume = _Playlists[playlist].Volume;
		_TimeVolume = _TargetVolume == _CurrentVolume ? 0 : _Current == -1 
			? _Playlists[playlist].Fade
			: std::min(_Playlists[playlist].Fade, _Playlists[_Current].Fade);
		if (_TimeVolume == 0)
		{
			_CurrentVolume = _TargetVolume;
			_AudioMixer->setMusicVolume(_TargetVolume);
		}
	}
	else
	{
		_AudioMixer->stopMusic(
			_Current == -1 ? _Playlists[playlist].Fade
			: std::min(_Playlists[playlist].Fade, _Playlists[_Current].Fade));
	}

	_Playlists[playlist].Current = track;
	_Current = playlist;
}

void CMusicPlaylistManager::setVolume(sint32 playlist, float volume)
{
	_Playlists[playlist].Volume = volume;
	if (playlist == _Current) _TargetVolume = volume;
	std::string cfgVarVolume = NLMISC::toString("%i_Volume", playlist);
	cfgGetVar(cfgVarVolume).setAsFloat(volume);
}

}

#endif