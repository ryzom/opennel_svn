
#include <nel/misc/types_nl.h>
#ifdef NL_OS_WINDOWS
#include "playlist_manager.h"

namespace SBCLIENT {

CPlaylistManager *CPlaylistManager::_Instance = NULL;

CPlaylistManager::CPlaylistManager(NLSOUND::UAudioMixer *audioMixer, NLMISC::CConfigFile *configFile, const std::string &configPrefix)
	: _Current(-1), _CurrentVolume(0.0f), _TargetVolume(1.0f), _TimeVolume(0)
{
	nlassert(_Instance == NULL);
	_Instance = this;

	_ConfigFile = configFile;
	_ConfigPrefix = configPrefix;
	_AudioMixer = audioMixer;
	uint32 playlists = configFile->exists(configPrefix + "s")
		? configFile->getVar(configPrefix + "s").asInt() : 0;
	_Playlists = new CPlaylist[playlists];

	for (uint32 i = 0; i < playlists; ++i)
	{
		std::string iS = NLMISC::toString(i);
		std::string iSp = configPrefix + iS;

		_Playlists[i] = CPlaylist(
			configFile->exists(iSp + "Volume")
			? configFile->getVar(iSp + "Volume").asFloat() : 1.0f,
			configFile->exists(iSp + "Fade")
			? configFile->getVar(iSp + "Fade").asInt() : 0,
			configFile->exists(iSp + "Async")
			? configFile->getVar(iSp + "Async").asBool() : true,
			configFile->exists(iSp + "Loop")
			? configFile->getVar(iSp + "Loop").asBool() : false);

		if (configFile->exists(iSp + "Music"))
		{
			NLMISC::CConfigFile::CVar &plMusic
				= configFile->getVar(iSp + "Music");
			uint32 plSize = plMusic.size();
			_Playlists[i].Music = new std::string[plSize];
			for (uint32 j = 0; j < plSize; ++j)
				_Playlists[i].Music[j] = plMusic.asString(j);
		}
	}
}

CPlaylistManager::~CPlaylistManager()
{
	_AudioMixer->stopMusic();
	delete[] _Playlists;
	_Instance = NULL;
}

CPlaylistManager *CPlaylistManager::getInstance()
{
	nlassert(_Instance != NULL);
	return _Instance;
}

void CPlaylistManager::update(NLMISC::TTime dTime)
{
	// kaetemi_todo: add automatic playlist loop for loop mode 2

	if (_CurrentVolume != _TargetVolume)
	{
		float change = (float)dTime / (float)_TimeVolume;
		if (change > 1.0f) { change = 1.0f; _TimeVolume = 0; }
		else { _TimeVolume -= (sint32)dTime; }
		change *= _TargetVolume - _CurrentVolume;
		_CurrentVolume += change;
		// nlinfo("Changing music volume with %f to %f", change, _CurrentVolume);
		_AudioMixer->setMusicVolume(_CurrentVolume);
	}
}

void CPlaylistManager::playMusic(sint32 playlist, sint32 track)
{
	if (playlist == -1)
	{
		_AudioMixer->stopMusic(_Playlists[_Current].Fade);
		_Current = playlist;
	}

	if (playlist == _Current && track == _Playlists[_Current].Current) return;
	if (track == -1) track = _Playlists[playlist].Current + 1;

	if (_Playlists[playlist].Volume > 0.0f)
	{
		std::string trackS = _Playlists[playlist].Music[track];
		// nlinfo("Setting music to %s", trackS.c_str());
		_AudioMixer->playMusic(
			_Playlists[playlist].Music[track], 
			_Current == -1 ? _Playlists[playlist].Fade
			: std::min(_Playlists[playlist].Fade, _Playlists[_Current].Fade), 
			_Playlists[playlist].Async, 
			_Playlists[playlist].Loop);
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
		// nlinfo("Stopping music with %i fade", 
		//	_Current == -1 ? _Playlists[playlist].Fade
		//	: std::min(_Playlists[playlist].Fade, _Playlists[_Current].Fade));
		_AudioMixer->stopMusic(
			_Current == -1 ? _Playlists[playlist].Fade
			: std::min(_Playlists[playlist].Fade, _Playlists[_Current].Fade));
	}

	_Playlists[playlist].Current = track;
	_Current = playlist;
}

void CPlaylistManager::setVolume(sint32 playlist, float volume)
{
	_Playlists[playlist].Volume = volume;
	if (playlist == _Current) _TargetVolume = volume;
	std::string cfgVarVolume = _ConfigPrefix + NLMISC::toString(playlist) + "Volume";
	_ConfigFile->getVar(cfgVarVolume).setAsFloat(volume);
}

}

#endif