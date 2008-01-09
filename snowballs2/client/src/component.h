/** \file component.h
 * IComponent
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * $Id$
 */

#ifndef SBCLIENT_COMPONENT_H
#define SBCLIENT_COMPONENT_H
#include <nel/misc/types_nl.h>

#include <nel/misc/progress_callback.h>
#include <nel/misc/ucstring.h>

namespace SBCLIENT {
	class CComponentManager;

/**
 * \brief IComponent (helper)
 * \date 2007
 * \author Jan Boon (Kaetemi)
 * Base class for components that can be identified uniquely by name.
 */
class IComponent
{
public:
	static std::string I18NPrefix;
	static bool KeepPrefix;

	uint UpdateId; // set and used
	uint RenderId; // by the component manager
protected:
	// pointers
	CComponentManager *_Manager; // not deleted here
	
	// instances
	std::string _InstanceId;
public:	
	/// Basic constructor of a component.
	/// Requires a pointer to the componentmanager and a unique name.
	IComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	virtual ~IComponent();

	virtual void update() { }
	static void update(void *context, void *tag);
	virtual void render() { }
	static void render(void *context, void *tag);

	/// Get the instance id of this component.
	std::string getInstanceId() { return _InstanceId; }

	/// Gets a component instance by name. 
	/// A component that uses this to get another component it depends on,
	/// should get the InstanceId from it's configuration, and optimally
	/// only call this once for every component instance it depends on.
	static IComponent &getInstance(const std::string &instanceId);
	static IComponent *getInstancePtr(const std::string &instanceId);

	/// Called when a component is registered or unregistered
	/// and this component requested to know about it.
	virtual void componentUp(IComponent *component) { }
	virtual void componentDown(IComponent *component) { }
protected:
	void registerNotifier(const std::string &target);
	void unregisterNotifier(const std::string &target);

	/// Gets the translation of the label if the label begins with 
	/// the IComponent::I18NPrefix (default is "i18n"), else it 
	/// simply returns the label. Useful for easy testing without i18n.
	static ucstring i18n(const std::string &label);
	static ucstring i18nGet(const std::string &label);
};

//#define SBCLIENT_PROGRESS_BEGIN float progressCurrent = 0.0f; const float progressTotal = 
//#define SBCLIENT_PROGRESS_ADVANCE progressCurrent += 1.0f; progressCallback.progress(progressCurrent / progressTotal)
//#define SBCLIENT_PROGRESS_END nlassert(progressCurrent == progressTotal);

}

#endif /* #ifndef SBCLIENT_COMPONENT_H */
