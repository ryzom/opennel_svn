#include "modemanager.h"
#include <string>

ModeManager::~ModeManager()
{
	std::vector<GfxMode*>::iterator iter;
	for (iter = modeList.begin(); iter != modeList.end(); iter++)
		delete *iter;
	modeList.clear();
}

void ModeManager::setLibraryPath(std::string name, std::string path)
{
	libraries[name] = path;
}

const std::vector<GfxMode*> &ModeManager::getModes()
{
	return modeList;
}
