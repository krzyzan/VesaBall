#include "stdafx.h"
#include "gameengine.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Bonus.h"
#include "Counter.h"
#include "EffectSprite.h"

CGameEngine::CGameEngine( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{  
	/*TODO: TMP*/hWnd = wnd;
	pSprite			= NULL;

	numFrameMove	= 0;
	numRender		= 0;

	bThruBrick	= false;
	bFallingBricks = false;
	
	pBonusTextures = new LPDIRECT3DTEXTURE8[ CBonus::MAX_TYPE ];
	ZeroMemory( pBonusTextures, sizeof( pBonusTextures[0] ) * CBonus::MAX_TYPE );
	ZeroMemory( pBrickTable, sizeof( pBrickTable ) );
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

	delete [] pBonusTextures;
}

HRESULT CGameEngine::InitDeviceObjects()
{
	// sceneria
	LoadTexture( "gfx/StatusBar.png", &pStatusBarTex );
	LoadTexture( "gfx/Wall.png", &pWallTex );

	// licznik
	LoadTexture( "gfx/Digits.png", &pCounterTex );

	LoadTexture( "gfx/SparkEffect.png",	&pSparkTex );
	LoadTexture( "gfx/Paddle.png",		&pPaddleTex );
	LoadTexture( "gfx/Lightning.png",	&pLightningTex );
	LoadTexture( "gfx/Ball_alu.png",	&pBallTex );
	LoadTexture( "gfx/Brick5.png",		&pBrickTex );
	
	// ³adujemy tekstury bonusów
	LoadTexture( "gfx/Bonus_ThruBrick.png",		&pBonusTextures[CBonus::ThruBrick] );
	LoadTexture( "gfx/Bonus_SetOffExploding.png",&pBonusTextures[CBonus::SetOffExploding] );
	LoadTexture( "gfx/Bonus_FireBall.png",		&pBonusTextures[CBonus::FireBall] );
	LoadTexture( "gfx/Bonus_ShootingPaddle.png",&pBonusTextures[CBonus::ShootingPaddle] );
	LoadTexture( "gfx/Bonus_GrabPaddle.png",	&pBonusTextures[CBonus::GrabPaddle] );

	LoadTexture( "gfx/Bonus_ExtraLife.png",		&pBonusTextures[CBonus::ExtraLife] );
	LoadTexture( "gfx/Bonus_LevelWarp.png",		&pBonusTextures[CBonus::LevelWarp] );
	LoadTexture( "gfx/Bonus_ZapBricks.png",		&pBonusTextures[CBonus::ZapBricks] );
	LoadTexture( "gfx/Bonus_SlowBall.png",		&pBonusTextures[CBonus::SlowBall] );
	LoadTexture( "gfx/Bonus_ExpandExploding.png",&pBonusTextures[CBonus::ExpandExploding] );

	LoadTexture( "gfx/Bonus_KillPaddle.png",	&pBonusTextures[CBonus::KillPaddle] );
	LoadTexture( "gfx/Bonus_ShrinkBall.png",	&pBonusTextures[CBonus::ShrinkBall] );
	LoadTexture( "gfx/Bonus_FastBall.png",		&pBonusTextures[CBonus::FastBall] );
	LoadTexture( "gfx/Bonus_SuperShrink.png",	&pBonusTextures[CBonus::SuperShrink] );
	LoadTexture( "gfx/Bonus_FallingBricks.png",	&pBonusTextures[CBonus::FallingBricks] );

	LoadTexture( "gfx/Bonus_ExpandPaddle.png",	&pBonusTextures[CBonus::ExpandPaddle] );
	LoadTexture( "gfx/Bonus_ShrinkPaddle.png",	&pBonusTextures[CBonus::ShrinkPaddle] );
	LoadTexture( "gfx/Bonus_SplitBall.png",		&pBonusTextures[CBonus::SplitBall] );
	LoadTexture( "gfx/Bonus_MegaBall.png",		&pBonusTextures[CBonus::MegaBall] );
	LoadTexture( "gfx/Bonus_EightBall.png",		&pBonusTextures[CBonus::EightBall] );

	// sceneria
	listRender.push_back( new CSprite( pStatusBarTex, D3DXVECTOR2(BOARD_W, BOARD_T), 0, D3DXVECTOR2(BOARD_L+BOARD_W/2, BOARD_T/2), 0xFFFFFFFF ) );
	listRender.push_back( new CSprite( pWallTex, D3DXVECTOR2(BOARD_L-0.0f, 0.75f), 0, D3DXVECTOR2((BOARD_L+0.00f)/2, 0.75f/2), 0xFFFFFFFF ) );
	listRender.push_back( new CSprite( pWallTex, D3DXVECTOR2(1.0f-BOARD_R, 0.75f), 0, D3DXVECTOR2((1.00f+BOARD_R)/2, 0.75f/2), 0xFFFFFFFF ) );

	// tworzymy licznik
	pCounter = new CCounter( pCounterTex, D3DXVECTOR2(BOARD_W*0.2f, BOARD_T*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, BOARD_T/2), 92, 6 );
	listRender.push_back( pCounter );
	listFrameMove.push_back( pCounter );	

	// tworzymy deskê z kulka
	AddPaddle( new CPaddle( pPaddleTex, pLightningTex ) );

	CBall* pBall = new CBall( pBallTex, pPaddle->vPosition + D3DXVECTOR2(0.01f, 0), D3DXVECTOR2(0,AVG_BALL_SPEED) );
	AddBall( pBall );
	pPaddle->CatchBall( pBall );


	// tworzymy cegie³ki
	for (int x=0; x<BRICK_X; x++)
		for (int y=0; y<BRICK_Y; y++)
			if (rand()%4>0) {
				pBrickTable[x][y] = new CBrick( pBrickTex, D3DXVECTOR2( BOARD_L + BOARD_W/BRICK_X*(0.5f + x), BOARD_T + BRICK_TABLE_H/BRICK_Y*(0.5f + y) ), 
					D3DXVECTOR2( BOARD_W/BRICK_X, BRICK_TABLE_H/BRICK_Y) );
				AddBrick( pBrickTable[x][y] );
			}

	timerRenderLimiter.Start();
	fTimeToRender = 0;

	return S_OK;
}

//LoadLevel( "lvl\test.lvl );

//SaveLevel(

void Reset()
{
}


HRESULT CGameEngine::RestoreDeviceObjects()
{
	D3DXCreateSprite( pd3dDevice, &pSprite );

	return S_OK;
}


HRESULT CGameEngine::RenderLoop()
{
	FLOAT fElapsedTime = timerRenderLimiter.GetElapsedTime();
	if ( fElapsedTime > 0.1 ) 
		return S_OK;	
	numFrameMove++;

	DestroyObjects();
	MoveObjects( fElapsedTime );
	CollideObjects();

///////////////////////////////////////
	
	fTimeToRender -= fElapsedTime;
	if (fTimeToRender > 0) 
		return S_OK;

	fTimeToRender = 1.0f/110;
	numRender++;

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
	list<CSprite*>::iterator iSprite = listRender.begin();
	while (iSprite != listRender.end())
		delete (*iSprite++);

	return S_OK;
}


void CGameEngine::MoveObjects( FLOAT fElapsedTime )
{
	// wykonujemy ruch dla wszystkich obiektów
	list<CMovingSprite*>::iterator	iMovingSprite;
	for (iMovingSprite = listFrameMove.begin(); iMovingSprite != listFrameMove.end(); iMovingSprite++)
		(*iMovingSprite)->FrameMove( fElapsedTime );

	DIMOUSESTATE2 dims2;
    ZeroMemory( &dims2, sizeof(dims2) );
	pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

	pPaddle->MouseMove( &dims2 );
}


void CGameEngine::ApplyBonus( DWORD Type, bool State )
{
	list<CBall*>::iterator iBall;

	switch (Type) {
		case CBonus::ThruBrick:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				(*iBall)->dwBlending = State ? 0xFFAFCFFF : 0xFFFFFFFF;
				bThruBrick = State;
			}
			break;
		/*
		SetOffExploding,
		FireBall
		ShootingPaddle,
		*/

		case CBonus::GrabPaddle:
			pPaddle->bGrabPaddle = State;
			break;

//////

		/*
		ExtraLife,
		LevelWarp,
		ZapBricks,
		*/
		case CBonus::SlowBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVec2Normalize( &(*iBall)->vSpeed, &(*iBall)->vSpeed );
				(*iBall)->vSpeed *= MIN_BALL_SPEED;
			}

		/*
		ExpandExploding,
		*/

//////

		//TODO: 
		/*
		KillPaddle,
		*/
		case CBonus::ShrinkBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVECTOR2 vNewSize = (*iBall)->vSize * 2;
				D3DXVec2Minimize( &vNewSize, &vNewSize, &D3DXVECTOR2(MIN_BALL_SIZE, MIN_BALL_SIZE) );
				(*iBall)->SetSize( vNewSize );
			}
			break;

		case CBonus::FastBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVec2Normalize( &(*iBall)->vSpeed, &(*iBall)->vSpeed );
				(*iBall)->vSpeed *= MAX_BALL_SPEED;
			}
			break;

		case CBonus::SuperShrink:
			pPaddle->SetSize( D3DXVECTOR2( MIN_PADDLE_WIDTH, pPaddle->vSize.y ) );
			break;
		
		case CBonus::FallingBricks:
			bFallingBricks = State;
			break;

