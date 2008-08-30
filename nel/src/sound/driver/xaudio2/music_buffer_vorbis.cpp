/**
 * \file music_buffer_vorbis.cpp
 * \brief CMusicBufferVorbis
 * \date 2008-08-30 11:38GMT
 * \author Jan Boon (Kaetemi)
 * CMusicBufferVorbis
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * 
 * This file is part of NLSOUND Music Library.
 * NLSOUND Music Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of
 * the License, or (at your option) any later version.
 * 
 * NLSOUND Music Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NLSOUND Music Library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */

#include "stdxaudio2.h"
#include "music_buffer_vorbis.h"

// Project includes

// NeL includes
#include <nel/misc/stream.h>
#include <nel/misc/file.h>

// STL includes

using namespace std;
using namespace NLMISC;

namespace NLSOUND {

size_t vorbisReadFunc(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	//CIFile *ifile = (CIFile *)datasource;
	NLMISC::IStream *stream = (NLMISC::IStream *)datasource;
	nlassert(stream->isReading());
	//stream->serialBuffer
	//if (ifile->eof()) { nlwarning("ifile->eof()"); return 0; }
	try
	{
		stream->serialBuffer((uint8 *)ptr, size * nmemb);
	}
	catch (EReadError)
	{
		for (uint32 i = 0; i < size * nmemb; ++i)
		{
			try
			{
				stream->serial(((uint8 *)ptr)[i]);
			}
			catch (EReadError)
			{
				return i;
			}
		}
	}
	catch (EStreamOverflow)
	{
		for (uint32 i = 0; i < size * nmemb; ++i)
		{
			try
			{
				stream->serial(((uint8 *)ptr)[i]);
			}
			catch (EStreamOverflow)
			{
				return i;
			}
		}
	}
	return size * nmemb;
}

//int vorbisSeekFunc(void *datasource, ogg_int64_t offset, int whence)
//{
//	CIFile *ifile = (CIFile *)datasource;
//	return 0;
//}

//int vorbisCloseFunc(void *datasource)
//{
//	NLMISC::IStream *stream = (NLMISC::IStream *)datasource;
//}

//long vorbisTellFunc(void *datasource)
//{
//	CIFile *ifile = (CIFile *)datasource;
//	return ifile->getFileSize();
//}

static ov_callbacks OV_CALLBACKS_NLMISC_STREAM = {
  (size_t (*)(void *, size_t, size_t, void *))  vorbisReadFunc,
  (int (*)(void *, ogg_int64_t, int))           NULL, //vorbisSeekFunc,
  (int (*)(void *))                             NULL, //vorbisCloseFunc,
  (long (*)(void *))                            NULL, //vorbisTellFunc
};

CMusicBufferVorbis::CMusicBufferVorbis(NLMISC::IStream *stream, bool loop) : _Stream(stream), _Loop(loop)
{
	ov_open_callbacks(stream, &_OggVorbisFile, NULL, 0, OV_CALLBACKS_NLMISC_STREAM);
}

CMusicBufferVorbis::~CMusicBufferVorbis()
{
	ov_clear(&_OggVorbisFile);
}

uint32 CMusicBufferVorbis::getRequiredBytes()
{
	return 0; // no minimum requirement of bytes to buffer out
}

int current_section = 0; // ???
uint32 CMusicBufferVorbis::getNextBytes(uint8 *buffer, uint32 minimum, uint32 maximum)
{
	nlassert(minimum <= maximum);
	uint32 bytes_read = 0;
	do
	{
		// signed 16-bit little-endian samples
		int br = ov_read(&_OggVorbisFile, (char *)buffer, maximum - bytes_read, 0, getBitsPerSample() == 8 ? 1 : 2, 1, &current_section);
		if (br <= 0) { nlwarning("ov_read: %i", br); break; }
		bytes_read += (uint32)br;
	} while (bytes_read < minimum);
	return bytes_read;
}

uint16 CMusicBufferVorbis::getChannels()
{
	vorbis_info *vi = ov_info(&_OggVorbisFile, -1);
	return vi->channels;
}

uint32 CMusicBufferVorbis::getSamplesPerSec()
{
	vorbis_info *vi = ov_info(&_OggVorbisFile, -1);
	return vi->rate;
}

uint16 CMusicBufferVorbis::getBitsPerSample()
{
	return 16;
}

} /* namespace NLSOUND */

/* end of file */
