#include "stdafx.h"
#include "gameengine.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Bonus.h"

CGameEngine::CGameEngine( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{  
	/*TODO: TMP*/hWnd = wnd;
	pSprite			= NULL;

	numFrameMove	= 0;
	numRender		= 0;

	pGameStates = new BOOL[ CBonus::MAX_TYPE ];
	ZeroMemory( pGameStates, sizeof(*pGameStates) * CBonus::MAX_TYPE );

	pBonusTextures = new LPDIRECT3DTEXTURE8[ CBonus::MAX_TYPE ];
	ZeroMemory( pGameStates, sizeof(*pBonusTextures) * CBonus::MAX_TYPE );

}


CGameEngine::~CGameEngine()
{
	char str[100] = "FrameMove() / Render(): ";
	char* p = str + strlen(str);
	_gcvt(((FLOAT)numFrameMove)/numRender, 4, p);
	p = str + strlen(str);
	strcat( p, "\nRender() / sec: ");
	p = str + strlen(str);
	_gcvt(numRender/timerRenderLimiter.GetTime(), 4, p);

	MessageBox( hWnd, str, "Internal counters", MB_OK );

	delete [] pGameStates;
	delete [] pBonusTextures;
}

HRESULT CGameEngine::InitDeviceObjects()
{
	LPDIRECT3DTEXTURE8 pPaddleTex;
	LPDIRECT3DTEXTURE8 pBrickTex;
	LPDIRECT3DTEXTURE8 pLightningTex;
	LPDIRECT3DTEXTURE8 pBallTex;
	LPDIRECT3DTEXTURE8 pSparkTex;

	LoadTexture( "gfx/SparkEffect.png", &pSparkTex );
	LoadTexture( "gfx/Paddle.png", &pPaddleTex );
	LoadTexture( "gfx/Lightning.png", &pLightningTex );
	LoadTexture( "gfx/Ball_alu.png", &pBallTex );
	LoadTexture( "gfx/Bonus_ThruBrick.png",			&pBonusTextures[CBonus::GhostBall] );
	LoadTexture( "gfx/Bonus_MagneticPaddle.png",	&pBonusTextures[CBonus::MagneticPaddle] );

	CBall::PrepareEnvironment( &listRender, &listFrameMove, pSparkTex);
	CBonus::PrepareEnvironment( pBonusTextures );
	CBrick::PrepareEnvironment( &listRender, &listFrameMove );

	//Tworzymy deskê z kulka
	//TODO: gdy dodajemy kulke po desce b³yskawica pojawia sie w z³ym mejscu
	AddPaddle( pPaddleTex, pLightningTex, pBallTex );

	//Tworzymy cegie³ki
	LoadTexture( "gfx/Brick5.png", &pBrickTex );

	float x, y;
	for (y=BOARD_T+BOARD_H/2/BRICK_Y; y<BOARD_T+BOARD_H*0.50; y+=BOARD_H/BRICK_Y)
		for (x=BOARD_L+BOARD_W/2/BRICK_X; x<BOARD_R; x+=BOARD_W/BRICK_X)
			if (rand()%4>0)
				AddBrick( pBrickTex, D3DXVECTOR2( x, y ), 
					D3DXVECTOR2( BOARD_W/BRICK_X, BOARD_H/BRICK_Y) );

	timerRenderLimiter.Start();
	fTimeToRender = 0;

	return S_OK;
}


HRESULT CGameEngine::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CGameEngine::RenderLoop()
{
	FLOAT fElapsedTime = timerRenderLimiter.GetElapsedTime();
	// TMP: nie wiem czemu u mnie na starcie timer zawiesza siê na 2 sek. 
	// wtedy nic nie rób ( mo¿e czas wymieniæ BIOS??? )
	if ( fElapsedTime > 0.1 ) 
		return S_OK;	
	numFrameMove++;

	DestroyObjects();
	MoveObjects( fElapsedTime );

///////////////////////////////////////
	
	fTimeToRender -= fElapsedTime;
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	numRender++;

///////////////////////////////////////

	RenderObjects();

	return S_OK;
}


HRESULT CGameEngine::InvalidateDeviceObjects()
{
	SAFE_RELEASE( pSprite );

	return S_OK;
}


HRESULT CGameEngine::DeleteDeviceObjects()
{
	// Kasujemy z listy renderowania
	list<CObject*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}


HRESULT CGameEngine::MoveObjects( FLOAT fElapsedTime )
{
	// wykonujemy ruch dla wszystkich obiektów
	list<CMovingSprite*>::iterator	iMovingSprite;
	for (iMovingSprite = listFrameMove.begin(); iMovingSprite != listFrameMove.end(); iMovingSprite++)
		(*iMovingSprite)->FrameMove( fElapsedTime );

	// obliczamy kulki
	for (list<CBall*>::iterator iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
		// odbicia od cegie³ek
		for (list<CBrick*>::iterator iBrick = listBrick.begin(); iBrick != listBrick.end(); iBrick++) {
			D3DXVECTOR2 vSide = (*iBall)->IsColliding( *iBrick );
			if (vSide != D3DXVECTOR2(0,0)) {
				if (!pGameStates[CBonus::GhostBall] )
					(*iBrick)->ReflectBall( *iBall, vSide );

				if (frand(0,1) < BONUS_PROBABILITY) {
					//TODO: AddBonus();
					CBonus* pBonus = new CBonus( static_cast<CBonus::TYPE>(rand()%CBonus::MAX_TYPE), (*iBall)->vPosition, (*iBall)->vSpeed/2 );
					listRender.push_back( pBonus );
					listFrameMove.push_back( pBonus );
					listBonus.push_back( pBonus );
				}

				(*iBrick)->bDeleteMe = true;
			}
		}

		// odbicia od deski
		D3DXVECTOR2 vSide = (*iBall)->IsColliding( pPaddle );
		if (vSide != D3DXVECTOR2(0,0)) {
			if (vSide.y)
				(*iBall)->vPosition.y = pPaddle->vPosition.y - pPaddle->vSize.y/2 - (*iBall)->vSize.y/2;
			if (vSide.y && pGameStates[ CBonus::MagneticPaddle ])
				pPaddle->CatchBall( (*iBall) );
			else {
				pPaddle->LaunchBall( (*iBall), D3DXVec2Length( &(*iBall)->vSpeed ) );
				(*iBall)->StrikeSparkles( vSide );
			}
		}

		//TODO: kolor kulek zmnieniæ raz a nie milion razy na sekunde :)
		if (pGameStates[ CBonus::GhostBall ])
			(*iBall)->dwBlending = 0xFFAFCFFF;
		else
			(*iBall)->dwBlending = 0xFFFFFFFF;
	}


	// obliczamy czy z³apano bonusy
	for (list<CBonus*>::iterator iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++) {
		D3DXVECTOR2 vSide = (*iBonus)->IsColliding( pPaddle );
		if (vSide != D3DXVECTOR2(0,0)) {
			pGameStates[(*iBonus)->eType ] = true;
			(*iBonus)->bDeleteMe = true;
		}
	}


	return S_OK;
}


HRESULT CGameEngine::DestroyObjects()
{
	list<CMovingSprite*>::iterator	iMovingSprite;
	list<CObject*>::iterator		iSprite;

	list<CBall*>::iterator		iBall;
	list<CBrick*>::iterator		iBrick;
	list<CBonus*>::iterator		iBonus;

	// Kasujemy z listy kulek
	iBall = listBall.begin(); 
	while (iBall != listBall.end()) {
		if ((*iBall)->bDeleteMe)
			iBall = listBall.erase( iBall );
		else
			iBall++;
	}

	// Kasujemy z listy cegie³ek
	iBrick = listBrick.begin(); 
	while (iBrick != listBrick.end()) {
		if ((*iBrick)->bDeleteMe)
			iBrick = listBrick.erase( iBrick );
		else
			iBrick++;
	}

	// Kasujemy z listy bonusów
	iBonus = listBonus.begin(); 
	while (iBonus != listBonus.end()) {
		if ((*iBonus)->bDeleteMe)
			iBonus = listBonus.erase( iBonus );
		else
			iBonus++;
	}

	// Kasujemy z listy obiektów ruchomych
	iMovingSprite = listFrameMove.begin(); 
	while (iMovingSprite != listFrameMove.end()) {
		if ((*iMovingSprite)->bDeleteMe)
			iMovingSprite = listFrameMove.erase( iMovingSprite );
		else
			iMovingSprite++;
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


HRESULT CGameEngine::RenderObjects()
{
	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00,0x00,0x00), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	list<CObject*>::iterator iSprite;
	for (iSprite = listRender.begin(); iSprite != listRender.end(); iSprite++)
		(*iSprite)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}


CBall* CGameEngine::AddBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
{
	CBall* pBall = new CBall( Texture, Position, Speed );
	listRender.push_front( pBall );
	listFrameMove.push_front( pBall );
	listBall.push_front( pBall );

	return pBall;
}


CBrick* CGameEngine::AddBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size )
{
	CBrick* pBrick = new CBrick( Texture, Position, Size );
	listRender.push_back( pBrick );
	listBrick.push_back( pBrick );

	return pBrick;
}

CPaddle* CGameEngine::AddPaddle( LPDIRECT3DTEXTURE8 PaddleTex, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECT3DTEXTURE8 BallTex )
{
	pPaddle = new CPaddle( PaddleTex, LightningTex, pDIDevice );
	listRender.push_back( pPaddle );
	listFrameMove.push_back( pPaddle );
    
	CBall* pBall = AddBall( BallTex, pPaddle->vPosition + D3DXVECTOR2(0.01f, 0), D3DXVECTOR2() );
	pBall->vPosition.y = pPaddle->vPosition.y - pPaddle->vSize.y/2 - pBall->vSize.y/2;	//TODO: TMP
	pPaddle->CatchBall( pBall );

	return pPaddle;
}
