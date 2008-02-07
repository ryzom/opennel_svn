/** \file client.cpp
 * Snowballs main file
 *
 * $Id$
 */

/* Copyright, 2001 Nevrax Ltd.
 *
 * This file is part of NEVRAX SNOWBALLS.
 * NEVRAX SNOWBALLS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX SNOWBALLS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX SNOWBALLS; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

//
// Includes
// 

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef SNOWBALLS_CONFIG
#define SNOWBALLS_CONFIG ""
#endif // SNOWBALLS_CONFIG

#include <nel/misc/types_nl.h>

#ifdef NL_OS_WINDOWS
#	define NOMINMAX
#	include <windows.h>
#endif

#include <ctime>
#include <string>
#include <vector>

#include <nel/misc/md5.h>
#include <nel/misc/path.h>
#include <nel/misc/file.h>
#include <nel/misc/vectord.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/command.h>
#include <nel/misc/config_file.h>

#include <nel/net/login_client.h>

#include <nel/3d/u_scene.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_material.h>
#include <nel/3d/u_text_context.h>

#include "_pacs.h"
#include "_radar.h"
#include "_graph.h"
#include "_camera.h"
#include "_compass.h"
#include "_commands.h"
#include "_entities.h"
#include "_network.h"
#include "_landscape.h"
#include "_animation.h"
#include "_interface.h"
#include "lens_flare.h"
#include "_mouse_listener.h"

#ifdef NL_OS_WINDOWS
// Sound currently disabled under Linux
#	include "_sound.h"
#endif


//#include "component_manager.h"
//#include "landscape_component.h"
//using namespace SBCLIENT;
//extern SBCLIENT::CLandscapeComponent *LandscapeComponent;

//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NL3D;
using namespace NLNET;

// 
// Constants
// 

static const uint8 GameStateLoad = 0, GameStateUnload = 1, GameStateReset = 2, GameStateExit = 3, 
GameStateLogin = 4, GameStateOnline = 5, GameStateOffline = 6;


// Temp
ULandscape *Landscape;
//
//// Component temp test
//CComponentManager *ComponentManager;

//
// Globals
//

// This class contains all variables that are in the client.cfg
CConfigFile *ConfigFile; // core
// The 3d driver
UDriver *Driver = NULL; // core
// This variable is used to display text on the screen
UTextContext *TextContext = NULL; // core
// This is the main scene
UScene *Scene = NULL; // ingame
// This class is used to handle mouse/keyboard input for camera movement
C3dMouseListener *MouseListener = NULL; // ingame
// The previous and current frame dates
TTime  LastTime, NewTime, DiffTime; // ingame
// true if the commands(chat) interface must be display. This variable is set automatically with the config file
bool ShowCommands = true; // ingame
// if true, the mouse can't go out the client window(work only on Windows)
bool CaptureState = false; // ingame
// Stuff for connection
//ucstring Login;
//static string FSAddr, Cookie;

//
// Prototypes
//

void initLoadingState();
void releaseLoadingState();
void renderLoadingState(char *state, bool logo3d);
void updateLoadingState(char *state, bool network, bool information);
void displayLoadingState(char *state);
void renderLoadingState(ucstring state, bool logo3d);
void updateLoadingState(ucstring state, bool network, bool information);
void displayLoadingState(ucstring state);
void update3dLogo();
void renderInformation();
void switchGameState();
void loopLogin();
void loopIngame();
void initCore();
void initLogin();
void initIngame();
void initOnline();
void initOffline();
void releaseCore();
void releaseLogin();
void releaseIngame();
void releaseOnline();
void releaseOffline();
void cbGraphicsDriver(CConfigFile::CVar &var);

void updateTime(void *context, void *tag)
{
	LastTime = NewTime;
	NewTime = CTime::getLocalTime();
	DiffTime = NewTime - LastTime;
}

//
// Functions
//

//void loopLogin()
//{
//#ifdef NL_OS_WINDOWS
//	playMusic(SBCLIENT_MUSIC_LOGIN);
//#endif
//	// todo: login screen, move this stuff to a button or something
//	displayLoadingState("Login");
//	if (ConfigFile->getVar("Local").asInt() == 0)
//	{
//		if (ConfigFile->getVar("UseDirectClient").asInt() == 1)
//		{
//			string result;
//			string LSHost(ConfigFile->getVar("LSHost").asString());
//			//Login = ConfigFile->getVar("Login").asString();
//			ucstring Password = ConfigFile->getVar("Password").asString();
//			CHashKeyMD5 hk = getMD5((uint8*)Password.c_str(), Password.size());
//			string CPassword = hk.toString();
//			nlinfo("The crypted password is %s", CPassword.c_str());
//			string Application = ConfigFile->getVar("ClientApplication").asString();
//			sint32 sid = ConfigFile->getVar("ShardId").asInt();
//
//			// 1/ Authenticate
//			updateLoadingState(ucstring("Authenticate"), false, false);
//			//result = CLoginClient::authenticateBegin(LSHost, Login, CPassword, Application);
//			if (!result.empty()) goto AuthenticateFail;
//			while (CLoginClient::authenticateUpdate(result))
//				updateLoadingState(ucstring("Authenticate"), false, false);
//			if (!result.empty()) goto AuthenticateFail;
//
//			goto AuthenticateSuccess;
//AuthenticateFail:
//			nlinfo("*** Authenticate failed '%s' ***", result.c_str());
//			for (TTime t = 0; t < 5000; t += DiffTime)
//				updateLoadingState(ucstring("Authenticate failed: ") + ucstring(result), false, false);
//			NextGameState = GameStateOffline;
//			return;
//AuthenticateSuccess:
//
//			nlinfo("%d Shards are available:", CLoginClient::ShardList.size());
//			for (uint i = 0; i < CLoginClient::ShardList.size(); i++)
//			{
//				nlinfo("    ShardId %3d: %s(%d online players)", CLoginClient::ShardList[i].Id, CLoginClient::ShardList[i].Name.toUtf8().c_str(), CLoginClient::ShardList[i].NbPlayers);
//			}
//
//			// 2/ Select shard
//			updateLoadingState(ucstring("Select shard"), false, false);
//			result = CLoginClient::selectShardBegin(sid);
//			if (!result.empty()) goto SelectFail;
//	/*		while (CLoginClient::selectShardUpdate(result, FSAddr, Cookie))
//				updateLoadingState(ucstring("Select shard"), false, false);*/
//			if (!result.empty()) goto SelectFail;
//			goto SelectSuccess;
//SelectFail:
//			nlinfo("*** Connection to the shard failed '%s' ***", result.c_str());
//			for (TTime t = 0; t < 5000; t += DiffTime)
//				updateLoadingState(ucstring("Select shard failed: ") + ucstring(result), false, false);
//			NextGameState = GameStateOffline;
//			return;
//SelectSuccess:;
//		}
//		NextGameState = GameStateOnline;
//		return;
//	}
//	NextGameState = GameStateOffline;
//	return;
//}

