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

	pScoreCounter = NULL; 
	pLivesCounter = NULL; 
	pPaddle = NULL;

	numFrameMove	= 0;
	numRender		= 0;
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
}

void CGameEngine::ResetBoard()
{
	if (pPaddle)
		KillPaddle();

	// Kasujemy z listy kulek
	list<CBall*>::iterator iBall = listBall.begin();
	while (iBall != listBall.end())
		delete (*iBall++);

	bThruBrick = false;

	/*
		FireBall
		ShootingPaddle,
	*/

//////

	bFallingBricks = false;

	// tworzymy deskê z kulka
	pPaddle = new CPaddle();

	CBall* pBall = new CBall( pPaddle->vPosition + D3DXVECTOR2(0.01f, 0), D3DXVECTOR2(0,BALL_SPEED_AVG) );
	listBall.push_front( pBall );
	pPaddle->CatchBall( pBall );
}

HRESULT CGameEngine::InitDeviceObjects()
{
	CGameBoard::InitDeviceObjects();

	// deska
	LoadTexture( "gfx/Paddle.png",		&CPaddle::s_pTexture );
	LoadTexture( "gfx/Lightning.png",	&CPaddle::s_pLightningTexture );
	
	// kulka
	LoadTexture( "gfx/Ball_alu.png",	&CBall::s_pTexture );
	LoadTexture( "gfx/SparkEffect.png",	&CBall::s_pSparkTexture );
	

	// ³adujemy tekstury bonusów
	LoadTexture( "gfx/Bonus_ThruBrick.png",		&CBonus::s_pTextures[CBonus::ThruBrick] );
	LoadTexture( "gfx/Bonus_SetOffExploding.png",&CBonus::s_pTextures[CBonus::SetOffExploding] );
	LoadTexture( "gfx/Bonus_FireBall.png",		&CBonus::s_pTextures[CBonus::FireBall] );
	LoadTexture( "gfx/Bonus_ShootingPaddle.png",&CBonus::s_pTextures[CBonus::ShootingPaddle] );
	LoadTexture( "gfx/Bonus_GrabPaddle.png",	&CBonus::s_pTextures[CBonus::GrabPaddle] );

	LoadTexture( "gfx/Bonus_ExtraLife.png",		&CBonus::s_pTextures[CBonus::ExtraLife] );
	LoadTexture( "gfx/Bonus_LevelWarp.png",		&CBonus::s_pTextures[CBonus::LevelWarp] );
	LoadTexture( "gfx/Bonus_ZapBricks.png",		&CBonus::s_pTextures[CBonus::ZapBricks] );
	LoadTexture( "gfx/Bonus_SlowBall.png",		&CBonus::s_pTextures[CBonus::SlowBall] );
	LoadTexture( "gfx/Bonus_ExpandExploding.png",&CBonus::s_pTextures[CBonus::ExpandExploding] );

	LoadTexture( "gfx/Bonus_KillPaddle.png",	&CBonus::s_pTextures[CBonus::KillPaddle] );
	LoadTexture( "gfx/Bonus_ShrinkBall.png",	&CBonus::s_pTextures[CBonus::ShrinkBall] );
	LoadTexture( "gfx/Bonus_FastBall.png",		&CBonus::s_pTextures[CBonus::FastBall] );
	LoadTexture( "gfx/Bonus_SuperShrink.png",	&CBonus::s_pTextures[CBonus::SuperShrink] );
	LoadTexture( "gfx/Bonus_FallingBricks.png",	&CBonus::s_pTextures[CBonus::FallingBricks] );

	LoadTexture( "gfx/Bonus_ExpandPaddle.png",	&CBonus::s_pTextures[CBonus::ExpandPaddle] );
	LoadTexture( "gfx/Bonus_ShrinkPaddle.png",	&CBonus::s_pTextures[CBonus::ShrinkPaddle] );
	LoadTexture( "gfx/Bonus_SplitBall.png",		&CBonus::s_pTextures[CBonus::SplitBall] );
	LoadTexture( "gfx/Bonus_MegaBall.png",		&CBonus::s_pTextures[CBonus::MegaBall] );
	LoadTexture( "gfx/Bonus_EightBall.png",		&CBonus::s_pTextures[CBonus::EightBall] );

	// licznik
	LoadTexture( "gfx/Digits.png", &pDigitsTex );

	ResetBoard();

	// tworzymy licznik
	pScoreCounter = new CCounter( pDigitsTex, D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), 92, 6 );
	pLivesCounter = new CCounter( pDigitsTex, D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_R-BOARD_W*0.125f, 0.05f/2), 92, 6 );
	pLivesCounter->Set( INITIAL_LIVES );

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

	//tracimy ¿ycie
	if (listBall.empty() && listEffect.empty()) { 
		if (pLivesCounter->Get() == 0)
			return E_FAIL;
		pLivesCounter->Dec(1);
		ResetBoard();
	}


	fTimeToRender -= fElapsedTime;
	if (fTimeToRender > 0) 
		return S_OK;

