/** \file sub_configuration.h
 * CSubConfiguration is a class to handle easy loading and unloading of
 * configuration files used to construct an instance of some class.
 */

#ifndef SBCLIENT_SUB_CONFIGURATION_H
#define SBCLIENT_SUB_CONFIGURATION_H
#include <nel/misc/types_nl.h>
#include <nel/misc/config_file.h>

namespace SBCLIENT {

/**
 * CSubConfiguration
 * \author Jan Boon
 * \date 2007
 */
class CSubConfiguration
{
private: 
	std::string _ConfigPrefix;
	std::string _ConfigPrefixD;
	bool _ConfigFileCreated;
	void tryConfigFile();
protected:
	CSubConfiguration();
	~CSubConfiguration();
	NLMISC::CConfigFile *ConfigFile;
	void setConfigFile(const std::string &configFile, const std::string &configPrefix); // ConfigFiled created and deleted
	void setConfigFile(NLMISC::CConfigFile *configFile, const std::string &configPrefix); // configFile is NOT deleted
	void setConfigFile();
	NLMISC::CConfigFile::CVar &cfgGetVar(const std::string &varName);
	bool cfgExists(const std::string &varName);
};

}

#endif /* SBCLIENT_SUB_CONFIGURATION_H */
