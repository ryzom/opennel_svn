/**
 * \file member_callback_type.h
 * \brief member_callback_type
 * \date 2008-02-06 14:22GMT
 * \author Jan Boon (Kaetemi)
 * member_callback_type
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

#ifndef SBCLIENT_MEMBER_CALLBACK_TYPE_H
#define SBCLIENT_MEMBER_CALLBACK_TYPE_H
#include <nel/misc/types_nl.h>

class ucstring;
#include <nel/misc/config_file.h>
namespace NLMISC {
	class ICommand;
	class CEvent;
}

namespace SBCLIENT {

typedef void (*TCallback)(void *context, void *tag);
typedef void (*TStringCallback)(void *context, const std::string &s, void *tag);
typedef void (*TUcStringCallback)(void *context, const ucstring &s, void *tag);
typedef void (*TConfigCallback)(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag);
typedef bool (*TCommandCallback)(void *context, NLMISC::ICommand &command, const std::string &rawCommandString, const std::vector<std::string> &args, NLMISC::CLog &log, bool quiet, bool human, void *tag);
typedef bool (*TEventCallback)(void *context, const NLMISC::CEvent &ev, void *tag);

}

#endif /* #ifndef SBCLIENT_MEMBER_CALLBACK_TYPE_H */

/* end of file */
