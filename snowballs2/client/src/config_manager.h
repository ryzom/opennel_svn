/**
 * \file config_manager.h
 * \brief CConfigManager
 * \date 2008-02-02 13:49GMT
 * \author Jan Boon (Kaetemi)
 * CConfigManager
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

#ifndef SBCLIENT_CONFIG_MANAGER_H
#define SBCLIENT_CONFIG_MANAGER_H
#include <nel/misc/types_nl.h>

#include "evil_singleton_decl.h"

#include <nel/misc/config_file.h>

#include <map>

namespace SBCLIENT {
	class CConfigProxy;

/**
 * \brief CConfigManager
 * \date 2008-02-02 13:49GMT
 * \author Jan Boon (Kaetemi)
 * CConfigManager
 */
class CConfigManager
{
	struct CConfigCallback
	{
		CConfigCallback() { }
		CConfigCallback(void (*cb)(void *, const std::string &, NLMISC::CConfigFile::CVar &, void *), void *context, const std::string &name, void *tag)
			: Callback(cb), Context(context), Name(name), Tag(tag) { }
		void (*Callback)(void *, const std::string &, NLMISC::CConfigFile::CVar &, void *);
		void *Context;
		std::string Name;
		void *Tag;
		void call(NLMISC::CConfigFile::CVar &var) { Callback(Context, Name, var, Tag); }
	};
protected:
	// pointers
	NLMISC::CConfigFile *_ConfigFile;
	
	// instances
	std::map<NLMISC::CConfigFile *, uint> _ReferenceCounter;
	std::map<std::string, NLMISC::CConfigFile *> _ConfigFiles;
	std::map<NLMISC::CConfigFile *, std::string> _ConfigNames;
	std::map<std::string, CConfigCallback> _ConfigCallbacks;
public:
	CConfigManager(const std::string &configFile);
	virtual ~CConfigManager();

	NLMISC::CConfigFile *getConfigFile(const std::string &configFile);
	void dropConfigFile(NLMISC::CConfigFile *configFile);
	NLMISC::CConfigFile *getConfigSub(const std::string &id);

	void setCallback(NLMISC::CConfigFile *configFile, void (*cb)(void *, const std::string &, NLMISC::CConfigFile::CVar &, void *), const std::string &var, void *context, const std::string &name, void *tag);
	void dropCallback(NLMISC::CConfigFile *configFile, const std::string &varName);

private:
	static void configCallback(NLMISC::CConfigFile::CVar &var);

	SBCLIENT_EVIL_SINGLETON_DECL(SBCLIENT::CConfigManager);
}; /* class CConfigManager */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_CONFIG_MANAGER_H */

/* end of file */
