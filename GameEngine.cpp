#include "StdAfx.h"
#include "GameEngine.h"
#include "GameMenu.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Bonus.h"
#include "Counter.h"
#include "EffectSprite.h"


const DWORD INITIAL_LIVES = 2;

CGameEngine::CGameEngine( LPDIRECT3DDEVICE8 d3dDevice )
	: CGameBoard( d3dDevice )
{  
	fGameSpeed		= 1.0f;

	pScoreCounter	= NULL; 
	pLivesCounter	= NULL; 
	pPaddle			= NULL;

	fTimeToExplode	= 0;
}


CGameEngine::~CGameEngine()
{
}

void CGameEngine::SetBoard()
{
	// tworzymy deskê z kulka
	pPaddle = new CPaddle();
	CBall* pBall = new CBall( pPaddle->vPosition + D3DXVECTOR2(0.01f, 0), D3DXVECTOR2(0, BALL_SPEED_VAL_AVG) );
	listBall.push_front( pBall );
	pPaddle->CatchBall( pBall );
}


void CGameEngine::UnsetBoard()
{
	SAFE_DELETE( pPaddle );

	// Kasujemy z listy kulek
	while (!listBall.empty()) {
		delete (*listBall.begin());
		listBall.pop_front();
	}

	// Kasujemy z listy efektów
	while (!listEffect.empty()) {
		delete (*listEffect.begin());
		listEffect.pop_front();
	}

	// Kasujemy z listy bonusów
	while (!listBonus.empty()) {
		delete (*listBonus.begin());
		listBonus.pop_front();
	}

	listExploding.clear();

	bThruBrick = false;
	//TODO:
	/*
		FireBall
		ShootingPaddle,
	*/
	bFallingBricks = false;
}

void CGameEngine::ResetBoard()
{
	UnsetBoard();
	SetBoard();
}

HRESULT CGameEngine::InitDeviceObjects()
{
	CGameBoard::InitDeviceObjects();

	// deska
	LoadTexture( "gfx/Paddle.png",		&CPaddle::s_pTexture );
	LoadTexture( "gfx/Lightning.png",	&CPaddle::s_pLightningTexture );
	
	// kulka
	LoadTexture( "gfx/Ball_alu.png",	&CBall::s_pTexture );
	LoadTexture( "gfx/SparkEffect.png",	&pSparkTex );
	

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
	LoadTexture( "gfx/Digits.png", &CCounter::s_pTexture );

	ResetBoard();

	// tworzymy licznik
	pScoreCounter = new CCounter( 0,			D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), 92, 6 );
	pLivesCounter = new CCounter( INITIAL_LIVES,D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_R-BOARD_W*0.125f, 0.05f/2), 92, 6 );

	return S_OK;
}

HRESULT CGameEngine::ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod )
{
	if (didod->dwOfs == DIMOFS_BUTTON1 )
		if (didod->dwData & 0x80)		// przycisk nacisniety
			fGameSpeed *= 0.2f;
		else							// przycisk puszczony
			fGameSpeed /= 0.2f;

	if (!pPaddle)
		return S_OK;

	list<CBall*>::iterator iBall;
	switch (didod->dwOfs)
    {
        case DIMOFS_X:
			pPaddle->Move( (float)(int)didod->dwData * 1.5f / RES_X );
			break;

		case DIMOFS_BUTTON0:
			if (didod->dwData & 0x80)		// przycisk nacisniety
				pPaddle->LaunchAllBalls();
			break;
    }

	return S_OK;
}


HRESULT CGameEngine::ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )
{
	return CGameBoard::ProcessKeybrdEvent( didod );
}


