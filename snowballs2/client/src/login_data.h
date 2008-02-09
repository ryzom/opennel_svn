/**
 * \file login_data.h
 * \brief CLoginData
 * \date 2008-02-09 17:15GMT
 * \author Jan Boon (Kaetemi)
 * CLoginData
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on NEVRAX SNOWBALLS, Copyright (C) 2001  Nevrax Ltd.
 * 
 * This file is part of OpenNeL Snowballs.
 * OpenNeL Snowballs is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * OpenNeL Snowballs is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenNeL Snowballs; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef SBCLIENT_LOGIN_DATA_H
#define SBCLIENT_LOGIN_DATA_H
#include <nel/misc/types_nl.h>
#include "configurable_component.h"

namespace SBCLIENT {

/**
 * \brief CLoginData
 * \date 2008-02-09 17:15GMT
 * \author Jan Boon (Kaetemi)
 * CLoginData
 */
class CLoginData : public IConfigurableComponent
{
protected:
	// pointers
	// ...
	
	// instances
	// ...
public:
	CLoginData(CComponentManager *manager, const std::string &instanceId, NLMISC::IProgressCallback &progressCallback);
	~CLoginData();
	void update();
	void render();
	void config(const std::string &varName, NLMISC::CConfigFile::CVar &var);
	void componentUp(IComponent *component);
	void componentDown(IComponent *component);
}; /* class CLoginData */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LOGIN_DATA_H */

/* end of file */
