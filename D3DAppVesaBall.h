/*    	
ToDo:
	- Wywaliæ niepotrzebne argumenty z konstruktorów
	- zrobiæ obs³ugê dŸwiêku (np. kasuj¹c niepotrzebny kod z sampli do DSound )
	- Porz¹dek z protected, public, private
	- zrobic GameOver
*/

#pragma once

#include <d3dx8.h>
#include <dinput.h>
#include <stack>
using namespace std;

#include "D3DApp.h"

class CD3DScene;


class CVesaBallApp :
	public CD3DApp  
{
public:
	CVesaBallApp();
	HRESULT Create( HINSTANCE hInstance );

private:
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT	FinalCleanup();

	HRESULT ReadMouseEvents();
	HRESULT ReadKeyboardEvents();
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();

	HRESULT ManageScenes();

	// DirectInput
	HRESULT InitializeMouseInput();
	HRESULT InitializeKeyboardInput();

	LPDIRECTINPUT8			pDI;			// The main DI object
	LPDIRECTINPUTDEVICE8	pMouseDevice;	// The mouse device
	LPDIRECTINPUTDEVICE8	pKeyboardDevice;// The mouse device

	float					fTimeToRender;

	DWORD dwFrameMoveCounter;				//TODO: TMP
	DWORD dwFrameRenderCounter;				//TODO: TMP

	// Rendered scene
	stack<CD3DScene*>		stackScenes;
};
