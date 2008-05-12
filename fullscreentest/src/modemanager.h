#ifndef MODEMANAGER_H_
#define MODEMANAGER_H_

class ModeManager;

#include <vector>
#include <tr1/unordered_map>
#define CHashMap std::tr1::unordered_map
#include "gfxmode.h"
#include "extensionwrapper.h"
#include "properties.h"

/**
 * \brief A ModeManager provides a list of screen modes and
 *        allows to set them.
 * 
 * Every ModeManager is assigned to a specific way how to
 * retrieve screen modes and setting them, i.e. there is the
 * XVidModeManager to do all this by using Xinerama and
 * XF86VidMode, while XRandRModeManager is based on the
 * XRandR extension.
 * 
 * ModeManager was designed with Linux/X11 in mind. To lighten
 * the amount of dependencies, a dynamic library loading
 * approach was taken and resulted in the two additional
 * methods setLibraryPath() and initLibraries(). Former is
 * optional to use, latter should be called even if the
 * ModeManager is natively linked (the method will do nothing
 * then).
 * 
 * ModeManagers are stateless by design. This means they just
 * provide access to the mode retrieval and mode changer
 * functionality provided by different underlying libraries
 * or other means. In other words: They are not made for
 * saving the mode an application started in so it can be
 * restored on application exit. That's the job the application
 * has to take care of.
 * 
 * To use a ModeManager, the following steps have to be taken:
 * \arg Create the respective ModeManager object
 * \arg Configure the path names of the required libraries
 *      using setLibraryPath(), if necessary.
 * \arg Call initLibraries() to properly initialize function
 *      pointers to the library functions.
 * \arg With the invocation of initModes() the list of screen
 *      modes is filled with the ones supported by the
 *      ModeManager.
 * \arg After these steps the methods getModes(), getCurrentMode()
 *      and setMode() are free to use.
 */
class ModeManager {
public:
	ModeManager(ExtensionWrapperFactory *fac) : wrapperFactory(fac) {}
	virtual ~ModeManager();

	/**
	 * \brief Returns the list of supported screen modes.
	 * 
	 * Returns a list of screen modes (as GfxMode entries)
	 * which is supported by this ModeManager.
	 */
	const std::vector<GfxMode*> &getModes();

	/**
	 * \brief Sets the screen to the mode given as argument.
	 * 
	 * sjdgfjasdg
	 */
	virtual bool setMode(GfxMode *mode) = 0;
	virtual bool initLibraries() = 0;
	virtual void initModes() = 0;
	virtual GfxMode* getCurrentMode() = 0;
	virtual void cleanup(GfxMode* mode) {}

	void setLibraryPath(std::string name, std::string path);
	
	CVariant<uintptr_t>& operator[](std::string& key) { return properties[key]; }
	CVariant<uintptr_t>& operator[](const char *key) { return properties[key]; }

protected:
	ExtensionWrapperFactory *wrapperFactory;
	std::vector<GfxMode*> modeList;
	CHashMap<std::string, std::string> libraries;
	CProperties<uintptr_t> properties;
};

#endif /*MODEMANAGER_H_*/
