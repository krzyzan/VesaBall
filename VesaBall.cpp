#include "StdAfx.h"

#include "App.h"
#include "GameMenu.h"

//! Initializes SDL2 and starts the game menu.
int main(int argc, char* argv[])
{
	CApp app("VesaBall");

	HRESULT hr;

	if (FAILED(hr = app.Create()))
		return hr;

	if (FAILED(hr = app.StartNewScene(new CGameMenu())))
		return hr;

	return app.Run();
}
