/**
 * \file snowballs_client.cpp
 * \brief CSnowballsClient
 * \date 2008-02-07 17:21GMT
 * \author Jan Boon (Kaetemi)
 * CSnowballsClient
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

// Priority Distribution
// 
// 01. [+9000000 - +9099999] Update Utilities (configuration etc)
#define SBCLIENT_UPDATE_UTILITIES 9050000
// 02. [+8000000 - +8099999] Update Time (deltas)
#define SBCLIENT_UPDATE_TIME 8050000
// 03. [+7000000 - +7099999] Update Driver (keyboard controls, etc)
#define SBCLIENT_UPDATE_DRIVER 7050000
// 04. [+6000000 - +6099999] Update Incoming (network, receive messages)
#define SBCLIENT_UPDATE_INCOMING 6050000
// 05. [+5000000 - +5099999] Update Weather (sky, snow, wind, fog, sun)
#define SBCLIENT_UPDATE_WEATHER 5050000
// 06. [+4000000 - +4099999] Update Landscape (async zone loading near entity)
#define SBCLIENT_UPDATE_LANDSCAPE 4050000
// 07. [+3000000 - +3099999] Update Entities (collisions and actions)
//                           - Move Other Entities (move//, animations, etc)
//                           - Update Self Collision (move)
#define SBCLIENT_UPDATE_ENTITIES 3050000
//                           - Update Self Entity (animations//, etc)
// 08. [+2000000 - +2099999] Update Animations (playlists)
#define SBCLIENT_UPDATE_ANIMATIONS 2050000
// 09. [+1500000 - +1599999] Update Interface (login, ui, etc)
#define SBCLIENT_UPDATE_INTERFACE 1550000
// 10. [+1200000 - +1299999] Update Sound (sound driver)
#define SBCLIENT_UPDATE_SOUND 1250000
// 11. [+1000000 - +1099999] Update Outgoing (network, send new position etc)
#define SBCLIENT_UPDATE_OUTGOING 1050000
// 12. [-1000000 - +0999999] Update Debug (stuff for dev)
#define SBCLIENT_UPDATE_DEBUG 0
//
// 01. [+8000000 - +8099999] Render Driver (background black, done here)
#define SBCLIENT_RENDER_DRIVER 8050000
// 02. [+7000000 - +7099999] Render Sky (sky scene)
#define SBCLIENT_RENDER_SKY 7050000
// 03. [+6000000 - +6099999] Render Landscape (landscape zones)
#define SBCLIENT_RENDER_LANDSCAPE 6050000
// 04. [+5000000 - +5099999] Render Scene (entity scene)
#define SBCLIENT_RENDER_SCENE 5050000
// 05. [+4000000 - +4099999] Render Effects (flare)
#define SBCLIENT_RENDER_EFFECTS 4050000
// 06. [+3000000 - +3099999] Render Interface 3D (player names)
#define SBCLIENT_RENDER_INTERFACE_3D 3050000
//                           - Might need switch with Effects due to Z
// 07. [+2000000 - +2099999] Render Interface 2D (chatboxes etc, optionally does have 3d)
#define SBCLIENT_RENDER_INTERFACE_2D 2050000
// 08. [-1000000 - +0999999] Render Debug (stuff for dev)
#define SBCLIENT_RENDER_DEBUG 0

#include <nel/misc/types_nl.h>
#include "snowballs_client.h"
#include "snowballs_config.h"

#include "i18n_helper.h"
#include "config_manager.h"
#include "config_proxy.h"
#include "member_callback_impl.h"
#include "command_wrapper.h"

#include "loading.h"
#include "graphics.h"
#include "sound.h"
#include "time.h"
#include "landscape.h"
#include "collisions_old.h"
#include "animation_old.h"
#include "entities_old.h"

#include <nel/misc/config_file.h>
#include <nel/misc/path.h>
#include <nel/misc/debug.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/i18n.h>
#include <nel/misc/app_context.h>

#include <nel/3d/u_driver.h>
#include <nel/3d/u_text_context.h>

// temp
#include <nel/3d/u_texture.h>
#include <nel/misc/geom_ext.h>
#include <nel/3d/u_material.h>

using namespace std;
using namespace NLMISC;
using namespace NL3D;

namespace SBCLIENT {

struct CSnowballsDebug
{
protected:
#if SBCLIENT_USE_LOG
	NLMISC::CFileDisplayer *_FileDisplayer; // deleted here
#endif
public:
	CSnowballsDebug::CSnowballsDebug()		
#if SBCLIENT_USE_LOG
	: _FileDisplayer(NULL)
#endif
	{
		// use log.log if NEL_LOG_IN_FILE and SBCLIENT_USE_LOG_LOG defined as 1
		createDebug(NULL, SBCLIENT_USE_LOG_LOG, false);
		srand((uint)NLMISC::CTime::getLocalTime());

#if SBCLIENT_USE_LOG
		// create snowballs_client.log
		// filedisplayer only deletes the 001 etc
		if (CFile::isExists(SBCLIENT_LOG_FILE))
			CFile::deleteFile(SBCLIENT_LOG_FILE);
		// initialize the log file
		_FileDisplayer = new CFileDisplayer();
		_FileDisplayer->setParam(SBCLIENT_LOG_FILE, SBCLIENT_ERASE_LOG);
		DebugLog->addDisplayer(_FileDisplayer);
		InfoLog->addDisplayer(_FileDisplayer);
		WarningLog->addDisplayer(_FileDisplayer);
		AssertLog->addDisplayer(_FileDisplayer);
		ErrorLog->addDisplayer(_FileDisplayer);
#endif
		nlinfo("Starting Snowballs!");
		// end of debug/log initialization
	}

	CSnowballsDebug::~CSnowballsDebug()
	{	
		// begin of debug/log destruction
		nlinfo("See you later!");
#if SBCLIENT_USE_LOG
		nlassert(_FileDisplayer);
		DebugLog->removeDisplayer(_FileDisplayer);
		InfoLog->removeDisplayer(_FileDisplayer);
		WarningLog->removeDisplayer(_FileDisplayer);
		AssertLog->removeDisplayer(_FileDisplayer);
		ErrorLog->removeDisplayer(_FileDisplayer);
		delete _FileDisplayer;
#endif		
	}
};

CSnowballsClient::CSnowballsClient() :
// utilities
_StringIdentifier(), _LoadingScreen(), _LoginData(), 
_UpdateFunctions(), _RenderFunctions(), 
_I18NHelper(NULL), _Config(NULL), _ConfigManager(NULL), 
// special function ids
_UpdateUtilitiesId(0), _UpdateDebugId(0), _RenderDebugId(0), 
// components and their function ids
_Loading(NULL), 
_Graphics(NULL), _GraphicsUpdateDriverId(0), 
_Sound(NULL), _SoundUpdateSoundId(0), 
_Time(NULL), _TimeUpdateTimeId(0), 
_Login(NULL), _LoginUpdateInterfaceId(0), _LoginRenderInterfaceId(0), _LoginUpdateNetworkId(0), 
_Landscape(NULL), _LandscapeUpdateAnimationsId(0), _LandscapeUpdateLandscapeId(0), _LandscapeUpdateSceneId(0), 
_Collisions(NULL), 
_Animation(NULL), _AnimationUpdateAnimationsId(0), 
_Entities(NULL), _EntitiesUpdateEntitiesId(0), 
// commands
_SetStateCommand(NULL), 
// states
_LoadedUtils(false), _LoadedBase(false), _LoadedLogin(false), 
_LoadedIngame(false), _LoadedConnection(false), 
_EnabledUtils(false), _EnabledBase(false), _EnabledLogin(false), 
_EnabledIngame(false), _EnabledConnection(false) // do not change these
{
	Invalid = CStringIdentifier::get("Invalid");
	Load = CStringIdentifier::get("Load");
	Reset = CStringIdentifier::get("Reset");
	Exit = CStringIdentifier::get("Exit");
	Login = CStringIdentifier::get("Login");
	Game = CStringIdentifier::get("Game");
	_CurrentState = Invalid;
	_NextState = Load;
}

CSnowballsClient::~CSnowballsClient()
{
	// just to be sure ;)
	disableAll();
	unloadAll();
}

int CSnowballsClient::run()
{
	// load and unload required components for the current game state
	// and call the componentmanager update until the state changes
SwitchState:
	nlinfo("Switching to the next game state");
	if (_CurrentState == _NextState)
	{
		nlwarning("NextGameState wasn't changed");
	}
	else
	{
		uint ns = _NextState;
		// force unloads for current state
		if (_CurrentState == Game)
		{
			disableConnection();
			unloadConnection();
		}

		// load and unload for next state
		if (_NextState == Load)
		{
			loadUtils();
			enableUtils();
			loadBase();
			enableBase();
		}
		else if (_NextState == Reset)
		{
			// displayLoadingState("Reset");
			disableAll();
			unloadAll();
		}
		else if (_NextState == Exit)
		{
			// displayLoadingState("See you later!");
			disableAll();
			unloadAll();
		}
		else if (_NextState == Login)
		{
			loadUtils(); // core is required
			enableUtils();
			loadBase();
			enableBase();
			disableIngame(); // ingame not needed to render and update
			// unloadIngame(); // don't uncomment this, we keep the data :)
			loadLogin(); // login is required
			enableLogin();
		}
		else if (_NextState == Game)
		{
			loadUtils(); // core is required
			enableUtils();
			loadBase();
			enableBase();
			disableLogin();
			unloadLogin(); //login can be released
			loadIngame(); // ingame is required
			enableIngame();
			loadConnection();
			enableConnection();
		}
		_CurrentState = ns;
		if (_NextState != ns) 
		{
			nlinfo("State changed while loading");
			goto SwitchState;
		}
		_LoadingScreen.setBackground(0);
	}

	// handle special states
	if (_CurrentState == Load) // switch to the default state
	{
		_NextState = Login;
	}
	else if (_CurrentState == Reset) // used to reset everything
	{
		_NextState = Load;
	}
	else if (_CurrentState ==  Exit) // exit the loop
	{
		return EXIT_SUCCESS;
	}
	else
	{
		do { if (!_Graphics->Driver->isActive()) 
				{ _NextState = Exit; goto SkipLoop; }
			// call all update functions
			_UpdateFunctions.execute();
			// if driver is lost (d3d) do nothing for a while
			if (_Graphics->Driver->isLost()) nlSleep(10);
			else
			{
				// clear all buffers
				_Graphics->Driver->clearBuffers(CRGBA(0, 0, 0, 255));

				// call all render thingies
				_RenderFunctions.execute();
			
				// swap 3d buffers
				_Graphics->Driver->swapBuffers();
			}
SkipLoop:;
		} while (_CurrentState == _NextState);
	}
	goto SwitchState;
}

void CSnowballsClient::loadUtils()
{
	if (!_LoadedUtils)
	{
		_LoadedUtils = true;
		// init configuration file
		_ConfigManager = new CConfigManager(SBCLIENT_CONFIG_FILE, SBCLIENT_CONFIG_DEFAULT_FILE);
		_Config = new CConfigProxy(SBCLIENT_NAME);

		// set the search paths (kinda important)
		CConfigFile::CVar *var;
		var = _Config->getVarPtr("SearchPaths");
		uint varsize = var->size();
		for (uint i = 0; i < varsize; ++i)
			CPath::addSearchPath(var->asString(i), true, false);
		var = _Config->getVarPtr("RemapExtensions");
		varsize = var->size();
		for (uint i = 0; i < varsize; i += 2)
			CPath::remapExtension(var->asString(i), var->asString(i + 1), true);

		// set the language code
		CI18N::load(_Config->getVar("LanguageCode").asString());
		_I18NHelper = new CI18NHelper("i18n", false);
		_I18NHelper->set("VERSION", ucstring(SBCLIENT_VERSION_STRING));

		// load the loading screen manager
		nlassert(!_Loading);
		_Loading = new CLoading(_LoadingScreen, "Loading", _I18NHelper);
		nlassert(_Loading);

		// register commands
		nlassert(!_SetStateCommand);
		_SetStateCommand = new CCommandWrapper(SBCLIENT_NAME,
			_Config->getValue("SetStateCommand", string("set_state")).c_str(),
			_I18NHelper->get("i18nSetStateCommandHelp").toString().c_str(),
			_I18NHelper->get("i18nSetStateCommandVariables").toString().c_str(),
			CSnowballsClient::commandSetState, this, NULL);
		nlassert(_SetStateCommand);
	}
}

void CSnowballsClient::unloadUtils()
{
	if (_LoadedUtils)
	{
		// unregister commands
		nlassert(_SetStateCommand); delete _SetStateCommand; _SetStateCommand = NULL;

		// unload the loading screen manager
		nlassert(_Loading); delete _Loading; _Loading = NULL;

		// if they're null, something must've seriously went wrong at some point
		// release of the language data is done when loading new file
		nlassert(_I18NHelper); delete _I18NHelper;

		// release the configuration file
		nlassert(_Config); delete _Config;
		nlassert(_ConfigManager); delete _ConfigManager;

		// release the search paths etc
		CPath::releaseInstance();

		_LoadedUtils = false;
	}
}

void CSnowballsClient::enableUtils()
{
	if (!_EnabledUtils)
	{
		_EnabledUtils = true;
		
		nlassert(!_UpdateDebugId);
		_UpdateDebugId = _UpdateFunctions.add(
			updateDebug, this, NULL, 0);
		
		nlassert(!_RenderDebugId);
		_RenderDebugId = _RenderFunctions.add(
			renderDebug, this, NULL, 0);
		
		nlassert(!_UpdateUtilitiesId);
		_UpdateUtilitiesId = _UpdateFunctions.add(
			updateUtilities, this, NULL, 9050000);
	}
}

void CSnowballsClient::disableUtils()
{
	if (_EnabledUtils)
	{
		nlassert(_UpdateUtilitiesId);
		_UpdateFunctions.remove(_UpdateUtilitiesId);
		_UpdateUtilitiesId = 0;

		nlassert(_RenderDebugId);
		_RenderFunctions.remove(_RenderDebugId);
		_RenderDebugId = 0;

		nlassert(_UpdateDebugId);
		_UpdateFunctions.remove(_UpdateDebugId);
		_UpdateDebugId = 0;

		_EnabledUtils = false;
	}
}

void CSnowballsClient::loadBase()
{
	if (!_LoadedBase)
	{		
		_LoadedBase = true;
		_Loading->setBackgroundNeL();
		_Loading->setMessageState("");

		_LoadingScreen.setRange(0.00f, 0.45f);
		/* _Loading->setMessageState("InitializeGraphics"); */
		nlassert(!_Graphics);
		_Graphics = new CGraphics(_LoadingScreen, "Graphics", _I18NHelper);
		nlassert(_Graphics);
		_LoadingScreen.setDriver(_Graphics->Driver);
		_LoadingScreen.setTextContext(_Graphics->TextContext);

		_LoadingScreen.setRange(0.45f, 0.90f);
		/* _Loading->setMessageState("InitializeSound"); */
		nlassert(!_Sound);
		_Sound = new CSound(_LoadingScreen, "Sound");
		nlassert(_Sound);

		_LoadingScreen.setRange(0.90f, 1.00f);
		/* _Loading->setMessageState("InitializeTime"); */
		nlassert(!_Time);
		_Time = new SBCLIENT::CTime(_LoadingScreen);
		nlassert(_Time);

		_LoadingScreen.progress(1.0f);
	}
}

