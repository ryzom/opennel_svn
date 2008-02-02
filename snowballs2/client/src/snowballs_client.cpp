/** \file snowballs_client.cpp
 * CSnowballsClient
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
 * 
 * $Id$
 */

//1. Update Time
//2. Update Driver
//3. Update Network
//4. Update Weather
//5. Update Landscape
//6. Update Entities
//7. Update Animations
//
//1. Render Driver
//2. Render Sky
//3. Render Landscape
//4. Render Scene
//5. Render Effects
//6. Render 3D Interface
//7. Render 2D Interface

#include <nel/misc/types_nl.h>
#include "snowballs_client.h"
#include "snowballs_config.h"

#include "i18n_helper.h"
#include "config_manager.h"
#include "config_proxy.h"

#include "graphics.h"

#include <nel/misc/config_file.h>
#include <nel/misc/path.h>
#include <nel/misc/debug.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/i18n.h>
#include <nel/misc/app_context.h>

#include <nel/3d/u_driver.h>

using namespace std;
using namespace NLMISC;

//// temp
//#define SBCLIENT_MUSIC_WAIT (0), (0)
//#define SBCLIENT_MUSIC_LOGIN (1), (0)
//#define SBCLIENT_MUSIC_BACKGROUND (2), (0)
//#define SBCLIENT_MUSIC_BACKGROUND_BEAT (2), (1)
//extern NLMISC::CConfigFile *ConfigFile;
//extern NL3D::UDriver *Driver;
//extern NL3D::UTextContext *TextContext;
//#include <nel/3d/u_scene.h>
//extern NL3D::UScene *Scene;
//#include "mouse_listener.h"
//#include <nel/misc/vectord.h>
//extern C3dMouseListener *MouseListener;
//extern float PlayerSpeed;
//extern bool CaptureState;
//#include "landscape_component.h"
//SBCLIENT::CLandscapeComponent *LandscapeComponent;
//void initLoadingState();
//void initSound();
//void displayLoadingState(char *state);
//void playMusic(sint32 playlist, sint32 track);
//void initLight();
//void releaseLight();
//void releaseLoadingState();
//void releaseSound();
//void updateTime(void *context, void *tag);
//void updateSound(void *context, void *tag);
//void initLandscape();
//void initPACS();
//void initAiming();
//void initCamera();
//void initMouseListenerConfig();
//void initInterface();
//void initRadar();
//void initCompass();
//void initGraph();
//void initCommands();
//void initEntities();
//void initAnimation();
//void initLensFlare();
//void initSky();
//void releaseLensFlare();
//void releaseRadar();
//void releaseCommands();
//void releaseEntities();
//void releaseGraph();
//void releaseCompass();
//void releaseInterface();
//void releaseNetwork();
//void releaseAnimation();
//void releaseAiming();
//void releasePACS();
//void releaseLandscape();
//extern uint32 NextEID;
//#include "entities.h"
//void addLine(const std::string &line);
//void deleteAllEntities();
//void updateClient(void *context, void *tag);
//void renderClient(void *context, void *tag);
//#include "scene_component.h"
//SBCLIENT::CSceneComponent *SceneComponent;
//#include "time_component.h"
//SBCLIENT::CTimeComponent *TimeComponent;
//#include "weather_component.h"
//SBCLIENT::CWeatherComponent *WeatherComponent;
//#include "lens_flare_component.h"
//SBCLIENT::CLensFlareComponent *LensFlareComponent;