void renderClient(void *context, void *tag)
{
	//updateLensFlare(); // Render the lens flare
	updateCompass(); // Update the compass		
	updateRadar(); // Update the radar
	updateGraph(); // Update the radar		
	if (ShowCommands) updateCommands(); // Update the commands panel
	updateAnimation();		
	renderEntitiesNames(); // Render the name on top of the other players		
	updateInterface(); // Update interface
	renderInformation();
	update3dLogo();
	// Update network messages
}

void updateClient(void *context, void *tag)
{
	updateNetwork();
	// Manage the keyboard
	if (Driver->AsyncListener.isKeyDown(KeySHIFT) && Driver->AsyncListener.isKeyDown(KeyESCAPE))
	{
		// Shift Escape -> quit
		/*NextGameState = GameStateExit;*/
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF3))
	{
		// F3 -> toggle wireframe/solid
		UDriver::TPolygonMode p = Driver->getPolygonMode();
		p = UDriver::TPolygonMode(((int)p+1)%3);
		Driver->setPolygonMode(p);
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF4))
	{
		// F4 -> clear the command(chat) output
		clearCommands();
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF5))
	{
		// F5 -> display/hide the commands(chat) interface
		ShowCommands = !ShowCommands;
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF6))
	{
		// F6 -> display/hide the radar interface
		RadarState =(RadarState + 1) % 3;
	}
	else if (Driver->AsyncListener.isKeyDown(KeyF7))
	{
		// F7 -> radar zoom out
		RadarDistance += 50;
	}
	else if (Driver->AsyncListener.isKeyDown(KeyF8))
	{
		// F8 -> radar zoom in
		RadarDistance -= 50;
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF9))
	{
		// F9 -> release/capture the mouse cursor
		if (!CaptureState)
		{
			Driver->setCapture(false);
			Driver->showCursor(true);
			MouseListener->removeFromServer(Driver->EventServer);
		}
		else
		{
			Driver->setCapture(true);
			Driver->showCursor(false);
			MouseListener->addToServer(Driver->EventServer);
		}
		CaptureState = !CaptureState;
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF10))
	{
		// F10 -> switch beteen online and offline
		/*if (isOnline()) NextGameState = GameStateOffline;
		else NextGameState = GameStateLogin;*/
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF11))
	{
		// F11 -> reset the PACS global position of the self entity(in case of a pacs failure :-\)
		if (Self != NULL)
			resetEntityPosition(Self->Id);
	}
	else if (Driver->AsyncListener.isKeyPushed(KeyF12))
	{
		// F12 -> take a screenshot
		// _DriverComponent->saveScreenshot("screenshot", false);
	}


	// Check if the config file was modified by another program
	CConfigFile::checkConfigFiles();

	// Update all entities positions
	MouseListener->update();
	updateEntities();

	// setup the camera
	// -> first update camera position directly from the mouselistener
	// -> then update stuffs linked to the camera(snow, sky, lens flare etc.)
	MouseListener->updateCamera();
}

