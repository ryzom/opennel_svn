/**
 * \file landscape.cpp
 * \brief CLandscape
 * \date 2008-02-03 20:52GMT
 * \author Jan Boon (Kaetemi)
 * CLandscape
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

#include <nel/misc/types_nl.h>
#include "landscape.h"

// Project includes
#include "member_callback_impl.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/3d/u_landscape.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_driver.h>
#include <nel/misc/progress_callback.h>
#include <nel/misc/vector.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

CLandscape::CLandscape(IProgressCallback &progressCallback, 
const std::string &id, UDriver *driver,
TGlobalAnimationTime *globalAnimationTime) 
: _Config(id), _Driver(driver), GlobalAnimationTime(globalAnimationTime)
{
	nlassert(_Driver);
	nlassert(GlobalAnimationTime);
	progressCallback.progress(0.0f);

	// create a scene
	Scene = _Driver->createScene(false);
	nlassert(Scene);

	// create the landscape
	Landscape = Scene->createLandscape();
	nlassert(Landscape);

	// load the bank files
	Landscape->loadBankFiles(
		_Config.getVar("BankName").asString(), 
		_Config.getVar("FarBankName").asString());
	Landscape->invalidateAllTiles();

	//Landscape->postfixTileFilename("_sp");
	//Landscape->postfixTileVegetableDesc("_sp");

	// -- -- this doesn't do anything useful
	//////// setup the zone path
	//////Landscape->setZonePath(ConfigFile->getVar("DataPath").asString() + "zones/");

	Landscape->setupStaticLight(
		_Config.getValue("DiffuseColor", CRGBA(255, 255, 255, 255)),
		_Config.getValue("AmbiantColor", CRGBA(255, 255, 255, 255)),
		_Config.getValue("MultiplyFactor", 1.0f));

	_Config.setCallbackAndCall("ReceiveShadowMap", configReceiveShadowMap, this, NULL);
	_Config.setCallbackAndCall("TileNear", configTileNear, this, NULL);
	_Config.setCallbackAndCall("Threshold", configThreshold, this, NULL);
	_Config.setCallbackAndCall("Vision", configVision, this, NULL);
	_Config.setCallbackAndCall("VisionInitial", configVisionInitial, this, NULL);
}

CLandscape::~CLandscape()
{
	// release config callbacks
	_Config.dropCallback("ReceiveShadowMap");
	_Config.dropCallback("TileNear");
	_Config.dropCallback("Threshold");
	_Config.dropCallback("Vision");
	_Config.dropCallback("VisionInitial");

	// destroy the landscape
	Scene->deleteLandscape(Landscape);

	// destroy the scene
	_Driver->deleteScene(Scene);	
}

SBCLIENT_CALLBACK_IMPL(CLandscape, updateAnimations)
{
	Scene->animate(*GlobalAnimationTime);
}

SBCLIENT_CALLBACK_IMPL(CLandscape, updateLandscape)
{
	if (RefreshZonesAround)
		Landscape->refreshZonesAround(*RefreshZonesAround, _Vision);
}

SBCLIENT_CALLBACK_IMPL(CLandscape, renderScene)
{
	Scene->render();
}

void CLandscape::refresh(IProgressCallback &progressCallback)
{
	nlassert(RefreshZonesAround);
	std::vector<string> added;
	std::vector<string> removed;
	Landscape->refreshAllZonesAround(*RefreshZonesAround, _VisionInitial, added, removed, progressCallback);
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CLandscape, configReceiveShadowMap)
{
	Landscape->enableReceiveShadowMap(var.asBool());
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CLandscape, configTileNear)
{
	Landscape->setTileNear(var.asFloat());
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CLandscape, configThreshold)
{
	Landscape->setThreshold(var.asFloat());
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CLandscape, configVision)
{
	_Vision = var.asFloat();
}

SBCLIENT_CALLBACK_CONFIG_IMPL(CLandscape, configVisionInitial)
{
	_VisionInitial = var.asFloat();
}

} /* namespace SBCLIENT */

/* end of file */