void CSnowballsClient::unloadBase()
{
	if (_LoadedBase)
	{
		nlassert(_Time); delete _Time; _Time = NULL;
		nlassert(_Sound); delete _Sound; _Sound = NULL;
		_LoadingScreen.setTextContext(NULL);
		_LoadingScreen.setDriver(NULL);
		nlassert(_Graphics); delete _Graphics; _Graphics = NULL;
		
		_LoadedBase = false;
	}
}

void CSnowballsClient::enableBase()
{
	if (!_EnabledBase)
	{		
		_EnabledBase = true;

		nlassert(!_GraphicsUpdateDriverId);
		_GraphicsUpdateDriverId = _UpdateFunctions.add(
			SBCLIENT::CGraphics::updateDriver, _Graphics, NULL, 0 + SBCLIENT_UPDATE_DRIVER);

		nlassert(!_SoundUpdateSoundId);
		_SoundUpdateSoundId = _UpdateFunctions.add(
			SBCLIENT::CSound::updateSound, _Sound, NULL, 0 + SBCLIENT_UPDATE_SOUND);

		nlassert(!_TimeUpdateTimeId);
		_TimeUpdateTimeId = _UpdateFunctions.add(
			SBCLIENT::CTime::updateTime, _Time, NULL, 0 + SBCLIENT_UPDATE_TIME);
	}
}

