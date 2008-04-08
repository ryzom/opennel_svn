#include "extensionwrapper.h"

ExtensionWrapper::~ExtensionWrapper() {
	dlerror(); // clear any previously collected errors
	if (libraryHandle != NULL)
		dlclose(libraryHandle);
}

bool ExtensionWrapper::initLibraryHandle(const char *path) {
	// initialized should be set/read via a mutex to make sure
	// it's working 100% in all cases. For now, the current way
	// will have to do ... check for libraryHandle before doing
	// anything else, just to be sure
	if (initialized)
		return true;
	dlerror(); // clear any previously collected errors
	if (libraryHandle != NULL) { // in case the check on initialized failed
		dlclose(libraryHandle);
		dlerror(); // clear possible error from dlcose()
	}
	libraryHandle = dlopen(path, RTLD_NOW);
	if (libraryHandle == NULL) {
		std::cerr << "dlopen(\"" << path << "\", RTLD_NOW) failed: " << dlerror()
				<< std::endl;
		return false;
	}
	return true;
}

ExtensionWrapper *ExtensionWrapperFactory::getInstance(const char *name, const char *path) {
	if (name == NULL)
		return 0;
	ExtensionWrapper *wrapper = wrappers[std::string(name)];
	if (wrapper == 0)
		return 0;
	wrapper->initLibrary(path);
	return wrapper;
}

void ExtensionWrapperFactory::addWrapper(ExtensionWrapper *wrapper) {
	// sanity check
	if (wrapper == 0)
		return;
	// if the thing has no name, then it probably
	// didn't get initialized properly
	if (wrapper->getName() == NULL)
		return;
	wrappers[std::string(wrapper->getName())] = wrapper;
}
