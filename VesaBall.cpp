#include "StdAfx.h"

#include "App.h"
#include "GameMenu.h"

#include <fstream>
using namespace std;

DWORD RES_X;
DWORD RES_Y;

//! Initializes SDL2 and starts the game menu.
int main(int argc, char* argv[])
{
	ifstream ConfigFile;
	ConfigFile.open("cfg/res.cfg", ios::in);
	ConfigFile >> RES_X;
	ConfigFile >> RES_Y;
	ConfigFile.close();

	CApp app("VesaBall");

	HRESULT hr;

	if (FAILED(hr = app.Create()))
		return hr;

	if (FAILED(hr = app.StartNewScene(new CGameMenu())))
		return hr;

	return app.Run();
}
