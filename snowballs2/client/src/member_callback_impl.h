/**
 * \file member_callback_impl.h
 * \brief member_callback_impl
 * \date 2008-02-06 13:38GMT
 * \author Jan Boon (Kaetemi)
 * member_callback_impl
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

#ifndef SBCLIENT_MEMBER_CALLBACK_IMPL_H
#define SBCLIENT_MEMBER_CALLBACK_IMPL_H
#include <nel/misc/types_nl.h>

#include <nel/misc/debug.h>

#define SBCLIENT_CALLBACK_IMPL(__class, __name) \
	void __class::__name(void *context, void *tag) \
	{ \
		nlassert(context); \
		((__class *)context)->__name(tag); \
	} \
	void __class::__name(void *tag)

#define SBCLIENT_CALLBACK_ERROR_IMPL(__class, __name) \
	void __class::__name(void *context, const std::string &error, void *tag) \
	{ \
		nlassert(context); \
		((__class *)context)->__name(error, tag); \
	} \
	void __class::__name(const std::string &error, void *tag)

#define SBCLIENT_CALLBACK_STRING_IMPL(__class, __name) \
	void __class::__name(void *context, const std::string &str, void *tag) \
	{ \
		nlassert(context); \
		((__class *)context)->__name(str, tag); \
	} \
	void __class::__name(const std::string &str, void *tag)

#define SBCLIENT_CALLBACK_UCSTRING_IMPL(__class, __name) \
	void __class::__name(void *context, const ucstring &uc, void *tag) \
	{ \
		nlassert(context); \
		((__class *)context)->__name(uc, tag); \
	} \
	void __class::__name(const ucstring &uc, void *tag)

#define SBCLIENT_CALLBACK_CONFIG_IMPL(__class, __name) \
	void __class::__name(void *context, const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag) \
	{ \
		nlassert(context); \
		((__class *)context)->__name(varName, var, tag); \
	} \
	void __class::__name(const std::string &varName, NLMISC::CConfigFile::CVar &var, void *tag)

#define SBCLIENT_CALLBACK_COMMAND_IMPL(__class, __name) \
	bool __class::__name(void *context, NLMISC::ICommand &command, const std::string &rawCommandString, const std::vector<std::string> &args, NLMISC::CLog &log, bool quiet, bool human, void *tag) \
	{ \
		nlassert(context); \
		return ((__class *)context)->__name(command, rawCommandString, args, log, quiet, human, tag); \
	} \
	bool __class::__name(NLMISC::ICommand &command, const std::string &rawCommandString, const std::vector<std::string> &args, NLMISC::CLog &log, bool quiet, bool human, void *tag)

#define SBCLIENT_CALLBACK_EVENT_IMPL(__class, __name) \
	bool __class::__name(void *context, const NLMISC::CEvent &ev, void *tag) \
	{ \
		nlassert(context); \
		return ((__class *)context)->__name(ev, tag); \
	} \
	bool __class::__name(const NLMISC::CEvent &ev, void *tag)

#endif /* #ifndef SBCLIENT_MEMBER_CALLBACK_IMPL_H */

/* end of file */