void CSnowballsClient::disableBase()
{
	if (_EnabledBase)
	{
		nlassert(_TimeUpdateTimeId);
		_UpdateFunctions.remove(_TimeUpdateTimeId);
		_TimeUpdateTimeId = 0;

		nlassert(_SoundUpdateSoundId);
		_UpdateFunctions.remove(_SoundUpdateSoundId);
		_SoundUpdateSoundId = 0;

		nlassert(_GraphicsUpdateDriverId);
		_UpdateFunctions.remove(_GraphicsUpdateDriverId);
		_GraphicsUpdateDriverId = 0;
		
		_EnabledBase = false;
	}
}

void CSnowballsClient::loadLogin()
{
	if (!_LoadedLogin)
	{		
		_LoadedLogin = true;
		if (_CurrentState == Game) 
			_Loading->setBackgroundSnowballsUnloading();
		else _Loading->setBackgroundNeL();
		_Loading->setMessageState("");

		_LoadingScreen.setRange(0.0f, 1.0f);
		/* _Loading->setMessageState("InitializeLogin"); */
		nlassert(!_Login);
		_Login = new CLogin("Login", _Time, _Graphics->Driver, 
			_Graphics->TextContext, _I18NHelper, &_LoginData);
		nlassert(_Login);

		_LoadingScreen.progress(1.0f);
	}
}

