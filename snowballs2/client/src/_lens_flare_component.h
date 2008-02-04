/**
 * \file lens_flare_component.h
 * \brief CLensFlareComponent
 * \date 2008-01-09 20:47GMT
 * \author Jan Boon (Kaetemi)
 * CLensFlareComponent
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS 0.2, Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_LENS_FLARE_COMPONENT_H
#define SBCLIENT_LENS_FLARE_COMPONENT_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace NLMISC {
	class CVector;
}

namespace NL3D {
	class UTextureFile;
	class UDriver;
	class UScene;
}

namespace SBCLIENT {
	class CLensFlare;

/**
 * \brief CLensFlareComponent
 * \date 2008-01-09 20:47GMT
 * \author Jan Boon (Kaetemi)
 * CLensFlareComponent
 */
class CLensFlareComponent : public IConfigurableComponent
{
protected:
	// pointers
	NL3D::UDriver *_Driver; // not deleted here
	NL3D::UScene *_Scene; // not deleted here
	CLensFlare *_LensFlare; // created and deleted here
	
	// temp!
	NL3D::UTextureFile *flareTexture1;
	NL3D::UTextureFile *flareTexture3;
	NL3D::UTextureFile *flareTexture4;
	NL3D::UTextureFile *flareTexture5;
	NL3D::UTextureFile *flareTexture6;
	NL3D::UTextureFile *flareTexture7;

	// instances
	// ...
public:
	CLensFlareComponent(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CLensFlareComponent();
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	void componentUp(IComponent *component);
	void componentDown(IComponent *component);

	void setSunDirection(const NLMISC::CVector &direction);
}; /* class CLensFlareComponent */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LENS_FLARE_COMPONENT_H */

/* end of file */
