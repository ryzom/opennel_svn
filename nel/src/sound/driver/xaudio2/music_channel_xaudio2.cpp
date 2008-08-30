/**
 * \file music_channel_xaudio2.cpp
 * \brief CMusicChannelXAudio2
 * \date 2008-08-30 13:31GMT
 * \author Jan Boon (Kaetemi)
 * CMusicChannelXAudio2
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * 
 * This file is part of NLSOUND XAudio2 Driver.
 * NLSOUND XAudio2 Driver is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of
 * the License, or (at your option) any later version.
 * 
 * NLSOUND XAudio2 Driver is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NLSOUND XAudio2 Driver; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */

#include "stdxaudio2.h"
#include "music_channel_xaudio2.h"

// Project includes

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/misc/file.h>
#include <nel/misc/mem_stream.h>
#include <nel/misc/stream.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace NLSOUND {

CMusicChannelXAudio2::CMusicChannelXAudio2(CSoundDriverXAudio2 *soundDriver)
{
	_MusicVoices[0].init(soundDriver);
	_MusicVoices[1].init(soundDriver);
	_Streams[0] = NULL;
	_Streams[1] = NULL;
}

CMusicChannelXAudio2::~CMusicChannelXAudio2()
{
	delete _Streams[0]; _Streams[0] = NULL;
	delete _Streams[1]; _Streams[1] = NULL;
}

void CMusicChannelXAudio2::play(NLMISC::CIFile &file, uint xFadeTime, bool loop)
{
	nlinfo("play music preloaded");

	delete _Streams[0]; _Streams[0] = NULL;
 
	//CIFile *ifile = new CIFile(file.
	CMemStream *memstream = new CMemStream(true, false, file.getFileSize()); // todo: delete when done xD
	file.serialBuffer(const_cast<uint8 *>(memstream->buffer()), file.getFileSize());
	_Streams[0] = memstream;

	_MusicVoices[0].play(file.getStreamName(), memstream, loop);
}

void CMusicChannelXAudio2::play(const std::string &path, uint xFadeTime, uint fileOffset, uint fileSize, bool loop)
{
	nlinfo("play music async");

	delete _Streams[0]; _Streams[0] = NULL;
	
	CIFile *ifile = new CIFile(path); // todo: delete the ifile when done :)
	ifile->seek(fileOffset, NLMISC::IStream::begin);
	_Streams[0] = ifile;
	
	_MusicVoices[0].play(ifile->getStreamName(), ifile, loop);
}

} /* namespace NLSOUND */

/* end of file */