///////////////////////////////////////

	// renderujemy
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	pd3dDevice->BeginScene();
	pSprite->Begin();

	// sceneria
	CGameBoard::RenderLoop();

	// efekty
	list<CEffectSprite*>::iterator iEffect;
	for (iEffect = listEffect.begin(); iEffect != listEffect.end(); iEffect++)
		(*iEffect)->Render( pSprite );

	// kulki
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		(*iBall)->Render( pSprite );

	// bonusy
	list<CBonus*>::iterator iBonus;
	for (iBonus = listBonus.begin(); iBonus != listBonus.end(); iBonus++)
		(*iBonus)->Render( pSprite );

	// deska
	if (pPaddle)
		pPaddle->Render( pSprite );

	// liczniki
	pScoreCounter->Render( pSprite );
	pLivesCounter->Render( pSprite );

	pSprite->End();
	pd3dDevice->EndScene();

	// Show the frame on the primary surface.
	pd3dDevice->Present( NULL, NULL, NULL, NULL );

	fTimeToRender = 1.0f/110;
	numRender++;

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

	// Kasujemy z listy efektów
	list<CEffectSprite*>::iterator iEffect = listEffect.begin();
	while (iEffect != listEffect.end())
		delete (*iEffect++);

	SAFE_DELETE( pScoreCounter );
	SAFE_DELETE( pLivesCounter );
	SAFE_DELETE( pPaddle );

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
	pScoreCounter->FrameMove( fElapsedTime );
	pLivesCounter->FrameMove( fElapsedTime );

	// ruch deski
	if (pPaddle) {
		DIMOUSESTATE2 dims2;
		ZeroMemory( &dims2, sizeof(dims2) );
		pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

		pPaddle->MouseMove( &dims2 );
		if (dims2.rgbButtons[1])
			fGameSpeed = 0.2f;
		else
			fGameSpeed = 1.0f;
	}
}

void CGameEngine::ApplyBonus( DWORD Type )
{
	list<CBall*>::iterator iBall;

	switch (Type) {
		case CBonus::ThruBrick:
			bThruBrick = true;
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				(*iBall)->dwBlending = 0xFFAFCFFF;
			}
			break;
		/*
		SetOffExploding,
		FireBall
		ShootingPaddle,
		*/

		case CBonus::GrabPaddle:
			pPaddle->bGrabPaddle = true;
			break;

//////


		case CBonus::ExtraLife:
			pLivesCounter->Inc(1);
			break;

		/*
		LevelWarp,
		*/
		case CBonus::ZapBricks:
			pBrickArray->ZapBricks();
			break;

		case CBonus::SlowBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVec2Normalize( &(*iBall)->vSpeed, &(*iBall)->vSpeed );
				(*iBall)->vSpeed *= BALL_SPEED_MIN;
			}
			break;

		/*
		ExpandExploding,
		*/