namespace SBCLIENT {

CSnowballsClient::CSnowballsClient() 
: _I18NHelper(NULL), _Config(NULL), _ConfigManager(NULL), // utils
#if SBCLIENT_USE_LOG
  _FileDisplayer(NULL), 
#endif
  // components
  _Graphics(NULL), 
  // states
  _CurrentState(Invalid), _NextState(Load),
  _HasUtils(false), _HasBase(false), _HasLogin(false), _HasIngame(false),
  _HasConnection(false), _ServerVersion(Offline) // do not change these
{
	// use log.log if NEL_LOG_IN_FILE and SBCLIENT_USE_LOG_LOG defined as 1
	createDebug(NULL, SBCLIENT_USE_LOG_LOG, false);
	srand((uint)CTime::getLocalTime());

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

CSnowballsClient::~CSnowballsClient()
{
	disableAll(); // just to be sure :(
	
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
		switch(_CurrentState)
		{
		case Game:
			disableConnection();
			break;
		}
		switch(_NextState)
		{
		case Load:
			enableUtils();
			enableBase();
			break;
		case Reset:
			// displayLoadingState("Reset");
			disableAll();
			break;
		case Exit:
			// displayLoadingState("See you later!");
			disableAll();
			break;
		case Login:
			enableUtils(); // core is required
			enableBase();
			enableLogin(); // login is required
			break;
		case Game:
			enableUtils(); // core is required
			enableBase();
			disableLogin(); //login can be released
			enableIngame(); // ingame is required
			enableConnection();
			break;
		}
	}
	_CurrentState = _NextState;
	switch(_CurrentState)
	{
	case Load: // switch to the default state
		_NextState = Login;
		break;
	case Reset: // used to reset everything
		_NextState = Load;
		break;
	case Exit: // exit the loop
		return EXIT_SUCCESS;
	default:
		do { if (!_Graphics->Driver->isActive()) 
				{ _NextState = Exit; break; }
			// check all config files for updates
			CConfigFile::checkConfigFiles();
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
		} while (_CurrentState == _NextState);
		break;
	}
	goto SwitchState;
}

void CSnowballsClient::enableUtils()
{
	if (!_HasUtils)
	{
		_HasUtils = true;
		// init configuration file
		_ConfigManager = new CConfigManager(SBCLIENT_CONFIG_FILE);
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
	}
}

void CSnowballsClient::disableUtils()
{
	if (_HasUtils)
	{
		// if they're null, something must've seriously went wrong at some point
		// release of the language data is done when loading new file
		nlassert(_I18NHelper); delete _I18NHelper;

		// release the configuration file
		nlassert(_Config); delete _Config;
		nlassert(_ConfigManager); delete _ConfigManager;

		// release the search paths etc
		CPath::releaseInstance();

		_HasUtils = false;
	}
}

void CSnowballsClient::enableBase()
{
	if (!_HasBase)
	{		
		_HasBase = true;

		// _LoadingScreen.setblah("Initialize Graphics");
		_Graphics = new CGraphics(_LoadingScreen, "Graphics", _I18NHelper);
		nlassert(_Graphics);

		// TODO: 
		// - put this somewhere else
		// - get the uint and store it, and assert it isn't used set yet
		// - remove by uint
		_UpdateFunctions.add(CGraphics::updateDriver, _Graphics, NULL, 0);
	}
}

void CSnowballsClient::disableBase()
{
	if (_HasBase)
	{
		//temp..
		_UpdateFunctions.removeC(_Graphics, true);

		nlassert(_Graphics); delete _Graphics;
		
		_HasBase = false;
	}
}


//void CSnowballsClient::enableCore()
//{
//	if (!_HasCore)
//	{
//
//
//		
//
//		
//
//
//
//		// dynamic core, temp
//		_LoadingComponent = new CLoadingComponent(
//			_ComponentManager, "LoadingScreen", _LoadingScreen);
//		_ComponentManager->registerComponent(_LoadingComponent);
//
//		_DriverComponent = new CDriverComponent(
//			_ComponentManager, "Graphics", _LoadingScreen);
//		_ComponentManager->registerComponent(_DriverComponent);
//		_ComponentManager->registerUpdate(_DriverComponent, 100);
//		_Driver = _DriverComponent->getDriver();
//
//		_HelloWorldComponent = new CHelloWorldComponent(
//			_ComponentManager, "HelloWorld", _LoadingScreen);
//		_ComponentManager->registerComponent(_HelloWorldComponent);
//		_ComponentManager->registerRender(_HelloWorldComponent, -2000);
//
//		TimeComponent = new CTimeComponent(
//			_ComponentManager, "Time", _LoadingScreen);
//		_ComponentManager->registerComponent(TimeComponent);
//		_ComponentManager->registerUpdate(TimeComponent, 1000000);
//
//
//		// and some more compatibility code
//		Driver = _DriverComponent->getDriver();
//		TextContext = _DriverComponent->getTextContext();
//		ConfigFile = _ConfigFile;
//
//		/// even more temp!
//		CFunctionCaller &updates = _ComponentManager->getUpdateCaller();
//		updates.add(updateTime, NULL, NULL, 1000000);
//		displayLoadingState("Initialize Loading");
//		initLoadingState();
//		// Initialize sound for loading screens etc
//#if SBCLIENT_WITH_SOUND
//		displayLoadingState("Initialize Sound");
//		initSound();
//		playMusic(SBCLIENT_MUSIC_WAIT);
//		updates.add(updateSound, NULL, NULL, 0);
//#endif
//	}
//}

//void CSnowballsClient::disableCore()
//{
//	if (_HasCore)
//	{
//		/// EVEN MORE TEMP!
//		// Release the loading state textures
//		releaseLoadingState();
//		// Release the sound
//#if SBCLIENT_WITH_SOUND
//		releaseSound();
//#endif
//
//		// dynamic core, temp
//		_ComponentManager->unregisterComponent(_HelloWorldComponent);
//		delete _HelloWorldComponent;
//
//		_ComponentManager->unregisterComponent(_DriverComponent);
//		delete _DriverComponent;
//
//		_ComponentManager->unregisterComponent(_LoadingComponent);
//		delete _LoadingComponent;
//
//
//
//
//
//

//
//		_HasCore = false;
//	}
//}

void CSnowballsClient::enableLogin()
{
	if (!_HasLogin)
	{		
		_HasLogin = true;

		// ...
	}
}

void CSnowballsClient::disableLogin()
{
	if (_HasLogin)
	{
		// ...
		
		_HasLogin = false;
	}
}

void CSnowballsClient::enableIngame()
{
	if (!_HasIngame)
	{		
		_HasIngame = true;

//		// just temp things
//#if SBCLIENT_WITH_SOUND
//		playMusic(SBCLIENT_MUSIC_WAIT);
//#endif
//
//		// Create a scene
//		SceneComponent = new CSceneComponent(
//			_ComponentManager, "Scene", _LoadingScreen);
//		_ComponentManager->registerComponent(SceneComponent);
//		_ComponentManager->registerUpdate(SceneComponent, -2000);
//		_ComponentManager->registerRender(SceneComponent, -500);
//		Scene = SceneComponent->getScene();
//		// Init the landscape using the previously created UScene
//		displayLoadingState("Initialize Landscape");
//		LandscapeComponent = new CLandscapeComponent(
//			_ComponentManager, "Landscape", _LoadingScreen);
//		_ComponentManager->registerComponent(LandscapeComponent);
//		_ComponentManager->registerUpdate(LandscapeComponent, -1100);
//		initLandscape();
//		// Init the pacs
//		displayLoadingState("Initialize PACS ");
//		initPACS();
//		// Init the aiming system
//		displayLoadingState("Initialize Aiming ");
//		initAiming();
//		// Init the user camera
//		displayLoadingState("Initialize Camera ");
//		initCamera();
//		// init weather
//		displayLoadingState("Initialize Weather");
//		WeatherComponent = new CWeatherComponent(
//			_ComponentManager, "Weather", _LoadingScreen);
//		_ComponentManager->registerComponent(WeatherComponent);
//		_ComponentManager->registerRender(WeatherComponent, -250);
//		_ComponentManager->registerUpdate(WeatherComponent, 5000);
//		// Create a 3D mouse listener
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
//		// Init the entities prefs
//		displayLoadingState("Initialize Entities ");
//		initEntities();
//		// Init animation system
//		displayLoadingState("Initialize Animation ");
//		initAnimation();
//		// Init the lens flare
//		displayLoadingState("Initialize LensFlare ");
//		LensFlareComponent = new CLensFlareComponent(
//			_ComponentManager, "LensFlare", _LoadingScreen);
//		_ComponentManager->registerComponent(LensFlareComponent);
//		_ComponentManager->registerRender(LensFlareComponent, -900);
//		
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

void CSnowballsClient::disableIngame()
{
	if (_HasIngame)
	{
		//// more temp
		//CFunctionCaller &updates = _ComponentManager->getUpdateCaller();		
		//updates.removeF(updateClient, true);
		//CFunctionCaller &renderers = _ComponentManager->getRenderCaller();
		//updates.removeF(renderClient, true);

		//if (CaptureState)
		//{
		//	Driver->setCapture(false);
		//	Driver->showCursor(true);
		//}

		//// Release all before quit

		//releaseRadar();
		//releaseCommands();
		//releaseEntities();
		//releaseGraph();
		//releaseCompass();
		//releaseInterface();
		//releaseNetwork();
		//releaseAnimation();
		//releaseMouseListenerConfig();
		//releaseCamera();
		//releaseAiming();
		//releasePACS();
		//delete LandscapeComponent;
		//// Release the mouse listener
		//MouseListener->removeFromServer(Driver->EventServer);
		//delete MouseListener;
		//Driver->deleteScene(Scene);
		//
		//_HasIngame = false;
	}
}

void CSnowballsClient::enableConnection()
{
	if (!_HasConnection)
	{		
		_HasConnection = true;

		// ...
	}
}

void CSnowballsClient::disableConnection()
{
	if (_HasConnection)
	{
		// ...
		
		_HasConnection = false;
	}
}

//void CSnowballsClient::enableOffline()
//{
//	if (!_HasOffline)
//	{		
//		_HasOffline = true;

//		// another bunch of temp stuff
//#if SBCLIENT_WITH_SOUND
//		playMusic(SBCLIENT_MUSIC_WAIT);
//#endif
//
//		uint32 id = NextEID++;
//		ucstring Login = ucstring("Entity" + toString(id));
//
//		// Creates the self entity
//		displayLoadingState("Creating offline entity");
//		addEntity(id, Login.toUtf8(), 
//			CEntity::Self, CVector(
//					ConfigFile->getVar("StartPoint").asFloat(0),
//					ConfigFile->getVar("StartPoint").asFloat(1),
//					ConfigFile->getVar("StartPoint").asFloat(2)),
//				CVector(
//					ConfigFile->getVar("StartPoint").asFloat(0),
//					ConfigFile->getVar("StartPoint").asFloat(1),
//					ConfigFile->getVar("StartPoint").asFloat(2)));
//
//		displayLoadingState("Load Landscape");
//		LandscapeComponent->loadAllZonesAround();
//
//		// Display a local welcome message
//		addLine(">>>>> Welcome to Snowballs!");
//		addLine(">>>>> Press SHIFT-ESC to exit the game.");
//
//		displayLoadingState("Ready!");
//	
//#if SBCLIENT_WITH_SOUND
//		playMusic(SBCLIENT_MUSIC_BACKGROUND);
//#endif
//	}
//}

//void CSnowballsClient::disableOffline()
//{
//	if (_HasOffline)
//	{
//		//// temp
//		//deleteAllEntities();
//		
//		_HasOffline = false;
//	}
//}

void CSnowballsClient::disableAll()
{
	disableConnection();
	disableIngame();
	disableLogin();
	disableBase();
	disableUtils();
}

}


void end();
SBCLIENT::CSnowballsClient *client = NULL;
#ifdef NL_OS_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdline, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{	
	client = new SBCLIENT::CSnowballsClient();
	atexit(end); exit(client->run());
	return EXIT_FAILURE;
}

void end()
{
	delete client;
}
