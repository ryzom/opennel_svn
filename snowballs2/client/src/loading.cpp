/**
 * \file loading.cpp
 * \brief MLoading
 * \date 2008-02-03 21:24GMT
 * \author Jan Boon (Kaetemi)
 * MLoading
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

#include <nel/misc/types_nl.h>
#include "loading.h"

#include "loading_screen.h"
#include "string_identifier.h"
#include "i18n_helper.h"

// #include <nel/misc/debug.h>
#include <nel/misc/config_file.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

MLoading::MLoading(CLoadingScreen &loadingScreen, const std::string &id, CI18NHelper *i18n)
: _LoadingScreen(&loadingScreen), _I18N(i18n), _Config(id)
{
	// message ids
	_State = CStringIdentifier::get("State");
	// background ids
	_NeL = CStringIdentifier::get("NeL");
	_Snowballs = CStringIdentifier::get("Snowballs");

	CConfigFile::CVar &backgrounds = _Config.getVar("Background");
	uint bgcount = backgrounds.size();
	for (uint i = 0; i < bgcount; ++i)
	{
		string bgprefix = string("Background_") + backgrounds.asString(i) + "_";
		loadingScreen.addBackground(CStringIdentifier::get(backgrounds.asString(i)),
			_Config.getVar(bgprefix + "EmptyFile").asString(),
			_Config.getVar(bgprefix + "EmptySize").asInt(0),
			_Config.getVar(bgprefix + "EmptySize").asInt(1),
			_Config.getVar(bgprefix + "EmptyBorder").asInt(0),
			_Config.getVar(bgprefix + "EmptyBorder").asInt(1),
			_Config.getVar(bgprefix + "FullFile").asString(),
			_Config.getVar(bgprefix + "FullSize").asInt(0),
			_Config.getVar(bgprefix + "FullSize").asInt(1),
			_Config.getVar(bgprefix + "FullBorder").asInt(0),
			_Config.getVar(bgprefix + "FullBorder").asInt(1),
			_Config.getVar(bgprefix + "Side").asFloat(0),
			_Config.getVar(bgprefix + "Side").asFloat(1));
	}
	CConfigFile::CVar &messages = _Config.getVar("Message");
	uint msgcount = messages.size();
	for (uint i = 0; i < msgcount; ++i)
	{
		string msgprefix = string("Message_") + messages.asString(i) + "_";
		NL3D::UTextContext::THotSpot hot_spot;
		std::string hot_spot_string = _Config.getVar(msgprefix + "HotSpot").asString();
		if (hot_spot_string == "BottomLeft") hot_spot = NL3D::UTextContext::BottomLeft;
		else if (hot_spot_string == "BottomRight") hot_spot = NL3D::UTextContext::BottomRight;
		else if (hot_spot_string == "MiddleBottom") hot_spot = NL3D::UTextContext::MiddleBottom;
		else if (hot_spot_string == "MiddleLeft") hot_spot = NL3D::UTextContext::MiddleLeft;
		else if (hot_spot_string == "MiddleMiddle") hot_spot = NL3D::UTextContext::MiddleMiddle;
		else if (hot_spot_string == "MiddleRight") hot_spot = NL3D::UTextContext::MiddleRight;
		else if (hot_spot_string == "MiddleTop") hot_spot = NL3D::UTextContext::MiddleTop;
		else if (hot_spot_string == "TopLeft") hot_spot = NL3D::UTextContext::TopLeft;
		else if (hot_spot_string == "TopRight") hot_spot = NL3D::UTextContext::TopRight;
		else nlerror("Invalid HotSpot");
		loadingScreen.addMessage(CStringIdentifier::get(messages.asString(i)),
			_Config.getVar(msgprefix + "Location").asFloat(0),
			_Config.getVar(msgprefix + "Location").asFloat(1),
			NLMISC::CRGBA(
				_Config.getVar(msgprefix + "Color").asInt(0),
				_Config.getVar(msgprefix + "Color").asInt(1),
				_Config.getVar(msgprefix + "Color").asInt(2),
				_Config.getVar(msgprefix + "Color").asInt(3)),
			_Config.getVar(msgprefix + "Size").asInt(0),
			hot_spot);
		loadingScreen.setMessage(CStringIdentifier::get(messages.asString(i)),
			_Config.getVar(msgprefix + "Message").asString());
	}
}

MLoading::~MLoading()
{
	
}

void MLoading::setMessageState(const std::string &label)
{
	_LoadingScreen->setMessage(_State, _I18N->get(label));
}

void MLoading::setBackgroundNeL()
{
	_LoadingScreen->setBackground(_NeL);
}

void MLoading::setBackgroundSnowballs()
{
	_LoadingScreen->setBackground(_Snowballs);
}

} /* namespace SBCLIENT */

/* end of file */