void renderInformation()
{
	// Display if we are online or offline
	TextContext->setHotSpot(UTextContext::TopRight);
	TextContext->setColor(isOnline()?CRGBA(0, 255, 0):CRGBA(255, 0, 0));
	TextContext->setFontSize(18);
	TextContext->printfAt(0.99f, 0.99f, isOnline() ? "Online" : "Offline");

	// Display the frame rate
	float fps = 1000.0f /(float)DiffTime;
	TextContext->setHotSpot(UTextContext::TopLeft);
	TextContext->setColor(CRGBA(255, 255, 255, 255));
	TextContext->setFontSize(14);
	TextContext->printfAt(0.01f, 0.99f, "%.2ffps %ums", fps,(uint32)DiffTime);

	// one more frame
	FpsGraph.addValue(1.0f);
	SpfGraph.addOneValue((float)DiffTime);
}

// 
// Configuration callbacks
// 

void cbGraphicsDriver(CConfigFile::CVar &var)
{

}

//
// Loading state procedure
//

static UTextureFile *NelLogo = NULL;
static UTextureFile *NevraxLogo = NULL;
static float ScreenWidth, ScreenHeight;
// The logo 3D objects
static UScene				*LogoScene = NULL;
static UInstance			Logo = NULL;

void initLoadingState()
{

}

void releaseLoadingState()
{

}

void renderLoadingState(char *state, bool logo3d)
{

}
void renderLoadingState(ucstring state, bool logo3d)
{

}

void displayLoadingState(char *state)
{

}
void displayLoadingState(ucstring state)
{

}

void updateLoadingState(char *state, bool network, bool information)
{

}
void updateLoadingState(ucstring state, bool network, bool information)
{

}

void update3dLogo()
{

}