void CSnowballsClient::unloadLogin()
{
	if (_LoadedLogin)
	{
		nlassert(_Login); delete _Login; _Login = NULL;
		
		_LoadedLogin = false;
	}
}

void CSnowballsClient::enableLogin()
{
	if (!_EnabledLogin)
	{		
		_EnabledLogin = true;

		nlassert(!_LoginUpdateInterfaceId);
		_LoginUpdateInterfaceId = _UpdateFunctions.add(
			CLogin::updateInterface, _Login, NULL, 1550000);

		nlassert(!_LoginRenderInterfaceId);
		_LoginRenderInterfaceId = _RenderFunctions.add(
			CLogin::renderInterface, _Login, NULL, 2050000);

		nlassert(!_LoginUpdateNetworkId);
		_LoginUpdateNetworkId = _UpdateFunctions.add(
			CLogin::updateNetwork, _Login, NULL, 6050000);

		_Login->enable();
	}
}

void CSnowballsClient::disableLogin()
{
	if (_EnabledLogin)
	{
		_Login->disable();

		nlassert(_LoginUpdateNetworkId);
		_UpdateFunctions.remove(_LoginUpdateNetworkId);
		_LoginUpdateNetworkId = 0;

		nlassert(_LoginRenderInterfaceId);
		_RenderFunctions.remove(_LoginRenderInterfaceId);
		_LoginRenderInterfaceId = 0;

		nlassert(_LoginUpdateInterfaceId);
		_UpdateFunctions.remove(_LoginUpdateInterfaceId);
		_LoginUpdateInterfaceId = 0;
		
		_EnabledLogin = false;
	}
}

