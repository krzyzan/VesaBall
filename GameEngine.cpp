#include "StdAfx.h"
#include "GameEngine.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Bonus.h"
#include "Counter.h"
#include "EffectSprite.h"

CGameEngine::CGameEngine( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CGameBoard( d3dDevice, DIDevice )
{  
	/*TODO: TMP*/hWnd = wnd;
	fGameSpeed = 1.0f;

	numFrameMove	= 0;
	numRender		= 0;

	bThruBrick	= false;
	bFallingBricks = false;
	
	pBonusTextures = new LPDIRECT3DTEXTURE8[ CBonus::MAX_TYPE ];
	ZeroMemory( pBonusTextures, sizeof( pBonusTextures[0] ) * CBonus::MAX_TYPE );
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
	CGameBoard::InitDeviceObjects();

	// licznik
	LoadTexture( "gfx/Digits.png", &pCounterTex );

	LoadTexture( "gfx/SparkEffect.png",	&pSparkTex );
	LoadTexture( "gfx/Paddle.png",		&pPaddleTex );
	LoadTexture( "gfx/Lightning.png",	&pLightningTex );
	LoadTexture( "gfx/Ball_alu.png",	&pBallTex );
	
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

	// tworzymy licznik
	pCounter = new CCounter( pCounterTex, D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), 92, 6 );
	listRender.push_back( pCounter );

	// tworzymy deskê z kulka
	InsertPaddle( new CPaddle( pPaddleTex, pLightningTex ) );

	CBall* pBall = new CBall( pBallTex, pPaddle->vPosition + D3DXVECTOR2(0.01f, 0), D3DXVECTOR2(0,AVG_BALL_SPEED) );
	listBall.push_front( pBall );
	pPaddle->CatchBall( pBall );

	timerRenderLimiter.Start();
	fTimeToRender = 0;

	return S_OK;
}


HRESULT CGameEngine::RenderLoop()
{
	FLOAT fElapsedTime = timerRenderLimiter.GetElapsedTime();
	if ( fElapsedTime > 0.1 ) 
		return S_OK;	
	numFrameMove++;

	DestroyObjects();
	MoveObjects( fElapsedTime * fGameSpeed );
	CollideObjects();

	fTimeToRender -= fElapsedTime;
	if (fTimeToRender > 0) 
		return S_OK;

///////////////////////////////////////

	fTimeToRender = 1.0f/110;
	numRender++;

	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	// sceneria
	CGameBoard::RenderLoop();

	// kulki
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		(*iBall)->Render( pSprite );

	// bonusy
	list<CBonus*>::iterator iBonus;
	for (iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++)
		(*iBonus)->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}

HRESULT CGameEngine::DeleteDeviceObjects()
{
	// Kasujemy z listy kulek
	list<CBall*>::iterator iBall = listBall.begin();
	while (iBall != listBall.end())
		delete (*iBall++);

	// Kasujemy z listy bonusów
	list<CBonus*>::iterator iBonus = listBonus.begin();
	while (iBonus != listBonus.end())
		delete (*iBonus++);

	return CGameBoard::DeleteDeviceObjects();
}


void CGameEngine::MoveObjects( FLOAT fElapsedTime )
{
	// ruch kulek
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		(*iBall)->FrameMove( fElapsedTime );

	// ruch bonusów
	list<CBonus*>::iterator iBonus;
	for (iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++)
		(*iBonus)->FrameMove( fElapsedTime );

	// ruch efektów
	list<CEffectSprite*>::iterator iEffect;
	for (iEffect = listEffect.begin(); iEffect != listEffect.end(); iEffect++)
		(*iEffect)->FrameMove( fElapsedTime );

	// aktualizacja licznika
	pCounter->FrameMove( fElapsedTime );

	// ruch deski
	DIMOUSESTATE2 dims2;
    ZeroMemory( &dims2, sizeof(dims2) );
	pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

	pPaddle->MouseMove( &dims2 );
	if (dims2.rgbButtons[1])
		fGameSpeed = 0.2f;
	else
		fGameSpeed = 1.0f;
}

//TODO: true i false dla wszystkich bonusów

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
			break;

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
			pPaddle->SetWidth( MIN_PADDLE_WIDTH );
			break;
		
		case CBonus::FallingBricks:
			bFallingBricks = State;
			break;

