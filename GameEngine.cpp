#include "StdAfx.h"
#include "GameEngine.h"
#include "GameMenu.h"

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "Bonus.h"
#include "Counter.h"
#include "SpriteEffect.h"
#include "SpriteAnimated.h"


const DWORD INITIAL_LIVES	= 2;
const float BONUS_PROB		= 0.05f;
const DWORD MAX_BALLS		= 32;
const DWORD MAX_EFFECTS		= 64;


CGameEngine::CGameEngine()
{  
	fGameSpeed				= 1.0f;
	bPaused					= false;

	pScoreCounter			= NULL; 
	pLivesCounter			= NULL; 
	pPaddle					= NULL;

	fTimeToBrickExplode		= 0;
	fTimeToBallTail			= 0;
}


CGameEngine::~CGameEngine()
{
}


void CGameEngine::BoardPrepare()
{
	// tworzymy deskê z kulka
	pPaddle = new CPaddle();
	CBall* pBall = new CBall( pPaddle->vPosition + D3DXVECTOR2(0.01f, 0), D3DXVECTOR2(0, BALL_SPEED_VAL_AVG) );
	listBall.push_front( pBall );
	pPaddle->CatchBall( pBall );
}


void CGameEngine::BoardClear()
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

	// Kasujemy z listy eksplozji
	while (!listExplosion.empty()) {
		delete (*listExplosion.begin());
		listExplosion.pop_front();
	}

	// Kasujemy z listy bonusów
	while (!listBonus.empty()) {
		delete (*listBonus.begin());
		listBonus.pop_front();
	}

	listExploding.clear();

	bThruBrick = false;
	bFireBall = true;
	//TODO:
	/*
		ShootingPaddle,
	*/
	bFallingBricks = false;
}

void CGameEngine::BoardReset()
{
	BoardClear();
	BoardPrepare();
}

HRESULT CGameEngine::InitDeviceObjects()
{
	CGameBoard::InitDeviceObjects();

	// deska
	LoadTexture( "gfx/Paddle.png",		&CPaddle::spTexture );
	LoadTexture( "gfx/Lightning.png",	&CPaddle::spLightningTexture );
	
	// kulka
	LoadTexture( "gfx/Ball_alu.png",	&CBall::spTexture );
	LoadTexture( "gfx/SparkEffect.png",	&pSparkTex );
	

	// ³adujemy tekstury bonusów
	LoadTexture( "gfx/Bonus_ThruBrick.png",		&CBonus::spTextures[CBonus::ThruBrick] );
	LoadTexture( "gfx/Bonus_SetOffExploding.png",&CBonus::spTextures[CBonus::SetOffExploding] );
	LoadTexture( "gfx/Bonus_FireBall.png",		&CBonus::spTextures[CBonus::FireBall] );
	LoadTexture( "gfx/Bonus_ShootingPaddle.png",&CBonus::spTextures[CBonus::ShootingPaddle] );
	LoadTexture( "gfx/Bonus_GrabPaddle.png",	&CBonus::spTextures[CBonus::GrabPaddle] );

	LoadTexture( "gfx/Bonus_ExtraLife.png",		&CBonus::spTextures[CBonus::ExtraLife] );
	LoadTexture( "gfx/Bonus_LevelWarp.png",		&CBonus::spTextures[CBonus::LevelWarp] );
	LoadTexture( "gfx/Bonus_ZapBricks.png",		&CBonus::spTextures[CBonus::ZapBricks] );
	LoadTexture( "gfx/Bonus_SlowBall.png",		&CBonus::spTextures[CBonus::SlowBall] );
	LoadTexture( "gfx/Bonus_ExpandExploding.png",&CBonus::spTextures[CBonus::ExpandExploding] );

	LoadTexture( "gfx/Bonus_KillPaddle.png",	&CBonus::spTextures[CBonus::KillPaddle] );
	LoadTexture( "gfx/Bonus_ShrinkBall.png",	&CBonus::spTextures[CBonus::ShrinkBall] );
	LoadTexture( "gfx/Bonus_FastBall.png",		&CBonus::spTextures[CBonus::FastBall] );
	LoadTexture( "gfx/Bonus_SuperShrink.png",	&CBonus::spTextures[CBonus::SuperShrink] );
	LoadTexture( "gfx/Bonus_FallingBricks.png",	&CBonus::spTextures[CBonus::FallingBricks] );

	LoadTexture( "gfx/Bonus_ExpandPaddle.png",	&CBonus::spTextures[CBonus::ExpandPaddle] );
	LoadTexture( "gfx/Bonus_ShrinkPaddle.png",	&CBonus::spTextures[CBonus::ShrinkPaddle] );
	LoadTexture( "gfx/Bonus_SplitBall.png",		&CBonus::spTextures[CBonus::SplitBall] );
	LoadTexture( "gfx/Bonus_MegaBall.png",		&CBonus::spTextures[CBonus::MegaBall] );
	LoadTexture( "gfx/Bonus_EightBall.png",		&CBonus::spTextures[CBonus::EightBall] );

	// licznik
	LoadTexture( "gfx/Digits.png",				&CCounter::spTexture );

	//TODO: tmp
	LoadTexture( "gfx/Explosion.png",			&pExplosionTex );

	// tworzymy licznik
	POINT DigitPixels = {64,92};
	pScoreCounter = new CCounter( 0,			D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), DigitPixels, 6 );
	pLivesCounter = new CCounter( INITIAL_LIVES,D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_R-BOARD_W*0.125f, 0.05f/2), DigitPixels, 6 );

	BoardReset();

	return S_OK;
}