HRESULT CGameEngine::FrameMove( float fElapsedTime )
{
	if ( fElapsedTime > 0.1 ) 
		return S_OK;	

	// Kasujemy z listy efektów
	list<CEffectSprite*>::iterator iEffect = listEffect.begin(); 
	while (iEffect != listEffect.end()) {
		if ( (*iEffect)->Expired() ) {
			delete (*iEffect);
			iEffect = listEffect.erase( iEffect );
		}
		else
			iEffect++;
	}

	MoveObjects( fElapsedTime * fGameSpeed );
	CollideObjects();

	// tracimy ¿ycie
	if ( listBall.empty() || !pPaddle && listEffect.empty() ) { 
		pLivesCounter->lValue--;
		ResetBoard();
	}

	// przejdŸ do nastêpnego poziomu
	if ( listEffect.empty() && pBrickArray->Empty() ) {
		dwLevelNum++;
		if (dwLevelNum < NUM_LEVELS) {
			ResetBoard();
			pBrickArray->Load( dwLevelNum );
		}
	}



	return S_OK;
}


HRESULT CGameEngine::FrameRender()
{
	// renderujemy
	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	pD3DDevice->BeginScene();
	pSprite->Begin();

	// sceneria
	CGameBoard::FrameRender();

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
	pD3DDevice->EndScene();

	return S_OK;
}


CD3DScene* CGameEngine::GetNextScene()
{
	if ( pLivesCounter->lValue < 0 )
		// TODO: gameover :)
		return NULL;

	if ( dwLevelNum > NUM_LEVELS-1 )
		// TODO: highscores :)
		return NULL;

	return CGameBoard::GetNextScene();
}


HRESULT CGameEngine::DeleteDeviceObjects()
{
	UnsetBoard();

	SAFE_DELETE( pScoreCounter );
	SAFE_DELETE( pLivesCounter );

	return CGameBoard::DeleteDeviceObjects();
}


void CGameEngine::MoveObjects( float fElapsedTime )
{
	fTimeToExplode -= fElapsedTime/**listExploding.size()*/;

	if (fTimeToExplode < 0) {
		while (!listExploding.empty()) {
			POINT pos = listExploding.front();
			listExploding.pop_front();
			if (pBrickArray->GetBrick( pos )) {
				Explode( pos );
				break;
			}
		}
		fTimeToExplode += 0.01f;
	}

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
	pScoreCounter->Update( fElapsedTime );
	pLivesCounter->Update( fElapsedTime );
}