void CSnowballsClient::loadIngame()
{
	if (!_LoadedIngame)
	{		
		_LoadedIngame = true;
		_Loading->setBackgroundSnowballsLoading();
		float max_progress = 4.f;

		_LoadingScreen.setRange(0.f / max_progress, 1.f / max_progress);
		_Loading->setMessageState("InitializeLandscape");
		nlassert(!_Landscape);
		_Landscape = new CLandscape(_LoadingScreen, "Landscape", 
			_Graphics->Driver, &_Time->AnimationTime);
		nlassert(_Landscape);

		_LoadingScreen.setRange(1.f / max_progress, 2.f / max_progress);
		_Loading->setMessageState("InitializeCollisions");
		nlassert(!_Collisions);
		_Collisions = new CCollisionsOld(_LoadingScreen, "Collisions", 
			_Landscape->Scene, _Landscape->Landscape);
		nlassert(_Collisions);

//		// init weather
//		displayLoadingState("Initialize Weather");
//		WeatherComponent = new CWeatherComponent(
//			_ComponentManager, "Weather", _LoadingScreen);
//		_ComponentManager->registerComponent(WeatherComponent);
//		_ComponentManager->registerRender(WeatherComponent, -250);
//		_ComponentManager->registerUpdate(WeatherComponent, 5000);

		_LoadingScreen.setRange(2.f / max_progress, 3.f / max_progress);
		_Loading->setMessageState("InitializeAnimation");
		nlassert(!_Animation);
		_Animation = new CAnimationOld(_LoadingScreen,
			_Graphics->Driver, _Landscape->Scene);
		nlassert(_Animation);

		_LoadingScreen.setRange(3.f / max_progress, 4.f / max_progress);
		_Loading->setMessageState("InitializeEntities");
		nlassert(!_Entities);
		_Entities = new CEntitiesOld(_LoadingScreen, 
			_Landscape->Scene, _Collisions->VisualCollisionManager, 
			_Collisions->MoveContainer, _Collisions->GlobalRetriever,
			_Animation); // yay
		nlassert(_Entities);

//		// Init the entities prefs
//		displayLoadingState("Initialize Entities ");
//		initEntities();
//		// Init animation system
//		displayLoadingState("Initialize Animation ");
//		initAnimation();

		_LoadingScreen.progress(1.f);





//		// Create a 3D mouse listener
//		// Init the aiming system
//		displayLoadingState("Initialize Aiming ");
//		initAiming();
//		// Init the user camera
//		displayLoadingState("Initialize Camera ");
//		initCamera();
//		
//		displayLoadingState("Initialize MouseListener ");
//		MouseListener = new C3dMouseListener();
//		MouseListener->addToServer(Driver->EventServer);
//		MouseListener->setCamera(Camera);
//		MouseListener->setHotSpot(CVectorD(0,0,0));
//		MouseListener->setFrustrum(Camera.getFrustum());
//		MouseListener->setMatrix(Camera.getMatrix());
//		MouseListener->setSpeed(PlayerSpeed);
//		initMouseListenerConfig();
//		// Init interface
//		displayLoadingState("Initialize Interface ");
//		initInterface();
//		// Init radar
//		displayLoadingState("Initialize Radar ");
//		initRadar();
//		// Init compass
//		displayLoadingState("Initialize Compass ");
//		initCompass();
//		// Init graph
//		displayLoadingState("Initialize Graph ");
//		initGraph();
//		// Init the command control
//		displayLoadingState("Initialize Commands ");
//		initCommands();
//

//		// Init the mouse so it's trapped by the main window.
//		Driver->showCursor(false);
//		Driver->setCapture(true);
//		Driver->setMousePos(0.5f, 0.5f);
//
//
//		// render adds
//		CFunctionCaller &updates = _ComponentManager->getUpdateCaller();		
//		updates.add(updateClient, NULL, NULL, 10000); // meh
//		CFunctionCaller &renderers = _ComponentManager->getRenderCaller();
//		renderers.add(renderClient, NULL, NULL, -1000); // meh
	}
}

