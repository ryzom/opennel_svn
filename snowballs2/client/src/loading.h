/**
 * \file loading.h
 * \brief CLoading
 * \date 2008-02-03 21:24GMT
 * \author Jan Boon (Kaetemi)
 * CLoading
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
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

#ifndef SBCLIENT_LOADING_H
#define SBCLIENT_LOADING_H
#include <nel/misc/types_nl.h>

#include "config_proxy.h"

namespace SBCLIENT {
	class CLoadingScreen;
	class CI18NHelper;

/**
 * \brief CLoading
 * \date 2008-02-03 21:24GMT
 * \author Jan Boon (Kaetemi)
 * CLoading
 */
class CLoading
{
protected:
	// pointers
	CLoadingScreen *_LoadingScreen; // not deleted here
	CI18NHelper *_I18N; // not deleted here
	
	// instances
	CConfigProxy _Config;
	// message ids
	uint _State;
	// background ids
	uint _NeL;
	uint _SnowballsLoading;
	uint _SnowballsUnloading;
public:
	CLoading(CLoadingScreen &loadingScreen, const std::string &id, CI18NHelper *i18n);
	virtual ~CLoading();

	void setMessageState(const std::string &label);

	void setBackgroundNeL();
	void setBackgroundSnowballsLoading();
	void setBackgroundSnowballsUnloading();
	void setBackground(const std::string id);
	//void setBackgroundEnteringOldContinent();
	//void setBackgroundEnteringSnowContinent();
	//void setBackgroundEnteringWaterContinent();
	//void setBackgroundEnteringFireContinent();

}; /* class CLoading */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LOADING_H */

/* end of file */
