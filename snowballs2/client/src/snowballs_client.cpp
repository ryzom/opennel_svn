/** \file snowballs_client.cpp
 * CSnowballsClient
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

#include <nel/misc/types_nl.h>
#include "snowballs_client.h"
#include "snowballs_config.h"

#include "component_manager.h"

// temp
#include "driver_component.h"
#include "loading_component.h"
#include "hello_world_component.h"

#include <nel/misc/config_file.h>
#include <nel/misc/path.h>
#include <nel/misc/debug.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/i18n.h>
#include <nel/misc/app_context.h>

#include <nel/3d/u_driver.h>

using namespace std;
using namespace NLMISC;

// temp
#define SBCLIENT_MUSIC_WAIT (0), (0)
#define SBCLIENT_MUSIC_LOGIN (1), (0)
#define SBCLIENT_MUSIC_BACKGROUND (2), (0)
#define SBCLIENT_MUSIC_BACKGROUND_BEAT (2), (1)
extern NLMISC::CConfigFile *ConfigFile;
extern NL3D::UDriver *Driver;
extern NL3D::UTextContext *TextContext;
#include <nel/3d/u_scene.h>
extern NL3D::UScene *Scene;
#include "mouse_listener.h"
#include <nel/misc/vectord.h>
extern C3dMouseListener *MouseListener;
extern float PlayerSpeed;
extern bool CaptureState;
#include "landscape_component.h"
SBCLIENT::CLandscapeComponent *LandscapeComponent;
void initLoadingState();
void initSound();
void displayLoadingState(char *state);
void playMusic(sint32 playlist, sint32 track);
void initLight();
void releaseLight();
void releaseLoadingState();
void releaseSound();
void updateTime(void *context, void *tag);
void updateSound(void *context, void *tag);
void initLandscape();
void initPACS();
void initAiming();
void initCamera();
void initMouseListenerConfig();
void initInterface();
void initRadar();
void initCompass();
void initGraph();
void initCommands();
void initEntities();
void initAnimation();
void initLensFlare();
void initSky();
void releaseLensFlare();
void releaseRadar();
void releaseCommands();
void releaseEntities();
void releaseGraph();
void releaseCompass();
void releaseInterface();
void releaseNetwork();
void releaseAnimation();
void releaseAiming();
void releasePACS();
void releaseLandscape();
extern uint32 NextEID;
#include "entities.h"
void addLine(const std::string &line);
void deleteAllEntities();
void updateClient(void *context, void *tag);
void renderClient(void *context, void *tag);
#include "scene_component.h"
SBCLIENT::CSceneComponent *SceneComponent;
#include "time_component.h"
SBCLIENT::CTimeComponent *TimeComponent;
#include "weather_component.h"
SBCLIENT::CWeatherComponent *WeatherComponent;
#include "lens_flare_component.h"
SBCLIENT::CLensFlareComponent *LensFlareComponent;

namespace SBCLIENT {

CSnowballsClient::CSnowballsClient() 
: _FileDisplayer(NULL), _ComponentManager(NULL), 
  _ConfigFile(NULL), _DriverComponent(NULL), _Driver(NULL), 
  _CurrentState(Invalid), _NextState(Load),
  _HasCore(false), _HasLogin(false), _HasIngame(false),
  _HasOnline(false), _HasOffline(false),
  _PlayOnline(false) // do not change these
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
	disableAll(); // just to be sure
	
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
			disableOnline();
			disableOffline();
			break;
		}
		switch(_NextState)
		{
		case Load:
			enableCore();
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
			enableCore(); // core is required
			enableLogin(); // login is required
			break;
		case Game:
			enableCore(); // core is required
			disableLogin(); //login can be released
			enableIngame(); // ingame is required
			if (_PlayOnline) enableOnline(); // connection is required
			else enableOffline();
			break;
		}
	}
	_CurrentState = _NextState;
	switch(_CurrentState)
	{
	case Load: // switch to the default state
		_PlayOnline = false;
		_NextState = Game;// _NextState = Login;
		break;
	case Reset: // used to reset everything
		_NextState = Load;
		break;
	case Exit: // exit the loop
		return EXIT_SUCCESS;
	default:
		do { if (!_Driver->isActive()) 
				{ _NextState = Exit; break; }
			_ComponentManager->update();
			_ComponentManager->render();
		} while (_CurrentState == _NextState);
		break;
	}
	goto SwitchState;
}

void CSnowballsClient::enableCore()
{
	if (!_HasCore)
	{
		_HasCore = true;
		// init configuration file
		_ConfigFile = new CConfigFile();
		_ConfigFile->load(SBCLIENT_CONFIG_FILE);

		// set the search paths (kinda important)
		CConfigFile::CVar *var;
		var = &_ConfigFile->getVar(SBCLIENT_NAME "_SearchPaths");
		uint varsize = var->size();
		for (uint i = 0; i < varsize; ++i)
			CPath::addSearchPath(var->asString(i), true, false);
		var = &_ConfigFile->getVar(SBCLIENT_NAME "_RemapExtensions");
		for (uint i = 0; i < varsize; i += 2)
			CPath::remapExtension(var->asString(i), var->asString(i + 1), true);

		// set the language code
		CI18N::load(_ConfigFile->getVar(SBCLIENT_NAME "_LanguageCode").asString());

		// initialize the manager
		_ComponentManager = new CComponentManager(_ConfigFile);

		

		



		// dynamic core, temp
		_LoadingComponent = new CLoadingComponent(
			_ComponentManager, "LoadingScreen", _LoadingScreen);
		_ComponentManager->registerComponent(_LoadingComponent);

		_DriverComponent = new CDriverComponent(
			_ComponentManager, "Graphics", _LoadingScreen);
		_ComponentManager->registerComponent(_DriverComponent);
		_ComponentManager->registerUpdate(_DriverComponent, 100);
		_Driver = _DriverComponent->getDriver();

		_HelloWorldComponent = new CHelloWorldComponent(
			_ComponentManager, "HelloWorld", _LoadingScreen);
		_ComponentManager->registerComponent(_HelloWorldComponent);
		_ComponentManager->registerRender(_HelloWorldComponent, -2000);

		TimeComponent = new CTimeComponent(
			_ComponentManager, "Time", _LoadingScreen);
		_ComponentManager->registerComponent(TimeComponent);
		_ComponentManager->registerUpdate(TimeComponent, 1000000);


		// and some more compatibility code
		Driver = _DriverComponent->getDriver();
		TextContext = _DriverComponent->getTextContext();
		ConfigFile = _ConfigFile;

		/// even more temp!
		CFunctionCaller &updates = _ComponentManager->getUpdateCaller();
		updates.add(updateTime, NULL, NULL, 1000000);
		displayLoadingState("Initialize Loading");
		initLoadingState();
		// Initialize sound for loading screens etc
#if SBCLIENT_WITH_SOUND
		displayLoadingState("Initialize Sound");
		initSound();
		playMusic(SBCLIENT_MUSIC_WAIT);
		updates.add(updateSound, NULL, NULL, 0);
#endif
	}
}

void CSnowballsClient::disableCore()
{
	if (_HasCore)
	{
		/// EVEN MORE TEMP!
		// Release the loading state textures
		releaseLoadingState();
		// Release the sound
#if SBCLIENT_WITH_SOUND
		releaseSound();
#endif

		// dynamic core, temp
		_ComponentManager->unregisterComponent(_HelloWorldComponent);
		delete _HelloWorldComponent;

		_ComponentManager->unregisterComponent(_DriverComponent);
		delete _DriverComponent;

		_ComponentManager->unregisterComponent(_LoadingComponent);
		delete _LoadingComponent;






		// if they're null, something must've seriously went wrong at some point	
		// release the component manager
		nlassert(_ComponentManager); delete _ComponentManager;

		// release of the language data is done when loading new file
		//CI18N::...

		// release the configuration file
		nlassert(_ConfigFile); delete _ConfigFile;

		// release the search paths etc
		CPath::releaseInstance();

		_HasCore = false;
	}
}

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

		// just temp things
#if SBCLIENT_WITH_SOUND
		playMusic(SBCLIENT_MUSIC_WAIT);
#endif

		// Create a scene
		SceneComponent = new CSceneComponent(
			_ComponentManager, "Scene", _LoadingScreen);
		_ComponentManager->registerComponent(SceneComponent);
		_ComponentManager->registerUpdate(SceneComponent, -2000);
		_ComponentManager->registerRender(SceneComponent, -500);
		Scene = SceneComponent->getScene();
		// Init the landscape using the previously created UScene
		displayLoadingState("Initialize Landscape");
		LandscapeComponent = new CLandscapeComponent(
			_ComponentManager, "Landscape", _LoadingScreen);
		_ComponentManager->registerComponent(LandscapeComponent);
		_ComponentManager->registerUpdate(LandscapeComponent, -1100);
		initLandscape();
		// Init the pacs
		displayLoadingState("Initialize PACS ");
		initPACS();
		// Init the aiming system
		displayLoadingState("Initialize Aiming ");
		initAiming();
		// Init the user camera
		displayLoadingState("Initialize Camera ");
		initCamera();
		// init weather
		displayLoadingState("Initialize Weather");
		WeatherComponent = new CWeatherComponent(
			_ComponentManager, "Weather", _LoadingScreen);
		_ComponentManager->registerComponent(WeatherComponent);
		_ComponentManager->registerRender(WeatherComponent, -250);
		_ComponentManager->registerUpdate(WeatherComponent, 5000);
		// Create a 3D mouse listener
		displayLoadingState("Initialize MouseListener ");
		MouseListener = new C3dMouseListener();
		MouseListener->addToServer(Driver->EventServer);
		MouseListener->setCamera(Camera);
		MouseListener->setHotSpot(CVectorD(0,0,0));
		MouseListener->setFrustrum(Camera.getFrustum());
		MouseListener->setMatrix(Camera.getMatrix());
		MouseListener->setSpeed(PlayerSpeed);
		initMouseListenerConfig();
		// Init interface
		displayLoadingState("Initialize Interface ");
		initInterface();
		// Init radar
		displayLoadingState("Initialize Radar ");
		initRadar();
		// Init compass
		displayLoadingState("Initialize Compass ");
		initCompass();
		// Init graph
		displayLoadingState("Initialize Graph ");
		initGraph();
		// Init the command control
		displayLoadingState("Initialize Commands ");
		initCommands();
		// Init the entities prefs
		displayLoadingState("Initialize Entities ");
		initEntities();
		// Init animation system
		displayLoadingState("Initialize Animation ");
		initAnimation();
		// Init the lens flare
		displayLoadingState("Initialize LensFlare ");
		LensFlareComponent = new CLensFlareComponent(
			_ComponentManager, "LensFlare", _LoadingScreen);
		_ComponentManager->registerComponent(LensFlareComponent);
		_ComponentManager->registerRender(LensFlareComponent, -900);
		

		// Init the mouse so it's trapped by the main window.
		Driver->showCursor(false);
		Driver->setCapture(true);
		Driver->setMousePos(0.5f, 0.5f);


		// render adds
		CFunctionCaller &updates = _ComponentManager->getUpdateCaller();		
		updates.add(updateClient, NULL, NULL, 10000); // meh
		CFunctionCaller &renderers = _ComponentManager->getRenderCaller();
		renderers.add(renderClient, NULL, NULL, -1000); // meh
	}
}

void CSnowballsClient::disableIngame()
{
	if (_HasIngame)
	{
		// more temp
		CFunctionCaller &updates = _ComponentManager->getUpdateCaller();		
		updates.removeF(updateClient, true);
		CFunctionCaller &renderers = _ComponentManager->getRenderCaller();
		updates.removeF(renderClient, true);

		if (CaptureState)
		{
			Driver->setCapture(false);
			Driver->showCursor(true);
		}

		// Release all before quit

		releaseRadar();
		releaseCommands();
		releaseEntities();
		releaseGraph();
		releaseCompass();
		releaseInterface();
		releaseNetwork();
		releaseAnimation();
		releaseMouseListenerConfig();
		releaseCamera();
		releaseAiming();
		releasePACS();
		delete LandscapeComponent;
		// Release the mouse listener
		MouseListener->removeFromServer(Driver->EventServer);
		delete MouseListener;
		Driver->deleteScene(Scene);
		
		_HasIngame = false;
	}
}

void CSnowballsClient::enableOnline()
{
	if (!_HasOnline)
	{		
		_HasOnline = true;

		// ...
	}
}

void CSnowballsClient::disableOnline()
{
	if (_HasOnline)
	{
		// ...
		
		_HasOnline = false;
	}
}

void CSnowballsClient::enableOffline()
{
	if (!_HasOffline)
	{		
		_HasOffline = true;

		// another bunch of temp stuff
#if SBCLIENT_WITH_SOUND
		playMusic(SBCLIENT_MUSIC_WAIT);
#endif

		uint32 id = NextEID++;
		ucstring Login = ucstring("Entity" + toString(id));

		// Creates the self entity
		displayLoadingState("Creating offline entity");
		addEntity(id, Login.toUtf8(), 
			CEntity::Self, CVector(
					ConfigFile->getVar("StartPoint").asFloat(0),
					ConfigFile->getVar("StartPoint").asFloat(1),
					ConfigFile->getVar("StartPoint").asFloat(2)),
				CVector(
					ConfigFile->getVar("StartPoint").asFloat(0),
					ConfigFile->getVar("StartPoint").asFloat(1),
					ConfigFile->getVar("StartPoint").asFloat(2)));

		displayLoadingState("Load Landscape");
		LandscapeComponent->loadAllZonesAround();

		// Display a local welcome message
		addLine(">>>>> Welcome to Snowballs!");
		addLine(">>>>> Press SHIFT-ESC to exit the game.");

		displayLoadingState("Ready!");
	
#if SBCLIENT_WITH_SOUND
		playMusic(SBCLIENT_MUSIC_BACKGROUND);
#endif
	}
}

void CSnowballsClient::disableOffline()
{
	if (_HasOffline)
	{
		// temp
		deleteAllEntities();
		
		_HasOffline = false;
	}
}

void CSnowballsClient::disableAll()
{
	disableOnline();
	disableOffline();
	disableIngame();
	disableLogin();
	disableCore();
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