void CSnowballsClient::unloadIngame()
{
	if (_LoadedIngame)
	{
		_Loading->setBackgroundSnowballsUnloading();
		_LoadingScreen.setRange(0.f, 1.f);
		float max_progress = 4.f;

		_LoadingScreen.progress(0.f / max_progress);
		_Loading->setMessageState("ReleaseCollisions");
		nlassert(_Entities); delete _Entities; _Entities = NULL;
		_LoadingScreen.progress(1.f / max_progress);
		_Loading->setMessageState("ReleaseAnimation");
		nlassert(_Animation); delete _Animation; _Animation = NULL;
		_LoadingScreen.progress(2.f / max_progress);
		_Loading->setMessageState("ReleaseCollisions");
		nlassert(_Collisions); delete _Collisions; _Collisions = NULL;
		_LoadingScreen.progress(3.f / max_progress);
		_Loading->setMessageState("ReleaseLandscape");
		nlassert(_Landscape); delete _Landscape; _Landscape = NULL;
		_LoadingScreen.progress(4.f / max_progress);

		//if (CaptureState)
		//{
		//	Driver->setCapture(false);
		//	Driver->showCursor(true);
		//}

		//// Release all before quit

		//releaseRadar();
		//releaseCommands();
		//releaseGraph();
		//releaseCompass();
		//releaseInterface();
		//releaseNetwork();
		//releaseMouseListenerConfig();
		//releaseCamera();
		//releaseAiming();
		//// Release the mouse listener
		//MouseListener->removeFromServer(Driver->EventServer);
		//delete MouseListener;

		_LoadedIngame = false;
	}
}