//////

		case CBonus::ExpandPaddle: 
			pPaddle->MultiplyWidth( 2 );
			break;
	
		case CBonus::ShrinkPaddle:
			pPaddle->MultiplyWidth( 0.5 );
			break;
		
		case CBonus::SplitBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				if ( !(*iBall)->bCatched ) {
					CBall* pBall = new CBall( **iBall );
					pBall->vSpeed.x =  (*iBall)->vSpeed.y;
					pBall->vSpeed.y = -(*iBall)->vSpeed.x;
					AddBall( pBall );
				}
			break;

		case CBonus::MegaBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVECTOR2 vNewSize = (*iBall)->vSize * 2;
				D3DXVec2Minimize( &vNewSize, &vNewSize, &D3DXVECTOR2(MAX_BALL_SIZE, MAX_BALL_SIZE) );
				(*iBall)->SetSize( vNewSize );
			}
			break;

		case CBonus::EightBall: 
			{
				D3DXMATRIX matRotation;
				D3DXMatrixRotationZ( &matRotation, (float)M_PI_4 );
				for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
					if ( !(*iBall)->bCatched ) {
						CBall* pBall = *iBall;
						for (int i=1; i<8; i++) {
							pBall = new CBall( *pBall );
							D3DXVec2TransformCoord( &pBall->vSpeed, &pBall->vSpeed, &matRotation );
							AddBall( pBall );
						}
					}
				break;
			}
	}
}


