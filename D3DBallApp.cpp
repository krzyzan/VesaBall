// D3DBallApp.cpp: implementation of the CD3DBallApp class.
//
//////////////////////////////////////////////////////////////////////

#include "D3DBallApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DBallApp::CD3DBallApp( HINSTANCE hInstance )
: CD3DApp( hInstance, 1024, 768 )
{
}

CD3DBallApp::~CD3DBallApp()
{
	SAFE_RELEASE( pTex );
}


//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//		 the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DBallApp::FrameMove()
{
	/*
	DIMOUSESTATE dims;		// DirectInput mouse state structure
	// Get the input's device state, and put the state in dims
	ZeroMemory( &dims, sizeof(dims) );
	DIDevice->GetDeviceState( sizeof(DIMOUSESTATE), &dims );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to look at, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.

	dims.lX*0.001f
	dims.lY*0.001f
	dims.lZ*0.0001f

	if (dims.rgbButtons[0] & 0x80) vEye += vLookAt * 0.0001f;
	if (dims.rgbButtons[1] & 0x80) vEye -= vLookAt * 0.0001f;

	*/
	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CD3DBallApp::InitDeviceObjects()
{
	D3DXCreateTextureFromFile( pd3dDevice, "Tynk.png", &pTex );
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );
	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//		 this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CD3DBallApp::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );
	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//		 rendering. This function sets up render states, clears the
//		 viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DBallApp::Render()
{
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	if( SUCCEEDED( pd3dDevice->BeginScene() ) ) {
		pSprite->Begin();

		FLOAT fTime = Timer.GetAbsoluteTime();
		pSprite->Draw( pTex, NULL, &D3DXVECTOR2(1, 1), NULL, fTime, &D3DXVECTOR2(150, 50), 0xFFFFFFFF );

		pSprite->End();
		pd3dDevice->EndScene();
	}

	//pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}