void CGameEngine::ApplyBonus( CBonus* pBonus )
{
	list<CBall*>::iterator iBall;

	switch ( pBonus->GetType() ) {
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
			pLivesCounter->lValue++;
			break;

		/*
		LevelWarp,
		*/
		case CBonus::ZapBricks:
			pBrickArray->ZapBricks();
			break;

		case CBonus::SlowBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVECTOR2 vSpeed = (*iBall)->GetSpeed();
				D3DXVec2Normalize( &vSpeed, &vSpeed );
				(*iBall)->SetSpeed( vSpeed * BALL_SPEED_VAL_MIN );
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
				(*iBall)->SetSize( BALL_SIZE_MIN );
			break;

		case CBonus::FastBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
				D3DXVECTOR2 vSpeed = (*iBall)->GetSpeed();
				D3DXVec2Normalize( &vSpeed, &vSpeed );
				(*iBall)->SetSpeed( vSpeed * BALL_SPEED_VAL_MAX );
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
					D3DXVECTOR2 vSpeed = (*iBall)->GetSpeed();
					pBall->SetSpeed( D3DXVECTOR2(vSpeed.y, -vSpeed.x) );
					listBall.push_front( pBall );
				}
			break;

		case CBonus::MegaBall:
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetSize( BALL_SIZE_MAX );
			break;

		case CBonus::EightBall: 
			{
				D3DXMATRIX matRotation;
				D3DXMatrixRotationZ( &matRotation, (float)M_PI_4 );
				for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
					if ( !(*iBall)->bCatched ) {
						CBall* pBall = *iBall;
						for (int i=1; i<8; i++) {
							if (listBall.size() > 100) continue;
							pBall = new CBall( *pBall );
							D3DXVECTOR2 vNewSpeed = pBall->GetSpeed();
							D3DXVec2TransformCoord( &vNewSpeed, &vNewSpeed, &matRotation );
							pBall->SetSpeed( vNewSpeed );
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
		CollideBallBricks( *iBall );

		if (pPaddle)
			if ( !(*iBall)->bCatched && (*iBall)->IsColliding( pPaddle ) ) 
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
			if ( (*iBonus)->IsColliding( pPaddle ) ) {
				ApplyBonus( *iBonus );
				pScoreCounter->lValue += 100;
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
	D3DXVECTOR2 vSide = pBall->GetCollisionSide( pPaddle );

	if (vSide.y && pPaddle->bGrabPaddle && fabs(pBall->vPosition.x - pPaddle->vPosition.x) < pPaddle->vSize.x/3 )
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
		CreateSparkles( pBall, vSide );
	}

    if (bFallingBricks)
		pBrickArray->FallBricks();
}

void CGameEngine::CollideBallBricks( CBall* pBall )
{
	for (LONG x=0; x<2; x++)
		for (LONG y=0; y<2; y++) {
			D3DXVECTOR2 vPos = pBall->vPosition - pBall->vSize/2 + D3DXVECTOR2( pBall->vSize.x*x, pBall->vSize.y*y );
			if (!pBrickArray->Contains( vPos ))
				continue;
				
			POINT pos = pBrickArray->GetArrayCoords( vPos );
			CBrick* pBrick = pBrickArray->GetBrick( pos );
			if ( pBrick == NULL )
				continue;

			D3DXVECTOR2 vSide = pBall->GetCollisionSide( pBrick );
			D3DXVECTOR2 vBonusSpeed = pBall->GetSpeed() / 2;

			if (! bThruBrick ) {
				pBall->Reflect( pBrick, vSide );
				pBrick->Hit();
				CreateSparkles( pBall, vSide );
			}

			if ( pBrick->IsDestroyed() || bThruBrick ) {
				pScoreCounter->lValue += pBrick->GetScore();
				if (frand(0,1) < BONUS_PROB) {
					CBonus* pBonus = new CBonus( static_cast<CBonus::TypeEnum>(rand()%CBonus::MAX_TYPE), pBall->vPosition, vBonusSpeed );
					listBonus.push_back( pBonus );
				}
				
				Explode( pos );
			}
		}
}


void CGameEngine::Explode( const POINT & pos)
{
	CBrick* pBrick = pBrickArray->GetBrick( pos );
	if (pBrick->IsExplosive()) {
		POINT pos1;
		for (pos1.x=max(0,pos.x-1); pos1.x<=min(pos.x+1,BRICK_ARRAY_X-1); pos1.x++)
			for (pos1.y=max(0,pos.y-1); pos1.y<=min(pos.y+1,BRICK_ARRAY_Y-1); pos1.y++) {
				if (!pBrickArray->GetBrick(pos1))
					continue;

				listExploding.push_back( pos1 );
			}
	}

	listEffect.push_front( pBrick->CreateBlendEffect() );
	pBrickArray->RemoveBrick( pos );
}


// Iskry przy odbiciu
void CGameEngine::CreateSparkles( CBall* pBall, const D3DXVECTOR2 & vSide )
{
	if (listEffect.size() > 200) return;
	D3DXVECTOR2 vSparkSize		= D3DXVECTOR2(1.0f/256, 1.0f/256);
	D3DXVECTOR2 vSparkPosition	= pBall->vPosition + vSide;
	D3DXVECTOR2 vSparkGravity	= D3DXVECTOR2( 0.0f, 0.25f );
	for (int i=0; i<8; i++) {
		float fSparkDuration = frand(0.5f, 1.0f);
		D3DXVECTOR2 vSparkSpeed = D3DXVECTOR2( frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) )/20 + pBall->GetSpeed()/4;
		CEffectSprite* pEffectSprite = new CEffectSprite( pSparkTex, vSparkSize, 
			vSparkPosition, vSparkSpeed, vSparkGravity, 0xFFFFFFFF, fSparkDuration );
		listEffect.push_back( pEffectSprite );
	}
}


void CGameEngine::KillPaddle()
{
	pPaddle->LaunchAllBalls();
	SAFE_DELETE( pPaddle );
}
