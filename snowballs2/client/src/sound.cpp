/**
 * \file sound.cpp
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

#include <nel/misc/types_nl.h>
#include "sound.h"

#include "member_callback_impl.h"

// #include <nel/misc/debug.h>
#include <nel/sound/u_audio_mixer.h>

using namespace std;
using namespace NLMISC;
using namespace NLSOUND;

namespace SBCLIENT {

CSound::CSound(IProgressCallback &progressCallback, const std::string &id) : _Config(id)
{
	progressCallback.progress(0.0f);
	AudioMixer = UAudioMixer::createAudioMixer();
	progressCallback.progress(0.5f);
	std::string driverName;
	UAudioMixer::TDriver driverType;
	if (!_Config.exists("Driver")) 
#ifdef NL_OS_WINDOWS
		driverType = NLSOUND::UAudioMixer::DriverFMod;
#elif defined (NL_OS_UNIX)
		driverType = NLSOUND::UAudioMixer::DriverOpenAl;
#else
		driverType = NLSOUND::UAudioMixer::DriverAuto;
#endif
	else 
	{
		driverName = _Config.getVar("Driver").asString();
		if (driverName == "Auto") driverType = NLSOUND::UAudioMixer::DriverAuto;
		else if (driverName == "FMod") driverType = NLSOUND::UAudioMixer::DriverFMod;
		else if (driverName == "DSound") driverType = NLSOUND::UAudioMixer::DriverDSound;
		else if (driverName == "OpenAL") driverType = NLSOUND::UAudioMixer::DriverOpenAl;
		else nlerror("%s_Driver value '%s' is invalid.", id.c_str(), driverName.c_str());
	}

	AudioMixer->init(_Config.getValue("MaxTracks", 32),
		_Config.getValue("UseEAX", true),
		_Config.getValue("UserADPCM", true),
		NULL, false, driverType,
		_Config.getValue("ForceSoftware", true));

	_Config.setCallback("MaxTracks", configMaxTracks, AudioMixer, NULL);
	progressCallback.progress(1.0f);
}

CSound::~CSound()
{
	delete AudioMixer;
}

SBCLIENT_CALLBACK_IMPL(CSound, updateSound)
{
	AudioMixer->update();
}

void CSound::configMaxTracks(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag)
{
	((UAudioMixer *)context)->changeMaxTrack(var.asInt());
}

} /* namespace SBCLIENT */

/* end of file */


