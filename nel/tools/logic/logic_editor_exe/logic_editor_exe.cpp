// logic_editor_exe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../logic_editor_dll/logic_editor_interface.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*

- Le apply change sur les event ne marche pas
- increment/decrement ne marche pas sur les counter (c toujours increment)

*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// Create a logic editor
	ILogicEditor * logicEditor = ILogicEditor::getInterface();

	if( logicEditor )
	{
		// test the default file creation
		//logicEditor->createDefaultFile();

		// Init ui
		logicEditor->initUI();
		//logicEditor->initUILight(0,0,800,800);

		// Go
		logicEditor->go();

		// Release ui
		logicEditor->releaseUI ();

		// Delete the pointer
		ILogicEditor::releaseInterface( logicEditor );
	}

	return 0;
}
