#include "StdAfx.h"

#include "D3DApp.h"
#include "GameMenu.h"
/*    	
ToDo:
	- Zrobiæ przyjació³ z CD3DApp i CD3DScene.
	- Wywaliæ niepotrzebne argumenty z konstruktorów
	- zrobiæ obs³ugê dŸwiêku (np. kasuj¹c niepotrzebny kod z sampli do DSound )
	- Porz¹dek z protected, public, private
	- zrobic GameOver
*/


//!	"Entry point" aplikacji
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
	CD3DApp app( _T("VesaBall") );

	HRESULT hr;

	if (FAILED( hr = app.Create( hInstance ) ) )
		return hr;

	if (FAILED( hr = app.SetupScene( new CGameMenu() ) ) )
		return hr;

	return app.Run();
}

