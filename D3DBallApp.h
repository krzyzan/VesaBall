/*    	
ToDo:
	- Przyspieszanie kulki przy odbiciu (a¿ do 1.0f ?)
	- zrobic GameOver oraz wyjœcie do menu z levelu i edytora
	- usun¹æ obracanie w CCounter i wsadziæ do CGameEngine
	- Wywaliæ niepotrzebne argumenty z konstruktorów
	- Zrobiæ porzadek z destruktorami ( wykasowaæ niepotrzebne, wszystkie musz¹ byæ virtual )
	- Rzeczy niezwi¹zane z ruchem (np. blending, efekty) wrzucic do Render
	- Co robiæ gdy wjedziemy bokiem deski w kulkê???
	- zrobiæ obs³ugê dŸwiêku (np. kasuj¹c niepotrzebny kod z sampli do DSound )
	- Porz¹dek z protected, public, private

ToDo:
	- Poœwiata/ogon dla fireballa
	- Gumowa deska
	- Deska, która losowo odbija 
*/

#pragma once

#include <d3dx8.h>
#include <dinput.h>
#include "D3DApp.h"

class CD3DAppScene;


class CD3DBallApp :
	public CD3DApp  
{
public:
	CD3DBallApp();
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

	// DirectInput
	HRESULT InitializeMouseInput();
	HRESULT InitializeKeyboardInput();

	LPDIRECTINPUT8			pDI;			// The main DI object
	LPDIRECTINPUTDEVICE8	pMouseDevice;	// The mouse device
	LPDIRECTINPUTDEVICE8	pKeyboardDevice;// The mouse device

	float					fTimeToRender;

	// Rendered scene
	CD3DAppScene*	pScene;

	DWORD dwFrameMoveCounter;				//TODO: TMP
	DWORD dwFrameRenderCounter;				//TODO: TMP
};
