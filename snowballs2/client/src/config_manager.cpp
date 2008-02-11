/**
 * \file config_manager.cpp
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

#include <nel/misc/types_nl.h>
#include "config_manager.h"

#include "evil_singleton_impl.h"

#include <nel/misc/debug.h>
#include <nel/misc/path.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CConfigManager::CConfigManager(const std::string &configFile, const std::string &rootConfigFile) 
: _RootConfigFilename(rootConfigFile)
{
	SBCLIENT_EVIL_SINGLETON_CONSTRUCTOR(SBCLIENT::CConfigManager);
	nlassert(CFile::fileExists(rootConfigFile));
	_ConfigFile = getConfigFile(configFile);
}

CConfigManager::~CConfigManager()
{
	SBCLIENT_EVIL_SINGLETON_DESTRUCTOR(SBCLIENT::CConfigManager);
	dropConfigFile(_ConfigFile);

	for (std::map<NLMISC::CConfigFile *, uint>::iterator it = _ReferenceCounter.begin(); it != _ReferenceCounter.end(); it++)
		nlwarning("Reference counter for ConfigFile '%s' was not properly removed", it->first->getFilename().c_str());

	for (std::map<std::string, NLMISC::CConfigFile *>::iterator it = _ConfigFiles.begin(); it != _ConfigFiles.end(); it++)
	{
		nlwarning("ConfigFile '%s' was not properly released", it->second->getFilename().c_str());
		delete it->second;
	}

	for (std::map<std::string, CConfigCallback>::iterator it = _ConfigCallbacks.begin(); it != _ConfigCallbacks.end(); it++)
		nlwarning("Config callback '%s' was not properly removed", it->first.c_str());

	for (std::map<NLMISC::CConfigFile *, std::string>::iterator it = _ConfigNames.begin(); it != _ConfigNames.end(); it++)
		nlwarning("Config name '%s' was not properly removed", it->second.c_str());
}

CConfigFile *CConfigManager::getConfigFile(const std::string &configFile)
{
	nlassert(!configFile.empty());
	CConfigFile *config = _ConfigFiles[configFile];
	if (!config)
	{
		config = new CConfigFile();
		if (CPath::lookup(configFile, false, true).empty() && !CFile::fileExists(configFile))
		{
			FILE *f = fopen(configFile.c_str(), "wt"); nlassert(f);
			fprintf(f, "RootConfigFilename = \"%s\";\n", _RootConfigFilename.c_str());
			fclose(f);
		}
		config->load(configFile);
		_ConfigFiles[configFile] = config;
		_ConfigNames[config] = configFile;
		_ReferenceCounter[config] = 1;
	}
	else _ReferenceCounter[config] = _ReferenceCounter[config] + 1;
	nlassert(config);
	return config;
}

/** Decreases one reference to a config file */
void CConfigManager::dropConfigFile(CConfigFile *configFile)
{
	nlassert(configFile);
	uint refc = _ReferenceCounter[configFile];
	--refc;
	if (refc == 0) 
	{
		_ReferenceCounter.erase(configFile);
		if (configFile->exists("SaveConfig") && configFile->getVar("SaveConfig").asBool())
			configFile->save();
		_ConfigFiles.erase(_ConfigNames[configFile]);
		_ConfigNames.erase(configFile);
		delete configFile;
	}
	else _ReferenceCounter[configFile] = refc;
}

CConfigFile *CConfigManager::getConfigSub(const std::string &id)
{
	if (_ConfigFile->exists(id) && !_ConfigFile->getVar(id).asString().empty())
	{
		CConfigFile *config = getConfigFile(_ConfigFile->getVar(id).asString());
		while (config->exists(id) && !config->getVar(id).asString().empty())
		{
			CConfigFile *drop = config;
			config = getConfigFile(config->getVar(id).asString());
			dropConfigFile(drop);
			if (config == drop) break; // happens when nonexisting config file gets default root file
		}
		return config;
	}
	_ReferenceCounter[_ConfigFile] = _ReferenceCounter[_ConfigFile] + 1;
	return _ConfigFile;
}

void CConfigManager::setCallback(NLMISC::CConfigFile *configFile, TConfigCallback cb, const std::string &var, void *context, const std::string &name, void *tag)
{
	if (_ConfigCallbacks.find(var) != _ConfigCallbacks.end())
		nlwarning("Config callback for '%s' already exists, overriding", var.c_str());

	_ConfigCallbacks[var] = CConfigCallback(cb, context, name, tag);
	configFile->setCallback(var, configCallback);
}

void CConfigManager::dropCallback(NLMISC::CConfigFile *configFile, const std::string &varName)
{
	nlassert(_ConfigCallbacks.find(varName) != _ConfigCallbacks.end());
	configFile->setCallback(varName, NULL);
	_ConfigCallbacks.erase(varName);
}

void CConfigManager::configCallback(NLMISC::CConfigFile::CVar &var)
{
	CConfigManager &cfgmgr = CConfigManager::getInstance();
	nlassert(cfgmgr._ConfigCallbacks.find(var.Name) != cfgmgr._ConfigCallbacks.end());
	cfgmgr._ConfigCallbacks[var.Name].call(var);
}

SBCLIENT_EVIL_SINGLETON_IMPL(SBCLIENT::CConfigManager);

} /* namespace SBCLIENT */

/* end of file */