//////

		case CBonus::ExpandPaddle: 
			pPaddle->SetWidth( pPaddle->vSize.x * 2 );
			break;
	
		case CBonus::ShrinkPaddle:
			pPaddle->SetWidth( pPaddle->vSize.x / 2 );
			break;
		
		case CBonus::SplitBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				if ( !(*iBall)->bCatched ) {
					CBall* pBall = new CBall( **iBall );
					pBall->vSpeed.x =  (*iBall)->vSpeed.y;
					pBall->vSpeed.y = -(*iBall)->vSpeed.x;
					listBall.push_front( pBall );
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
							listBall.push_front( pBall );
						}
					}
				break;
			}
	}
}


void CGameEngine::CollideObjects()
{ 
	// odbijamy kulki
	POINT pos;
	list<CBall*>::iterator iBall = listBall.begin();
	while (iBall != listBall.end()) {
		for (pos.x=0; pos.x<BRICK_X; pos.x++)
			for (pos.y=0; pos.y<BRICK_Y; pos.y++)
				CollideBallBrick( *iBall, pos );

		CollideBallPaddle( *iBall );

		// kasuj gdy wyjdzie za ekran
		if ((*iBall)->vPosition.y - (*iBall)->vSize.y/2 > BOARD_B )
			iBall = listBall.erase( iBall );
		else
			iBall++;
	}

	// ³apiemy bonusy
	list<CBonus*>::iterator iBonus = listBonus.begin();
	while (iBonus != listBonus.end()) {
		D3DXVECTOR2 vSide = (*iBonus)->IsColliding( pPaddle );
		if (vSide != D3DXVECTOR2(0,0)) {
			ApplyBonus( (*iBonus)->dwType, true );
			iBonus = listBonus.erase( iBonus );
			pCounter->Increment( 100 );
		}
		else 
			// kasuj gdy wyjdzie za ekran
			if ((*iBonus)->vPosition.y - (*iBonus)->vSize.y/2 > BOARD_B )
				iBonus = listBonus.erase( iBonus );
			else
				iBonus++;
	}
}


void CGameEngine::CollideBallBrick( CBall* pBall, const POINT & pos )
{
	CBrick* pBrick = pBrickTable[pos.x][pos.y];
	if (pBrick == NULL)
		return;

	// odbicia od cegie³ek
	D3DXVECTOR2 vSide = pBall->IsColliding( pBrick );
	if (vSide == D3DXVECTOR2(0,0))
		return;

	if (bThruBrick)
		pBrick->dwDurability = 0;
	else
		if (pBrick->dwDurability != 0xFFFFFFFF)
			pBrick->dwDurability--;

	if (pBrick->dwDurability == 0)
		if (frand(0,1) < BONUS_PROB) {
			CBonus* pBonus = new CBonus( static_cast<CBonus::TYPE>(rand()%CBonus::MAX_TYPE), 
				pBonusTextures, pBall->vPosition, pBall->vSpeed/2 );
			listBonus.push_back( pBonus );
		}


	if (! bThruBrick ) {
		pBrick->ReflectBall( pBall, vSide );
		pBall->CreateSparkles( vSide, &listRender, &listEffect, pSparkTex );
	}

	if (pBrick->dwDurability == 0) {
		// znikanie cegie³ki
		CEffectSprite* es = pBrick->CreateEffect();
		listRender.push_front( es );
		listEffect.push_front( es );

		RemoveBrick( pos );
	}

	pCounter->Increment( 100 + rand()%100 );		//TODO: wsadziæ w dobre miejsce
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
		pBall->CreateSparkles( vSide, &listRender, &listEffect, pSparkTex );
	}

	//TODO: funkcja
    if (bFallingBricks)
		for (int x=0; x<BRICK_X; x++)
			for (int y=BRICK_Y-1; y>0; y--)
				if (pBrickTable[x][y-1] != NULL && 
						pBrickTable[x][y-1]->dwDurability != 0xFFFFFFFF && 
						pBrickTable[x][y] == NULL) {
					pBrickTable[x][y] = pBrickTable[x][y-1];
					pBrickTable[x][y-1] = NULL;
					pBrickTable[x][y]->vPosition.y += pBrickTable[x][y]->vSize.y;
				}
}


void CGameEngine::DestroyObjects()
{
	list<CMovingSprite*>::iterator	iMovingSprite;
	list<CEffectSprite*>::iterator	iEffect;
	list<CSprite*>::iterator		iSprite;

	// Kasujemy z listy efektów
	iEffect = listEffect.begin(); 
	while (iEffect != listEffect.end()) {
		if ((*iEffect)->bDeleteMe)
			iEffect = listEffect.erase( iEffect );
		else
			iEffect++;
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

void CGameEngine::InsertPaddle( CPaddle* Paddle )
{
	pPaddle = Paddle;
	listRender.push_front( pPaddle );
}