HRESULT CGameEngine::ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod )
{
	if (didod->dwOfs == DIMOFS_BUTTON1 )
		if (didod->dwData & 0x80)		// przycisk nacisniety
			fGameSpeed = 0.2f;
		else							// przycisk puszczony
			fGameSpeed = 1.0f;

	if (!pPaddle)
		return S_OK;

	switch (didod->dwOfs)
    {
        case DIMOFS_X:
			pPaddle->Move( (float)(int)didod->dwData * 1.5f / RES_X );
			break;

		case DIMOFS_BUTTON0:
			if (didod->dwData & 0x80)		// przycisk nacisniety
				pPaddle->LaunchCatchedBalls();
			break;
    }

	return S_OK;
}


HRESULT CGameEngine::ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod )
{
	if ( didod->dwData & 0x80 )
		switch (didod->dwOfs) {
			case DIK_P:
				bPaused = !bPaused;
				return S_OK;
		}

    return CGameBoard::ProcessKeybrdEvent( didod );
}


HRESULT CGameEngine::FrameMove( float fElapsedTime )
{
	if (bPaused)
		return S_OK;

	pLivesCounter->lValue = 1.0f/fElapsedTime;

	//TODO: TMP
	if ( fElapsedTime > 0.1 ) 
		return S_OK;	

	MoveObjects( fElapsedTime * fGameSpeed );
	CollideObjects();

	// Kasujemy z list
	DeleteExpiredObjects( (list<CSpriteMoving*>*) &listEffect );
	DeleteExpiredObjects( (list<CSpriteMoving*>*) &listExplosion );
	DeleteExpiredObjects( (list<CSpriteMoving*>*) &listBonus );
	DeleteExpiredObjects( (list<CSpriteMoving*>*) &listBall );

	// tracimy ¿ycie
	if ( listBall.empty() || !pPaddle ) { 
		pLivesCounter->lValue--;

		// TODO: gameover :)
		if ( pLivesCounter->lValue >= 0 )
			BoardReset();
		else
			SetCurrentScene( NULL );
	}

	// przejdŸ do nastêpnego poziomu
	if ( pBrickArray->Empty() ) {
		dwLevelNum++;
		if (dwLevelNum < NUM_LEVELS) {
			BoardReset();
			pBrickArray->Load( dwLevelNum );
		}
		else
            SetCurrentScene( NULL );	// TODO: highscores :)

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

	// znikaj¹ce cegie³ki
	list<CSpriteEffect*>::iterator iEffect;
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

	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// wybuchy
	list<CSpriteAnimated*>::iterator iExplosion;
	for (iExplosion = listExplosion.begin(); iExplosion != listExplosion.end(); iExplosion++)
		(*iExplosion)->Render( pSprite );

	pSprite->End();
	pSprite->Begin();

	// liczniki
	pScoreCounter->Render( pSprite );
	pLivesCounter->Render( pSprite );

	pSprite->End();
	pD3DDevice->EndScene();

	// Show the frame on the primary surface.
	pD3DDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}


HRESULT CGameEngine::DeleteDeviceObjects()
{
	BoardClear();

	SAFE_DELETE( pScoreCounter );
	SAFE_DELETE( pLivesCounter );

	return CGameBoard::DeleteDeviceObjects();
}


void CGameEngine::MoveObjects( float fElapsedTime )
{
	// wybuchaj¹ce cegie³ki
	fTimeToBrickExplode -= fElapsedTime/**listExploding.size()*/;
	while (fTimeToBrickExplode < 0) {
		while (!listExploding.empty()) {
			POINT pos = listExploding.front();
			listExploding.pop_front();
			if (pBrickArray->GetBrick( pos )) {
				DestroyBrick( pos );
				break;
			}
		}
		fTimeToBrickExplode += 0.05f;
	}

	// ogon dla fireballa
	if (bFireBall)
		fTimeToBallTail -= fElapsedTime;

	while (fTimeToBallTail < 0) {
		// kulki
		list<CBall*>::iterator iBall;
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++) {
			POINT ptFramePixels = {64,64};
			CSpriteAnimated* pAnimation = new CSpriteAnimated( pExplosionTex, (*iBall)->vSize*2, frand(0,(float)M_PI*2),
				(*iBall)->vPosition + D3DXVECTOR2(frand(-0.5f,0.5f)*(*iBall)->vSize.x, frand(-0.5f,0.5f)*(*iBall)->vSize.y), 
				D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), bThruBrick ? 0xFF0000FF : 0xFFFFFFFF, frand(0.05f, 0.2f), 8, 44, ptFramePixels );
			listExplosion.push_back( pAnimation );
		}
		fTimeToBallTail += 0.005f;
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
	list<CSpriteEffect*>::iterator iEffect;
	for (iEffect = listEffect.begin(); iEffect != listEffect.end(); iEffect++)
		(*iEffect)->FrameMove( fElapsedTime );

	// ruch eksplozji
	list<CSpriteAnimated*>::iterator iExplosion;
	for (iExplosion = listExplosion.begin(); iExplosion != listExplosion.end(); iExplosion++)
		(*iExplosion)->FrameMove( fElapsedTime );

	// aktualizacja licznika
	pScoreCounter->Update( fElapsedTime );
	pLivesCounter->Update( fElapsedTime );
}


