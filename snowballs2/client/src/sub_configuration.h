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
	// maybe add void *Context, *State members to CVar
	struct CConfigCallback
	{
		CConfigCallback() { }
		CConfigCallback(void (*cb)(NLMISC::CConfigFile::CVar &, void *, void *), void *context, void *state)
			: Callback(cb), Context(context), State(state) { }
		void (*Callback)(NLMISC::CConfigFile::CVar &, void *, void *);
		void *Context;
		void *State;
	};
	static std::map<std::string, CConfigCallback> _CVarStates;
	static void callback(NLMISC::CConfigFile::CVar &var);
	std::string _ConfigPrefix;
	std::string _ConfigPrefixD;
	NLMISC::CConfigFile *ConfigFile;
	bool _ConfigFileCreated;
	void tryConfigFile();
public:
	CSubConfiguration();
	~CSubConfiguration();
	void setConfigFile(const std::string &configFile, const std::string &configPrefix); // ConfigFiled created and deleted
	void setConfigFile(NLMISC::CConfigFile *configFile, const std::string &configPrefix); // configFile is NOT deleted
	void setConfigFile();
	NLMISC::CConfigFile::CVar &getVar(const std::string &varName);
	bool exists(const std::string &varName);
	void setCallback(const std::string &varName, void (*cb)(NLMISC::CConfigFile::CVar &, void *, void *), void *context, void *state);
};

}

#endif /* SBCLIENT_SUB_CONFIGURATION_H */