// some stuff, todo implementation of enable/disable sound trough config!
//////
//////void cbConfigFileSoundEnabled(NLMISC::CConfigFile::CVar &var)
//////{
//////#ifdef NL_OS_WINDOWS
//////	if (var.asBool() != SoundEnabled)
//////	{
//////		if (var.asBool()) initSound2();
//////		else releaseSound2();
//////	}
//////#endif
//////}
//////
//////void cbConfigFileFail(NLMISC::CConfigFile::CVar &var)
//////{
//////	nlwarning("You can't modify the config variable '%s' at runtime for now, please restart the game", var.asString().c_str());
//////}
//////
//////#ifdef NL_OS_WINDOWS
//////void initSound2()
//////{
//////	AudioMixer = UAudioMixer::createAudioMixer ();
//////	std::string driverName;
//////	NLSOUND::UAudioMixer::TDriver driverType;
//////	if (!ConfigFile->exists("SoundDriver")) 
//////#ifdef NL_OS_WINDOWS
//////		driverType = NLSOUND::UAudioMixer::DriverFMod;
//////#elif defined (NL_OS_UNIX)
//////		driverType = NLSOUND::UAudioMixer::DriverOpenAl;
//////#else
//////		driverType = NLSOUND::UAudioMixer::DriverAuto;
//////#endif
//////	else 
//////	{
//////		driverName = ConfigFile->getVar("SoundDriver").asString();
//////		if (driverName == "Auto") driverType = NLSOUND::UAudioMixer::DriverAuto;
//////		else if (driverName == "FMod") driverType = NLSOUND::UAudioMixer::DriverFMod;
//////		else if (driverName == "DSound") driverType = NLSOUND::UAudioMixer::DriverDSound;
//////		else if (driverName == "OpenAl") driverType = NLSOUND::UAudioMixer::DriverOpenAl;
//////		else nlerror("SoundDriver value '%s' is invalid.", driverName.c_str());
//////	}
//////
//////	AudioMixer->init(
//////		ConfigFile->exists("SoundMaxTracks")
//////		? ConfigFile->getVar("SoundMaxTracks").asInt() : 32,
//////		ConfigFile->exists("SoundUseEax")
//////		? ConfigFile->getVar("SoundUseEax").asBool() : true,
//////		ConfigFile->exists("SoundUseADPCM")
//////		? ConfigFile->getVar("SoundUseADPCM").asBool() : true,
//////		NULL, false, driverType,
//////		ConfigFile->exists("SoundForceSoftware")
//////		? ConfigFile->getVar("SoundForceSoftware").asBool() : true);
//////
//////	ConfigFile->setCallback("SoundMaxTracks", cbConfigFileSoundMaxTracks);
//////	ConfigFile->setCallback("SoundUseEax", cbConfigFileFail);
//////	ConfigFile->setCallback("SoundUseADPCM", cbConfigFileFail);
//////	ConfigFile->setCallback("SoundForceSoftware", cbConfigFileFail);
//////	ConfigFile->setCallback("SoundDriver", cbConfigFileFail);
//////
//////	PlaylistManager = new SBCLIENT::CMusicPlaylistManager(AudioMixer, ConfigFile, "SoundPlaylist");
//////
//////	/* AudioMixer->loadSoundBuffers ("sounds.nss", &SoundIdArray);
//////	nlassert( SoundIdArray->size() == 2 );
//////	SoundId = (*SoundIdArray)[0];
//////	// StSoundId = (*SoundIdArray)[1]; */
//////	
//////	SoundEnabled = true;
//////}
//////#endif
//////
//////void initSound()
//////{
//////#ifdef NL_OS_WINDOWS
//////	if (ConfigFile->exists("SoundEnabled") ? ConfigFile->getVar("SoundEnabled").asBool() : false) initSound2();
//////	ConfigFile->setCallback("SoundEnabled", cbConfigFileSoundEnabled);
//////#endif
//////}
//////
//////void playSound (CEntity &entity, TSoundId id)
//////{
///////*	entity.Source = AudioMixer->createSource (id);
//////	entity.Source->setLooping (true);
//////	entity.Source->play ();
//////*/}
//////
//////void deleteSound (CEntity &entity)
//////{
///////*	if (entity.Source != NULL)
//////	{
//////		if (entity.Source->isPlaying ())
//////			entity.Source->stop ();
//////
//////		AudioMixer->removeSource (entity.Source);
//////		entity.Source = NULL;
//////	}
//////*/}
//////
//////void updateSound(void *context, void *tag)
//////{
//////#ifdef NL_OS_WINDOWS
//////	if (SoundEnabled)
//////	{
//////		PlaylistManager->update(DiffTime);
//////		AudioMixer->update();
//////	}
//////#endif
//////}
//////
//////void updateSound()
//////{
//////	updateSound(NULL, NULL);
//////}
//////
//////#ifdef NL_OS_WINDOWS
//////void releaseSound2()
//////{		
//////	SoundEnabled = false;
//////	ConfigFile->setCallback("SoundMaxTracks", NULL);
//////	ConfigFile->setCallback("SoundUseEax", NULL);
//////	ConfigFile->setCallback("SoundUseADPCM", NULL);
//////	ConfigFile->setCallback("SoundForceSoftware", NULL);
//////	ConfigFile->setCallback("SoundDriver", NULL);
//////	delete PlaylistManager;
//////	PlaylistManager = NULL;
//////	delete AudioMixer;
//////	AudioMixer = NULL;
//////}
//////#endif
//////
//////void releaseSound()
//////{
//////#ifdef NL_OS_WINDOWS
//////	ConfigFile->setCallback("SoundEnabled", NULL);
//////	if (SoundEnabled) releaseSound2();
//////#endif
//////}
//////