void CGameEngine::ApplyBonus( DWORD Type )
{
	list<CBall*>::iterator iBall;

	switch ( Type ) {
		case CBonus::ThruBrick:
			bThruBrick = true;
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetColor( 0xFFAFCFFF );
			break;
		/*
		SetOffExploding,
		*/
		case CBonus::FireBall:
			bFireBall = true;
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetColor( 0xFFFFCFAF );
			break;
		/*
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
					if (listBall.size() > MAX_BALLS) 
						continue;
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

		case CBonus::EightBall: {
			ApplyBonus( CBonus::FastBall );
			D3DXMATRIX matRotation;
			D3DXMatrixRotationZ( &matRotation, (float)M_PI_4 );
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				if ( !(*iBall)->bCatched ) {
					CBall* pBall = *iBall;
					for (int i=1; i<8; i++) {
						if (listBall.size() > MAX_BALLS) 
							continue;
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
	// odbijamy kulki od cegie³ek
	list<CBall*>::iterator iBall;
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		CollideBallBricks( *iBall );

	// jeœli nie ma deski na tym koñczymy
	if (!pPaddle)
		return;

	// odbijamy kulki od deski
	for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
		CollideBallPaddle( *iBall );

	// z³apiemy bonusy
	list<CBonus*>::iterator iBonus = listBonus.begin();
	while (iBonus != listBonus.end()) {
		if (!pPaddle)
			break;
		if ( (*iBonus)->GetCollisionSide( pPaddle ) != D3DXVECTOR2(0,0) ) {
			ApplyBonus( (*iBonus)->GetType() );
			iBonus = listBonus.erase( iBonus );
			pScoreCounter->lValue += 100;
			continue;
		}
		iBonus++;
	}
}

void CGameEngine::CollideBallPaddle( CBall* pBall )
{
	if ( pBall->bCatched )
		return;
    
	D3DXVECTOR2 vSide = pBall->GetCollisionSide( pPaddle );
	if (vSide == D3DXVECTOR2(0,0))
		return;

	if (pPaddle->bGrabPaddle && vSide.y && fabs(pBall->vPosition.x - pPaddle->vPosition.x) < pPaddle->vSize.x/3 )
		pPaddle->CatchBall( pBall );
	else {
		pBall->Reflect( pPaddle, vSide );
		CreateSparkles( pBall, vSide );
	}
	pPaddle->SetBallDirection( pBall );

    if (bFallingBricks)
		pBrickArray->FallBricks();
}

void CGameEngine::CollideBallBricks( CBall* pBall )
{
	bool bExplosion = false;

	for (LONG x=0; x<2; x++)
		for (LONG y=0; y<2; y++) {
			D3DXVECTOR2 vPos = pBall->vPosition - pBall->vSize/2 + D3DXVECTOR2( pBall->vSize.x*x, pBall->vSize.y*y );
			if (!pBrickArray->Contains( vPos ))
				continue;
				
			POINT pos = pBrickArray->GetArrayCoords( vPos );
			CBrick* pBrick = pBrickArray->GetBrick( pos );
			if ( pBrick == NULL )
				continue;

			bExplosion = bFireBall;

			if (! bThruBrick ) {
				D3DXVECTOR2 vSide = pBall->GetCollisionSide( pBrick );
				pBall->Reflect( pBrick, vSide );
				CreateSparkles( pBall, vSide );
				pBrick->Hit();
			}

			if ( pBrick->IsDestroyed() || bThruBrick )
				DestroyBrick( pos );
		}

	if (bExplosion)
		DoExplosion( pBall->vPosition );
}

void CGameEngine::DeleteExpiredObjects( list<CSpriteMoving*> *pList )
{
	list<CSpriteMoving*>::iterator iObject;
	iObject = pList->begin(); 
	while (iObject !=  pList->end()) {
		if ( (*iObject)->HasExpired() ) {
			delete (*iObject);
			iObject = pList->erase( iObject );
		}
		else
			iObject++;
	}
}


void CGameEngine::DoExplosion( const D3DXVECTOR2 & vPosition )
{
	POINT pos = pBrickArray->GetArrayCoords( vPosition );
	POINT pos1;
	for (pos1.x=max(0,pos.x-1); pos1.x<=min(pos.x+1,BRICK_ARRAY_X-1); pos1.x++)
		for (pos1.y=max(0,pos.y-1); pos1.y<=min(pos.y+1,BRICK_ARRAY_Y-1); pos1.y++) {
			if (!pBrickArray->GetBrick(pos1))
				continue;
			listExploding.push_back( pos1 );
		}

	for(int i=0; i<2; i++) {
		POINT ptFramePixels = {64,64};
		CSpriteAnimated* pAnimation = new CSpriteAnimated( pExplosionTex, D3DXVECTOR2(1.0f, 1.0f)/8, frand(0,(float)M_PI*2),
			vPosition + D3DXVECTOR2(frand(-0.01f,0.01f),frand(-0.01f,0.01f)), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 0xFFFFFFFF, 1.0f, 0, 44, ptFramePixels );
		listExplosion.push_back( pAnimation );
	}
}

void CGameEngine::DestroyBrick( const POINT & pos )
{
	CBrick* pBrick = pBrickArray->GetBrick( pos );
	if (pBrick->IsExplosive())
		DoExplosion( pBrick->vPosition );

	listEffect.push_front( pBrick->CreateBlendEffect() );
	pScoreCounter->lValue += pBrick->GetScore();

	if (frand(0,1) < BONUS_PROB) {
		CBonus* pBonus = new CBonus( static_cast<CBonus::TypeEnum>(rand()%CBonus::MAX_TYPE), pBrick->vPosition, D3DXVECTOR2( frand(-0.1f,0.1f), frand(-0.1f,0.1f) ) );
		listBonus.push_back( pBonus );
	}

	pBrickArray->RemoveBrick( pos );
}


// Iskry przy odbiciu
void CGameEngine::CreateSparkles( CBall* pBall, const D3DXVECTOR2 & vSide )
{
	D3DXVECTOR2 vSparkSize		= D3DXVECTOR2(1.0f/256, 1.0f/256);
	D3DXVECTOR2 vSparkPosition	= pBall->vPosition + vSide;
	D3DXVECTOR2 vSparkGravity	= D3DXVECTOR2( 0.0f, 0.25f );
	for (int i=0; i<8; i++) {
		if (listEffect.size() >= MAX_EFFECTS)
			continue;

		float fSparkDuration = frand(0.5f, 1.0f);
		D3DXVECTOR2 vSparkSpeed = D3DXVECTOR2( frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) )/20 + pBall->GetSpeed()/4;
		CSpriteEffect* pEffectSprite = new CSpriteEffect( pSparkTex, vSparkSize, 
			vSparkPosition, vSparkSpeed, vSparkGravity, 0xFFFFFFFF, fSparkDuration );
		listEffect.push_back( pEffectSprite );
	}
}


void CGameEngine::KillPaddle()
{
	pPaddle->LaunchCatchedBalls();
	SAFE_DELETE( pPaddle );
}
