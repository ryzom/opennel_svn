#ifndef EXTENSIONWRAPPER_H_
#define EXTENSIONWRAPPER_H_

#include <dlfcn.h>
#include <iostream>
#include <tr1/unordered_map>

// helper define to initialize a function with dlsym();
// useful for initLibrary() method content
// TODO make this more C++-ish, if possible
#define LIBRARY_FUNCTION(func, name)                                         \
	func = reinterpret_cast<func##_t>(dlsym(libraryHandle, name));           \
	if (func == NULL) {                                                      \
		std::cerr << ("dlsym(" name ") failed: ") << dlerror() << std::endl; \
		return false;                                                        \
	}

/**
 * \brief ExtensionWrappers cover all functions for a
 *        specific X11 extension. They don't have any
 *        state beyond the function pointers to the
 *        actual X11 extension functions and the library
 *        handle for the dlopen()/dlsym()/dlclose() calls.
 * 
 * The ExtensionWrapper allows to use the same extension
 * multiple times at different places without having to
 * duplicate the code again to do exactly the same which
 * has been done elsewhere already (i.e. Xinerama-Code
 * used in multiple ModeManager's).
 * 
 * Each wrapper keeps a reference to the factory it's
 * created in (which means one instance of a wrapper can
 * have only one factory); this allows the wrapper to
 * access the factory for other ExtensionWrappers, if
 * necessary.
 */
class ExtensionWrapper {
public:
	ExtensionWrapper() :
		name(NULL), initialized(false), libraryHandle(NULL) {
	}
	
	virtual ~ExtensionWrapper();
	
	virtual bool initLibrary(const char *path = NULL) = 0;
	
	const char *getName() const {
		return name;
	}

protected:
	const char *name;
	bool initialized;
	void *libraryHandle;
	
	bool initLibraryHandle(const char *path = NULL);
};

/**
 * \brief The ExtensionWrapperFactory manages a set of
 *        ExtensionWrappers. Currently it only allows to
 *        instantiate a specific set of ExtensionWrappers,
 *        which are loaded at instantiation of the factory.
 * 
 * The idea is to have one such ExtensionWrapperFactory
 * instance easily accessible for all parts involved. However,
 * it's not defined as a singleton to circumvent certain
 * downsides of this construct; it shouldn't be a problem
 * to keep one such instance around.
 */
class ExtensionWrapperFactory {
public:
	/**
	 * \brief Returns an ExtensionWrapper matching the given name,
	 *        with an optional path for the .so file to be used.
	 *        In case of an error it will return 0 / NULL.
	 */
	ExtensionWrapper *getInstance(const char *name, const char *path = NULL);

	void addWrapper(ExtensionWrapper *wrapper);
	
private:
	std::tr1::unordered_map<std::string, ExtensionWrapper*> wrappers;
};

#endif /*EXTENSIONWRAPPER_H_*/
