#include "stdafx.h"
#include "level.h"

CLevel::CLevel( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{  
	ZeroMemory( pTex, sizeof(pTex) );
	pSprite			= NULL;
}


CLevel::~CLevel()
{
}


HRESULT CLevel::InitDeviceObjects()
{
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Bg_tree.jpg",		&pTex[0]  );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Paddle.png",		&pTex[40] );
	//D3DXCreateTextureFromFile( pd3dDevice, "gfx/Lightning.png",		&pTex[40] );	//TMP
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/SparkEffect.png",	&pTex[50] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Brick1.png",		&pTex[51] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Brick5.png",		&pTex[52] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Bonus_Fireball.png",&pTex[60] );
	D3DXCreateTextureFromFile( pd3dDevice, "gfx/Ball_alu.png",		&pTex[70] );

	//Tworzymy t³o
	//CSprite* pBackground = new CSprite( pTex[0], D3DXVECTOR2(1.0f, 0.75f), 0, D3DXVECTOR2(1.0f/2, 0.75f/2), 0xFF3F3F3F );
	//listRender.push_back( pBackground );
	
	//Tworzymy deskê
	CPaddle* pPaddle = new CPaddle( pTex[40], pDIDevice );
	listRender.push_back( pPaddle );
	listFrameMove.push_back( pPaddle );
	
	//Tworzymy cegie³ki
	for (int y=0; y<BRICK_Y/2; y++)
		for (int x=0; x<BRICK_X; x++)
			if (rand()%4>0) {
				CBrick* pBrick = new CBrick( pTex[52], D3DXVECTOR2( 1.0f/BRICK_X*(0.5f+x), 1.0f/BRICK_Y*(0.5f+y) ), &listRender, &listFrameMove );
				listRender.push_back( pBrick );
			}

	//Tworzymy kulki
	for (int i=0; i<2; i++) {
		CBall* pBall = new CBall( pTex[70], D3DXVECTOR2( frand(0.1f,0.9f), frand(0.1f,0.65f) ),
				0.5f * (*D3DXVec2Normalize( &D3DXVECTOR2(), &D3DXVECTOR2( frand(-1,1), frand(-1,1) ) ) ), 
				&listRender, &listFrameMove, pTex[50] );
		listRender.push_back( pBall );
		listFrameMove.push_back( pBall );
		listBall.push_back( pBall );
	}

	bThruBrick = FALSE;
	
	return S_OK;
}

HRESULT CLevel::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CLevel::FrameMove( FLOAT fElapsedTime )
{
	list<CMovingSprite*>::iterator	iMovingSprite;
	list<CSprite*>::iterator		iSprite;
	list<CBall*>::iterator			iBall;

	// wykonujemy ruch dla wszystkich obiektów
	for (iMovingSprite = listFrameMove.begin(); iMovingSprite != listFrameMove.end(); iMovingSprite++)
		(*iMovingSprite)->FrameMove( fElapsedTime );

	// wykonujemy odbicia dla wszystkich obiektów
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Collide( &listBall, bThruBrick );

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

	//GAME OVER !!!!
	if (listBall.empty())
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel::Render()
{
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

	for (int i=0; i<256; i++)
		SAFE_RELEASE( pTex[i] );
	
	return S_OK;
}
