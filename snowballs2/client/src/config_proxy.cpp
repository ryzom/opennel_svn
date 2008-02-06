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

CRGBA CConfigProxy::getValue(const string &varName, CRGBA &defaultValue)
{
	if (exists(varName)) 
	{
		CConfigFile::CVar &var = getVar(varName);
		if (var.size() >= 3)
		{
			return CRGBA(var.asInt(0), var.asInt(1), var.asInt(2), var.size() >= 4 ? var.asInt(3) : 255);
		}
		nlwarning("Invalid RGBA value in config value '%s_%s', reverting to default { %i, %i, %i, %i }", _Id.c_str(), varName.c_str(), (sint)defaultValue.R, (sint)defaultValue.G, (sint)defaultValue.B, (sint)defaultValue.A);	
	}
	else
	{
		// create a new value only if one doesn't exist
		CConfigFile::CVar varToCopy;
		varToCopy.forceAsInt(defaultValue.R);
		varToCopy.setAsInt(defaultValue.G, 1);
		varToCopy.setAsInt(defaultValue.B, 2);
		varToCopy.setAsInt(defaultValue.A, 3);
		_ConfigFile->insertVar(_IdU + varName, varToCopy);
	}
	return defaultValue;
}

void CConfigProxy::setCallback(const std::string &varName, SBCLIENT_CALLBACK_CONFIG cb, void *context, void *tag)
{
	CConfigManager::getInstance().setCallback(_ConfigFile, cb, _IdU + varName, context, varName, tag);
}

void CConfigProxy::setCallbackAndCall(const std::string &varName, SBCLIENT_CALLBACK_CONFIG cb, void *context, void *tag)
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
