#include "stdafx.h"
#include "level.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

CLevel::CLevel( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{  
	/*TODO: TMP*/hWnd = wnd;
	pSprite			= NULL;

	numFrameMove	= 0;
	numRender		= 0;

	bThruBrick = FALSE;
}


CLevel::~CLevel()
{
	char str[100] = "FrameMove() / Render(): ";
	char* p = str + strlen(str);
	_gcvt(((FLOAT)numFrameMove)/numRender, 4, p);
	p = str + strlen(str);
	strcat( p, "\nRender() / sec: ");
	p = str + strlen(str);
	_gcvt(numRender/timerRenderLimiter.GetTime(), 4, p);

	MessageBox( hWnd, str, "Internal counters", MB_OK );
}


HRESULT CLevel::InitDeviceObjects()
{
	timerRenderLimiter.Start();
	fTimeToRender = 0;

	return S_OK;
}


HRESULT CLevel::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CLevel::RenderLoop()
{
	FLOAT fElapsedTime = timerRenderLimiter.GetElapsedTime();
	// TMP: nie wiem czemu u mnie na starcie timer zawiesza siê na 2 sek. 
	// wtedy nic nie rób ( mo¿e czas wymieniæ BIOS??? )
	if ( fElapsedTime > 0.1 ) 
		return S_OK;	
	numFrameMove++;

	MoveObjects( fElapsedTime );
	
	DestroyObjects();

	// GAME OVER !!!!
	if (listBall.empty())
		return E_FAIL;

///////////////////////////////////////
	
	fTimeToRender -= fElapsedTime;
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	numRender++;

///////////////////////////////////////

	RenderObjects();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}


HRESULT CLevel::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );

	return S_OK;
}


HRESULT CLevel::DeleteDeviceObjects()
{
	// Kasujemy z listy renderowania
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}


HRESULT CLevel::MoveObjects( FLOAT fElapsedTime )
{
	// wykonujemy ruch dla wszystkich obiektów
	list<CMovingSprite*>::iterator	iMovingSprite;
	for (iMovingSprite = listFrameMove.begin(); iMovingSprite != listFrameMove.end(); iMovingSprite++)
		(*iMovingSprite)->FrameMove( fElapsedTime );

	return S_OK;
}


HRESULT CLevel::DestroyObjects()
{
	list<CMovingSprite*>::iterator	iMovingSprite;
	list<CSprite*>::iterator		iSprite;
	list<CSprite*>::iterator		iBallObst;
	list<CBall*>::iterator			iBall;

	// Kasujemy z listy przeszkód dla kulek
	iBallObst = listBallObst.begin(); 
	while (iBallObst != listBallObst.end()) {
		if ((*iBallObst)->bDeleteMe)
			iBallObst = listBallObst.erase( iBallObst );
		else
			iBallObst++;
	}

	// Kasujemy z listy obiektów ruchomych
	iMovingSprite = listFrameMove.begin(); 
	while (iMovingSprite != listFrameMove.end()) {
		if ((*iMovingSprite)->bDeleteMe)
			iMovingSprite = listFrameMove.erase( iMovingSprite );
		else
			iMovingSprite++;
	}

	// Kasujemy z listy kulek
	iBall = listBall.begin(); 
	while (iBall != listBall.end()) {
		if ((*iBall)->bDeleteMe)
			iBall = listBall.erase( iBall );
		else
			iBall++;
	}

	// Kasujemy z listy renderowania
	iSprite = listRender.begin();
	while (iSprite != listRender.end()) {
		if ((*iSprite)->bDeleteMe) {
			delete (*iSprite);
			iSprite = listRender.erase( iSprite );
		}
		else
			iSprite++;
	}

	return S_OK;
}


HRESULT CLevel::RenderObjects()
{
	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00,0x00,0x00), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CSprite*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	return S_OK;
}


HRESULT CLevel::AddBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, LPDIRECT3DTEXTURE8 SparkTexture )
{
	CBall* pBall = new CBall( Texture, Position, Speed, &listBallObst,  &listRender, &listFrameMove, SparkTexture );
	listRender.push_back( pBall );
	listFrameMove.push_back( pBall );
	listBall.push_back( pBall );

	return S_OK;
}


HRESULT CLevel::AddBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
{
	CBrick* pBrick = new CBrick( Texture, Position, Size, &listRender, &listFrameMove );
	listRender.push_back( pBrick );
	listBallObst.push_back( pBrick );

	return S_OK;
}


HRESULT CLevel::AddPaddle( LPDIRECT3DTEXTURE8 Texture )
{
	CPaddle* pPaddle = new CPaddle( Texture, pDIDevice );
	listRender.push_back( pPaddle );
	listFrameMove.push_back( pPaddle );
	listBallObst.push_back( pPaddle );

	return S_OK;
}
