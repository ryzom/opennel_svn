#include <string>
#include "modemanager.h"

ModeManager::~ModeManager() {
	std::vector<GfxMode*>::iterator iter;
	for (iter = modeList.begin(); iter != modeList.end(); iter++) {
		if ((*iter)->Manager != 0)
			(*iter)->Manager->cleanup(*iter);
		delete *iter;
	}
	modeList.clear();
}

void ModeManager::setLibraryPath(std::string name, std::string path) {
	libraries[name] = path;
}

const std::vector<GfxMode*> &ModeManager::getModes() {
	return modeList;
}