//////

		case CBonus::KillPaddle:
			KillPaddle();
			break;

		case CBonus::ShrinkBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetSize( D3DXVECTOR2(BALL_SIZE_MIN, BALL_SIZE_MIN) );
			break;

		case CBonus::FastBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVec2Normalize( &(*iBall)->vSpeed, &(*iBall)->vSpeed );
				(*iBall)->vSpeed *= BALL_SPEED_MAX;
			}
			break;

		case CBonus::SuperShrink:
			pPaddle->SetWidth( PADDLE_WIDTH_MIN );
			break;
		
		case CBonus::FallingBricks:
			bFallingBricks = true;
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
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetSize( D3DXVECTOR2(BALL_SIZE_MAX, BALL_SIZE_MAX) );
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
	list<CBall*>::iterator iBall;
	iBall = listBall.begin();
	while (iBall != listBall.end()) {
		for (LONG x=-1; x<2; x+=2)
			for (LONG y=-1; y<2; y+=2) {
				D3DXVECTOR2 vPos = (*iBall)->vPosition + D3DXVECTOR2( (*iBall)->vSize.x/2*x, (*iBall)->vSize.y/2*y );
				if (pBrickArray->Contains( vPos )) {
					POINT pos = pBrickArray->VectorToArrayCoords( vPos );
					CBrick* pBrick = pBrickArray->pBrick[pos.x][pos.y];
					if ( pBrick == NULL )
						continue;

					D3DXVECTOR2 vSide = (*iBall)->GetContactSide( pBrick );
					D3DXVECTOR2 vOldSpeed = (*iBall)->vSpeed;

					if (! bThruBrick ) {
						pBrick->ReflectBall( *iBall, vSide );
						(*iBall)->CreateSparkles( vSide, &listEffect );
					}

					if ( pBrick->dwHitCounter == pBrick->pTypeDesc->dur || bThruBrick ) {
						pScoreCounter->Inc( 100 + rand()%100 );	//TODO: sensowne wartoœci
						if (frand(0,1) < BONUS_PROB) {
							CBonus* pBonus = new CBonus( static_cast<CBonus::TYPE>(rand()%CBonus::MAX_TYPE), (*iBall)->vPosition, vOldSpeed/2 );
							listBonus.push_back( pBonus );
						}

						// znikanie cegie³ki
						CEffectSprite* es = pBrick->CreateEffect();
						listEffect.push_front( es );

						SAFE_DELETE( pBrickArray->pBrick[pos.x][pos.y] );
					}

				}
			}

		if (pPaddle)
			if ( !(*iBall)->bCatched && (*iBall)->Overlaps( pPaddle ) ) 
				CollideBallPaddle( *iBall );

		// kasuj gdy wyjdzie poza ekran
		if ((*iBall)->vPosition.y - (*iBall)->vSize.y/2 > BOARD_B )
			iBall = listBall.erase( iBall );
		else
			iBall++;

	}


	list<CBonus*>::iterator iBonus = listBonus.begin();
	while (iBonus != listBonus.end()) {
		// z³ap bonus
		if (pPaddle)
			if ( (*iBonus)->Overlaps( pPaddle ) ) {
				ApplyBonus( (*iBonus)->dwType );
				pScoreCounter->Inc( 100 );
				iBonus = listBonus.erase( iBonus );
				continue;
			}

		// kasuj gdy wyjdzie za ekran
		if ((*iBonus)->vPosition.y - (*iBonus)->vSize.y/2 > BOARD_B )
			iBonus = listBonus.erase( iBonus );
		else
			iBonus++;
	}
}

void CGameEngine::CollideBallPaddle( CBall* pBall )
{
	D3DXVECTOR2 vSide = pBall->GetContactSide( pPaddle );

	if (vSide.y && pPaddle->bGrabPaddle )
		pPaddle->CatchBall( pBall );
	else {
		if ( vSide.y )
			pBall->vOldPosition.y = pBall->vPosition.y = pPaddle->vPosition.y - pPaddle->vSize.y/2 - pBall->vSize.y/2 - 0.001f;	//TODO: dok³adnoœæ :(
		else
			if ( vSide.x > 0 )
				pBall->vOldPosition.x = pBall->vPosition.x = pPaddle->vPosition.x - pPaddle->vSize.x/2 - pBall->vSize.x/2 - 0.001f;	//TODO: dok³adnoœæ :(
			else
				pBall->vOldPosition.x = pBall->vPosition.x = pPaddle->vPosition.x + pPaddle->vSize.x/2 + pBall->vSize.x/2 + 0.001f;	//TODO: dok³adnoœæ :(
		pPaddle->LaunchBall( pBall );
		pBall->CreateSparkles( vSide, &listEffect );
	}

    if (bFallingBricks)
		pBrickArray->FallBricks();
}


void CGameEngine::DestroyObjects()
{
	list<CEffectSprite*>::iterator iEffect;

	// Kasujemy z listy efektów
	iEffect = listEffect.begin(); 
	while (iEffect != listEffect.end()) {
		if ((*iEffect)->bDeleteMe) {
			delete (*iEffect);
			iEffect = listEffect.erase( iEffect );
		}
		else
			iEffect++;
	}
}

void CGameEngine::KillPaddle()
{
	pPaddle->LaunchAllBalls();
	SAFE_DELETE( pPaddle );
}
