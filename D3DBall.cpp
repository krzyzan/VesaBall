#include "D3DBallApp.h"

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point



INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CD3DBallApp app(hInst);
	app.Create();
	return app.Run();
}

