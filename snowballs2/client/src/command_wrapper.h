/**
 * \file command_wrapper.h
 * \brief CCommandWrapper
 * \date 2008-02-06 16:46GMT
 * \author Jan Boon (Kaetemi)
 * CCommandWrapper
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

#ifndef SBCLIENT_COMMAND_WRAPPER_H
#define SBCLIENT_COMMAND_WRAPPER_H
#include <nel/misc/types_nl.h>

// Project includes
#include "member_callback_type.h"

// NeL includes
#include <nel/misc/command.h>

// STL includes

namespace SBCLIENT {

/**
 * \brief CCommandWrapper
 * \date 2008-02-06 16:46GMT
 * \author Jan Boon (Kaetemi)
 * CCommandWrapper
 */
struct CCommandWrapper : public NLMISC::ICommand
{
	CCommandWrapper(const char *categoryName, 
		const char *commandName, const char *commandHelp, 
		const char *commandArgs, SBCLIENT_CALLBACK_COMMAND callback, 
		void *context, void *tag);
	virtual ~CCommandWrapper();
	
	virtual bool execute(const std::string &rawCommandString, const std::vector<std::string> &args, NLMISC::CLog &log, bool quiet, bool human);
	
	SBCLIENT_CALLBACK_COMMAND Callback;
	void *Context;
	void *Tag;
}; /* struct CCommandWrapper */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_COMMAND_WRAPPER_H */

/* end of file */