void CSnowballsClient::enableIngame()
{
	if (!_EnabledIngame)
	{		
		_EnabledIngame = true;

		// ...
	}
}

void CSnowballsClient::disableIngame()
{
	if (_EnabledIngame)
	{
		// ...

		_EnabledIngame = false;
	}
}

void CSnowballsClient::loadConnection()
{
	if (!_LoadedConnection)
	{		
		_LoadedConnection = true;

		switch (_LoginData.Version)
		{
		case CLogin::Offline:
			_LoginData.Message = ucstring::makeFromUtf8(toString(
			CI18N::get("SomethingNotImplemented").toUtf8().c_str(),
				"Offline"));
			nlwarning(_LoginData.Message.toString().c_str());
			_NextState = Login;
			break;
		case CLogin::Snowballs3:
			_LoginData.Message = ucstring::makeFromUtf8(toString(
			CI18N::get("SomethingNotImplemented").toUtf8().c_str(),
				"Snowballs3"));
			nlwarning(_LoginData.Message.toString().c_str());
			_NextState = Login;
			break;
		case CLogin::Snowballs5:
			_LoginData.Message = ucstring::makeFromUtf8(toString(
			CI18N::get("SomethingNotImplemented").toUtf8().c_str(),
				"Snowballs5"));
			nlwarning(_LoginData.Message.toString().c_str());
			_NextState = Login;
			break;
		default:
			nlerror("CSnowballsClient::loadConnection: _LoginData.Version invalid"); 
		}
	}
}

void CSnowballsClient::unloadConnection()
{
	if (_LoadedConnection)
	{
		switch (_LoginData.Version)
		{
		case CLogin::Offline:
			// nlassert(_Offline);
			// remove
			break;
		case CLogin::Snowballs3:
			// nlassert(_Snowballs3);
			// remove
			break;
		case CLogin::Snowballs5:
			// nlassert(_Snowballs5);
			// remove
			break;
		default:
			nlerror("CSnowballsClient::unloadConnection: _LoginData.Version invalid");
		}
		// nlassert(!_*);
		
		_LoadedConnection = false;
	}
}

void CSnowballsClient::enableConnection()
{
	if (!_EnabledConnection)
	{		
		_EnabledConnection = true;

		// nlassert(!_*Id);
		switch (_LoginData.Version)
		{
		case CLogin::Offline:
			// add
			// nlassert(_Offline*Id);
			break;
		case CLogin::Snowballs3:
			// add
			// nlassert(_Snowballs3*Id);
			break;
		case CLogin::Snowballs5:
			// add
			// nlassert(_Snowballs5*Id);			
			break;
		default:
			nlerror("CSnowballsClient::enableConnection: _LoginData.Version invalid"); 
		}
	}
}

void CSnowballsClient::disableConnection()
{
	if (_EnabledConnection)
	{
		switch (_LoginData.Version)
		{
		case CLogin::Offline:
			// nlassert(_Offline*Id);
			// remove
			break;
		case CLogin::Snowballs3:
			// nlassert(_Snowballs3*Id);
			// remove
			break;
		case CLogin::Snowballs5:
			// nlassert(_Snowballs5*Id);
			// remove
			break;
		default:
			nlerror("CSnowballsClient::disableConnection: _LoginData.Version invalid");
		}
		// nlassert(!_*Id);
		
		_EnabledConnection = false;
	}
}

