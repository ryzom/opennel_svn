/**
 * \file member_callback_decl.h
 * \brief member_callback_decl
 * \date 2008-02-06 13:38GMT
 * \author Jan Boon (Kaetemi)
 * It certainly beats those weird systems where you need to 
 * create a class for every single stupid function.
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

#ifndef SBCLIENT_MEMBER_CALLBACK_DECL_H
#define SBCLIENT_MEMBER_CALLBACK_DECL_H
#include <nel/misc/types_nl.h>

// parameters that are possibly not used by a callback implementation
namespace NLMISC {
	class CEvent;
}

// defines used in class declarations
#define SBCLIENT_CALLBACK_DECL(__name) \
	static void __name(void *context, void *tag); \
	inline void __name(void *tag);

#define SBCLIENT_CALLBACK_ERROR_DECL(__name) \
	static void __name(void *context, const std::string &error, void *tag); \
	inline void __name(const std::string &error, void *tag);

#define SBCLIENT_CALLBACK_STRING_DECL(__name) \
	static void __name(void *context, const std::string &str, void *tag); \
	inline void __name(const std::string &str, void *tag);

#define SBCLIENT_CALLBACK_UCSTRING_DECL(__name) \
	static void __name(void *context, const ucstring &uc, void *tag); \
	inline void __name(const ucstring &uc, void *tag);

#define SBCLIENT_CALLBACK_CONFIG_DECL(__name) \
	static void __name(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag); \
	inline void __name(const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag);

#define SBCLIENT_CALLBACK_COMMAND_DECL(__name) \
	static bool __name(void *context, NLMISC::ICommand &command, const std::string &rawCommandString, const std::vector<std::string> &args, NLMISC::CLog &log, bool quiet, bool human, void *tag); \
	inline bool __name(NLMISC::ICommand &command, const std::string &rawCommandString, const std::vector<std::string> &args, NLMISC::CLog &log, bool quiet, bool human, void *tag);

#define SBCLIENT_CALLBACK_EVENT_DECL(__name) \
	static bool __name(void *context, const NLMISC::CEvent &ev, void *tag); \
	inline bool __name(const NLMISC::CEvent &ev, void *tag);

#define SBCLIENT_CALLBACK_INTERFACE_DECL(__name) \
	static void __name(void *context, const std::string &params, const NLMISC::CEvent *ev, void *tag); \
	inline void __name(const std::string &params, const NLMISC::CEvent *ev, void *tag);

#endif /* #ifndef SBCLIENT_MEMBER_CALLBACK_DECL_H */

/* end of file */
