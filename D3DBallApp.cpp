// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v1.12
//
//////////////////////////////////////////////////////////////////////

#include "D3DBallApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD3DBallApp::CD3DBallApp( HINSTANCE hInstance )
: CD3DApp( hInstance, 1024, 768)
{
}

CD3DBallApp::~CD3DBallApp()
{

}

HRESULT CD3DBallApp::Render()
{
	FLOAT fTime = Timer.GetAbsoluteTime();

	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CSprite*>::iterator iSprite;
	for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	return S_OK;
}

HRESULT CD3DBallApp::InitDeviceObjects()
{
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/plank.png", &pTex[0] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/ballblue.png", &pTex[1] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Br¹z.png", &pTex[2] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Fiolet.png", &pTex[3] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Fiolet.png", &pTex[4] );

	CDeck* deck;
	deck = new CDeck( pTex[0], pDIDevice );
	listSprite.push_back( deck );

	//Tworzymy sprite'y
	CSprite* sprite;
	for (int i=0; i<10; i++)
		for (int j=0; j<10; j++) {
			sprite = new CBall( pTex[1], 
				D3DXVECTOR2(FLOAT(rand()%(1024-16)), FLOAT(rand()%(768-16))),
				deck );
			listSprite.push_back( sprite );
		}

	return S_OK;
}

HRESULT CD3DBallApp::FrameMove()
{
	list<CSprite*>::iterator iSprite;
	for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
		(*iSprite)->FrameMove();

	// Kasujemy sprite'y
	for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
		if ((*iSprite)->bDeleteMe) {
			delete (*iSprite);
			iSprite = listSprite.erase( iSprite );
			iSprite--;
		}

	return S_OK;
}

HRESULT CD3DBallApp::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );
	return S_OK;
}

HRESULT CD3DBallApp::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );
	return S_OK;
}

