// D3DBallApp.cpp: implementation of the CD3DBallApp class.
// v1.13
//
//////////////////////////////////////////////////////////////////////

#include "D3DBallApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CD3DBallApp::CD3DBallApp( HINSTANCE hInstance )
: CD3DApp( hInstance, RES_X, RES_Y)
{	
	numFrameMove	= 0;
	numRender		= 0;

	RenderingTimer.Reset();
	RenderingTimer.Start();
}

CD3DBallApp::~CD3DBallApp()
{
	//char str[100];
	//_itoa(numFrameMove/numRender,str,10);
	//DXTRACE_ERR(str,0);
}


HRESULT CD3DBallApp::InitDeviceObjects()
{
	LoadTexture( "gfx/plank.png",	0 );
	LoadTexture( "gfx/red.png",		1 );
	LoadTexture( "gfx/Br¹z.png",	2 );
	LoadTexture( "gfx/Fiolet.png",	3 );
	LoadTexture( "gfx/tree2.jpg",	4 );

	CDeck* deck;
	deck = new CDeck( pTex[0], pDIDevice );
	listSprite.push_back( deck );

	//Tworzymy sprite'y
	CSprite* sprite;
	for (int i=0; i<10; i++)
		for (int j=0; j<10; j++) {
			sprite = new CBall( pTex[1], 
				D3DXVECTOR2(FLOAT(rand()%(RES_X-32))+16, FLOAT(rand()%(RES_X-32))+16),
				deck );
			listSprite.push_back( sprite );
		}

	return S_OK;
}


HRESULT CD3DBallApp::Render()
{
	if(  RenderingTimer.GetAppTime() > 1.0f/120 ) {

		numRender++;
		//pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

		pd3dDevice->BeginScene();
		pSprite->Begin();

	//temp

		pSprite->Draw( pTex[4], NULL, &D3DXVECTOR2(1.0f, 1.5f), NULL, 0, NULL, 0xFF7F7F7F );
	//koniec temp


		list<CSprite*>::iterator iSprite;
		for (iSprite = listSprite.begin(); iSprite != listSprite.end(); iSprite++)
			(*iSprite)->Render( pSprite );

		pSprite->End();
		pd3dDevice->EndScene();

		// Show the frame on the primary surface.
		pd3dDevice->Present( NULL, NULL, NULL, NULL );
		RenderingTimer.Reset();

	}

	return S_OK;
}


HRESULT CD3DBallApp::FrameMove()
{
	numFrameMove++;

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

HRESULT CD3DBallApp::LoadTexture( LPCTSTR nameTexture, DWORD numTex )
{
	D3DXCreateTextureFromFileEx( pd3dDevice, nameTexture, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED , D3DX_DEFAULT,
		D3DX_DEFAULT , 0x00FF00FF,NULL,NULL, &pTex[numTex] );

	return S_OK;

}
