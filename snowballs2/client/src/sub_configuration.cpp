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
	if (cfgExists("ConfigFile"))
	{
		std::string configFile = cfgGetVar("ConfigFile").asString();
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

CConfigFile::CVar &CSubConfiguration::cfgGetVar(const std::string &varName)
{
	return ConfigFile->getVar(_ConfigPrefixD + varName);
}

bool CSubConfiguration::cfgExists(const std::string &varName)
{
	return ConfigFile->exists(_ConfigPrefixD + varName);
}

CSubConfiguration::CSubConfiguration() : _ConfigFileCreated(false)
{

}

CSubConfiguration::~CSubConfiguration()
{
	setConfigFile();
}

}