void CGameEngine::CollideObjects()
{ 
	// odbijamy kulki
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
		for (int x=0; x<BRICK_X; x++)
			for (int y=0; y<BRICK_Y; y++)
				if (pBrickTable[x][y] != NULL)
					CollideBallBrick( *iBall, pBrickTable[x][y] );

		CollideBallPaddle( *iBall );
	}

	// ³apiemy bonusy
	list<CBonus*>::iterator iBonus;
	for (iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++)
		CatchBonus( *iBonus );
}


void CGameEngine::CollideBallBrick( CBall* pBall, CBrick* pBrick )
{
	// odbicia od cegie³ek
	D3DXVECTOR2 vSide = pBall->IsColliding( pBrick );
	if (vSide != D3DXVECTOR2(0,0)) {
		if (frand(0,1) < BONUS_PROB)
			AddBonus( new CBonus( static_cast<CBonus::TYPE>(rand()%CBonus::MAX_TYPE), 
			//AddBonus( new CBonus( CBonus::FallingBricks, 
				pBonusTextures, pBall->vPosition, pBall->vSpeed/2 ) );

		if ( !bThruBrick ) {
			pBrick->ReflectBall( pBall, vSide );
			pBall->CreateSparkles( vSide, &listRender, &listFrameMove, pSparkTex );
		}
	
		// znikanie cegie³ki
		CEffectSprite* se = pBrick->CreateEffect();
		listRender.push_front( se );
		listFrameMove.push_front( se );
		pBrick->bDeleteMe = true;

		pCounter->Increment( 100 + rand()%100 );		//TODO: jakieœ sensowne wartoœci
	}
}