void CSnowballsClient::unloadAll()
{
	unloadConnection();
	unloadIngame();
	unloadLogin();
	unloadBase();
	unloadUtils();
}

void CSnowballsClient::disableAll()
{
	disableConnection();
	disableIngame();
	disableLogin();
	disableBase();
	disableUtils();
}

SBCLIENT_CALLBACK_COMMAND_IMPL(CSnowballsClient, commandSetState)
{
	// one argument required, fail
	if (args.size() != 1) return false;

	// set the next state
	_NextState = CStringIdentifier::get(args[0]);

	// everything ok
	return true;
}

SBCLIENT_CALLBACK_IMPL(CSnowballsClient, updateUtilities)
{
	// check all config files for updates
	CConfigFile::checkConfigFiles();
}

//UTextureFile *testtexture = NULL;
UMaterial testmaterial;

/// Used during development only.
SBCLIENT_CALLBACK_IMPL(CSnowballsClient, updateDebug)
{
	//if (_Graphics && !testtexture)
	//{
	//	testtexture = _Graphics->Driver->createTextureFile("snowballs_login.tga");
	//	testmaterial = _Graphics->Driver->createMaterial();
	//	testmaterial.setTexture(testtexture);
	//}
	//testmaterial.
}

float progress = 0.0f;

SBCLIENT_CALLBACK_IMPL(CSnowballsClient, renderDebug)
{
	//if (_Graphics)
	//{
	//	nlassert(testtexture);
	//	_Graphics->Driver->setMatrixMode2D11();
	//	CQuadUV quad;
	//	quad.V0.set(0.0f, 0.0f, 0.0f);
	//	quad.V1.set(1.0f, 0.0f, 0.0f);
	//	quad.V2.set(1.0f, 1.0f, 0.0f);
	//	quad.V3.set(0.0f, 1.0f, 0.0f);

	//	float height = 768.0f / 1024.0f;
	//	float width = 1024.0f / 1024.0f;
	//	quad.Uv0.set(0.0f, height);
	//	quad.Uv1.set(width, height);
	//	quad.Uv2.set(width, 0.0f);
	//	quad.Uv3.set(0.0f, 0.0f);

	//	float final = 0.5f;
	//	quad.V1.x = final;
	//	quad.V2.x = final;
	//	quad.Uv1.U = width * final;
	//	quad.Uv2.U = width * final;
	//	_Graphics->Driver->drawQuad(quad, testmaterial);
	//}

	//if (_Loading)
	//{
	//	progress += 0.001f;
	//	_Loading->setBackgroundSnowballs();
	//	_Loading->setMessageState("Test");
	//	_LoadingScreen.progress(progress);
	//}

	renderVersion(tag);
}

SBCLIENT_CALLBACK_IMPL(CSnowballsClient, renderVersion)
{
	if (_Graphics)
	{
		UTextContext *tc = _Graphics->TextContext; nlassert(tc);

		tc->setKeep800x600Ratio(true);
		tc->setColor(CRGBA(255, 255, 255, 255));
		tc->setHotSpot(UTextContext::BottomRight);
		tc->setFontSize(12);
		tc->printAt(0.99f, 0.01f, ucstring(SBCLIENT_VERSION_STRING));

		tc->setHotSpot(UTextContext::BottomLeft);
		tc->printAt(0.01f, 0.01f, CI18N::get("WindowTitle"));
	}
}

} /* namespace SBCLIENT */

void end();
SBCLIENT::CSnowballsClient *client = NULL;
SBCLIENT::CSnowballsDebug *debug = NULL;
#ifdef NL_OS_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdline, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
	if (debug || client) return EXIT_FAILURE;
	debug = new SBCLIENT::CSnowballsDebug(); nlassert(debug);
	client = new SBCLIENT::CSnowballsClient(); nlassert(client);
	atexit(end); exit(client->run());
	return EXIT_FAILURE;
}
void end()
{
	nlassert(client); delete client; client = NULL;
	nlassert(debug); delete debug; debug = NULL;
}

/* end of file */
