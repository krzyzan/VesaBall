#include "stdafx.h"
#include "D3DAppVesaBall.h"

//!	"Entry point" aplikacji
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
	CVesaBallApp app;
	app.Create( hInstance );
	return app.Run();
}

