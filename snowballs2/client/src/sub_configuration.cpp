/** \file sub_configuration.h * 
 * CSubConfiguration
 */

#include <nel/misc/types_nl.h>
#include "sub_configuration.h"

using namespace NLMISC;

namespace SBCLIENT {

void CSubConfiguration::setConfigFile(const std::string &configFile, const std::string &configPrefix)
{
	setConfigFile();
	_ConfigPrefix = configPrefix;
	_ConfigPrefixD = configPrefix + "_";
	ConfigFile = new CConfigFile();
	_ConfigFileCreated = true;
	ConfigFile->load(configFile);
	tryConfigFile();
}

void CSubConfiguration::setConfigFile(NLMISC::CConfigFile *configFile, const std::string &configPrefix)
{
	setConfigFile();
	_ConfigPrefix = configPrefix;
	_ConfigPrefixD = configPrefix + "_";
	ConfigFile = configFile;
	tryConfigFile();
}

void CSubConfiguration::tryConfigFile()
{
	if (exists("ConfigFile"))
	{
		std::string configFile = getVar("ConfigFile").asString();
		std::string configPrefix = _ConfigPrefix;
		setConfigFile(configFile, configPrefix);
	}
}

void CSubConfiguration::setConfigFile()
{
	if (_ConfigFileCreated)
	{
		if (ConfigFile->exists("SaveConfig") && ConfigFile->getVar("SaveConfig").asBool())
			ConfigFile->save();
		_ConfigFileCreated = false;
		delete ConfigFile;
	}
}

CConfigFile::CVar &CSubConfiguration::getVar(const std::string &varName)
{
	return ConfigFile->getVar(_ConfigPrefixD + varName);
}

bool CSubConfiguration::exists(const std::string &varName)
{
	return ConfigFile->exists(_ConfigPrefixD + varName);
}

void CSubConfiguration::setCallback(const std::string &varName, void (*cb)(CConfigFile::CVar &, void *, void *), void *context, void *state)
{
	std::string fullName = _ConfigPrefixD + varName;
	if (cb == NULL)
	{
		std::map<std::string, CConfigCallback>::iterator
			it = _CVarStates.find(fullName);
		if (it != _CVarStates.end()) _CVarStates.erase(it);
		ConfigFile->setCallback(fullName, NULL);
	}
	else
	{
		_CVarStates[fullName] = CConfigCallback(cb, context, state);
		ConfigFile->setCallback(fullName, callback);
	}
}

std::map<std::string, CSubConfiguration::CConfigCallback> CSubConfiguration::_CVarStates;

void CSubConfiguration::callback(CConfigFile::CVar &var)
{
	_CVarStates[var.Name].Callback(var, _CVarStates[var.Name].Context, _CVarStates[var.Name].State);
}

CSubConfiguration::CSubConfiguration() : _ConfigFileCreated(false)
{

}

CSubConfiguration::~CSubConfiguration()
{
	setConfigFile();
}

}