void CGameEngine::CollideBallPaddle( CBall* pBall )
{
	if (pBall->bCatched)
		return;

	// odbicia od deski
	D3DXVECTOR2 vSide = pBall->IsColliding( pPaddle );
	if (vSide == D3DXVECTOR2(0,0)) 
		return;

	if (vSide.y && pPaddle->bGrabPaddle )
		pPaddle->CatchBall( pBall );
	else {
		pPaddle->LaunchBall( pBall );
		pBall->CreateSparkles( vSide, &listRender, &listFrameMove, pSparkTex );
	}

	//TODO: funkcja
    if (bFallingBricks)
		for (int x=0; x<BRICK_X; x++)
			for (int y=BRICK_Y-1; y>0; y--)
				if (pBrickTable[x][y-1] != NULL && pBrickTable[x][y] == NULL) {
					pBrickTable[x][y] = pBrickTable[x][y-1];
					pBrickTable[x][y-1] = NULL;
					pBrickTable[x][y]->vPosition.y += pBrickTable[x][y]->vSize.y;
				}
}


void CGameEngine::CatchBonus( CBonus* pBonus )
{
	D3DXVECTOR2 vSide = pBonus->IsColliding( pPaddle );
	if (vSide != D3DXVECTOR2(0,0)) {
		ApplyBonus( pBonus->dwType, true );
		pBonus->bDeleteMe = true;
		pCounter->Increment( 100 );
	}
}


void CGameEngine::RenderObjects()
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

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


void CGameEngine::DestroyObjects()
{
	list<CMovingSprite*>::iterator	iMovingSprite;
	list<CSprite*>::iterator		iSprite;
	list<CBall*>::iterator			iBall;
	list<CBrick*>::iterator			iBrick;
	list<CBonus*>::iterator			iBonus;

	// Kasujemy z listy kulek
	iBall = listBall.begin(); 
	while (iBall != listBall.end()) {
		if ((*iBall)->bDeleteMe)
			iBall = listBall.erase( iBall );
		else
			iBall++;
	}

	// Kasujemy z tablicy cegie³ek
	for (int x=0; x<BRICK_X; x++)
		for (int y=0; y<BRICK_Y; y++)
			if (pBrickTable[x][y] != NULL && pBrickTable[x][y]->bDeleteMe)
				pBrickTable[x][y] = NULL;


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

	// Kasujemy z listy renderowania i usuwamy obiekt
	iSprite = listRender.begin();
	while (iSprite != listRender.end()) {
		if ((*iSprite)->bDeleteMe) {
			delete (*iSprite);
			iSprite = listRender.erase( iSprite );
		}
		else
			iSprite++;
	}
}

void CGameEngine::AddBall( CBall* pBall )
{
	listRender.push_front( pBall );
	listFrameMove.push_front( pBall );
	listBall.push_front( pBall );
}


void CGameEngine::AddBrick( CBrick* pBrick )
{
	listRender.push_back( pBrick );
}

// TODO: Add*( new CBall ) itp.
void CGameEngine::AddPaddle( CPaddle* Paddle )
{
	pPaddle = Paddle;
	listRender.push_front( pPaddle );
}

void CGameEngine::AddBonus( CBonus* pBonus )
{
	listRender.push_back( pBonus );
	listFrameMove.push_back( pBonus );
	listBonus.push_back( pBonus );
}