#include "stdafx.h"
#include "D3DBallApp.h"

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
	CD3DBallApp app;
	app.Create( hInstance );
	return app.Run();
}

