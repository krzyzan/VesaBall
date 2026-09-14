#include "StdAfx.h"

#include "D3DApp.h"
#include "GameMenu.h"

#include <fstream>
using namespace std;

DWORD RES_X;
DWORD RES_Y;

//! Inicjalizuje Direct3D i w³¹cza menu gry.
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR args, INT argc)
{
	ifstream ConfigFile;
	ConfigFile.open( "cfg/res.cfg", ios::in );
	ConfigFile >> RES_X;
	ConfigFile >> RES_Y;
	ConfigFile.close();

	CD3DApp app( _T("VesaBall") );

	HRESULT hr;

	if (FAILED( hr = app.Create( hInstance ) ) )
		return hr;

	if (FAILED( hr = app.StartNewScene( new CGameMenu() ) ) )
		return hr;

	return app.Run();
}

