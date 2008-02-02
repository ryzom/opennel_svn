/**
 * \file config_proxy.cpp
 * \brief CConfigProxy
 * \date 2008-02-02 13:01GMT
 * \author Jan Boon (Kaetemi)
 * CConfigProxy
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
#include "config_proxy.h"

#include "config_manager.h"

// #include <nel/misc/debug.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CConfigProxy::CConfigProxy(const string &id)
{
	_ConfigFile = CConfigManager::getInstance().getConfigSub(id);
	_Id = id; _IdU = id + "_";	
}

CConfigFile::CVar &CConfigProxy::getVar(const std::string &varName)
{
	return _ConfigFile->getVar(_IdU + varName);
}

CConfigFile::CVar *CConfigProxy::getVarPtr(const std::string &varName)
{
	return _ConfigFile->getVarPtr(_IdU + varName);
}

bool CConfigProxy::exists(const std::string &varName)
{
	return _ConfigFile->exists(_IdU + varName);
}

float CConfigProxy::getValue(const string &varName, float defaultValue)
{
	if (exists(varName)) return getVar(varName).asFloat();
	CConfigFile::CVar varToCopy;
	varToCopy.forceAsDouble((double)defaultValue);	
	_ConfigFile->insertVar(_IdU + varName, varToCopy);
	return defaultValue;
}

double CConfigProxy::getValue(const string &varName, double defaultValue)
{
	if (exists(varName)) return getVar(varName).asDouble();
	CConfigFile::CVar varToCopy;
	varToCopy.forceAsDouble(defaultValue);	
	_ConfigFile->insertVar(_IdU + varName, varToCopy);
	return defaultValue;
}

int CConfigProxy::getValue(const string &varName, int defaultValue)
{
	if (exists(varName)) return getVar(varName).asInt();
	CConfigFile::CVar varToCopy;
	varToCopy.forceAsInt(defaultValue);	
	_ConfigFile->insertVar(_IdU + varName, varToCopy);
	return defaultValue;
}

std::string CConfigProxy::getValue(const string &varName, const string &defaultValue)
{
	if (exists(varName)) return getVar(varName).asString();
	CConfigFile::CVar varToCopy;
	varToCopy.forceAsString(defaultValue);
	_ConfigFile->insertVar(_IdU + varName, varToCopy);
	return defaultValue;
}

bool CConfigProxy::getValue(const string &varName, bool defaultValue)
{
	if (exists(varName)) return getVar(varName).asBool();
	CConfigFile::CVar varToCopy;
	varToCopy.forceAsInt(defaultValue ? 1 : 0);	
	_ConfigFile->insertVar(_IdU + varName, varToCopy);
	return defaultValue;
}

void CConfigProxy::setCallback(const std::string &varName, void (*cb)(void *, const std::string &, NLMISC::CConfigFile::CVar &, void *), void *context, void *tag)
{
	CConfigManager::getInstance().setCallback(_ConfigFile, cb, _IdU + varName, context, varName, tag);
}

void CConfigProxy::setCallbackAndCall(const std::string &varName, void (*cb)(void *, const std::string &, NLMISC::CConfigFile::CVar &, void *), void *context, void *tag)
{
	std::string fullName = _IdU + varName;
	CConfigManager::getInstance().setCallback(_ConfigFile, cb, fullName, context, varName, tag);
	cb(context, varName, _ConfigFile->getVar(fullName), tag);
}

void CConfigProxy::dropCallback(const std::string &varName)
{
	CConfigManager::getInstance().dropCallback(_ConfigFile, _IdU + varName);
}

CConfigProxy::~CConfigProxy()
{
	CConfigManager::getInstance().dropConfigFile(_ConfigFile);
}

} /* namespace SBCLIENT */

/* end of file */
