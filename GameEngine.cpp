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

const DWORD INITIAL_LIVES	= 2;		// Poczatkowa iloœæ ¿yæ
const float BONUS_PROB		= 0.05f;	// Prawdopodobieñstwo wygenerowania bonusa
const DWORD MAX_BALLS		= 32;		// Maksymalna iloœc kulek
const DWORD MAX_EFFECTS		= 64;		// Maksymalna iloœæ efektów

const float BALL_SHIFT		= 0.01f;	// Pocz¹tkowe przesuniecie kulki na desce
const float FIREBALL_TAIL	= 0.005f;	// Czas pomiêdzy tworzeniem
const float EXPL_PROP_TIME	= 0.15f;	// Czas propagacji wybuchów
const float EXPL_BLOW		= 0.1f;		// Wspó³czynnik si³y podmuchu eksplozji
const float BALL_BLOW		= 10.0f;	// Wspó³czynnik si³y uderzenia kulki 
const float PADDLE_SPEED	= 1.5f;		// Czu³oœæ deski na ruchy myszy
const float GAME_SPEED		= 1.0f;		// Prêdkoœæ gry

const DWORD BONUS_SCORE		= 100;		//Iloœæ punktów za z³apanie bonusa

CGameEngine::CGameEngine()
{  
	fGameSpeed				= GAME_SPEED;
	bCheats					= false;

	bPaused					= false;

	pScoreCounter			= NULL; 
	pLivesCounter			= NULL; 
	pPaddle					= NULL;

	fTimeToExplosion		= 0;
	fTimeToBallTail			= 0;
}


CGameEngine::~CGameEngine()
{
}


HRESULT CGameEngine::OnInitDevice()
{
	CGameBoard::OnInitDevice();

	// deska
	LoadTexture( "gfx/Paddle.png",				&CPaddle::spTexture );
	LoadTexture( "gfx/Lightning.png",			&CPaddle::spLightningTexture );
	
	// kulka
	LoadTexture( "gfx/Ball_alu.png",			&CBall::spTexture );
	LoadTexture( "gfx/SparkEffect.png",			&pSparkTex );
	
	// Explozje
	LoadTexture( "gfx/Explosion.png",			&pExplosionTex );

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

	// tworzymy licznik
	LoadTexture( "gfx/Digits.png",				&CCounter::spTexture );
	POINT DigitPixels = {64,92};
	pScoreCounter = new CCounter( 0,			D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_L+BOARD_W*0.125f, 0.05f/2), DigitPixels, 6 );
	pLivesCounter = new CCounter( INITIAL_LIVES,D3DXVECTOR2(BOARD_W*0.2f, 0.05f*0.75f), D3DXVECTOR2(BOARD_R-BOARD_W*0.125f, 0.05f/2), DigitPixels, 6 );

	BoardReset();

	return S_OK;
}


HRESULT CGameEngine::OnDeleteDevice()
{
	BoardClear();

	SAFE_DELETE( pScoreCounter );
	SAFE_DELETE( pLivesCounter );

	return CGameBoard::OnDeleteDevice();
}

void CGameEngine::BoardPrepare()
{
	// tworzymy deskê z kulka
	pPaddle = new CPaddle();
	CBall* pBall = new CBall( pPaddle->vPosition + D3DXVECTOR2( BALL_SHIFT, 0), D3DXVECTOR2(0, BALL_SPEED_VAL_AVG) );
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

	listExplodingPos.clear();

	bThruBrick		= false;
	bFireBall		= false;
	//TODO:	ShootingPaddle = false;
	bFallingBricks	= false;
}

void CGameEngine::BoardReset()
{
	BoardClear();
	BoardPrepare();
}


HRESULT CGameEngine::OnMouseEvent( LPDIDEVICEOBJECTDATA didod )
{
	if (pPaddle && !bPaused)
		switch (didod->dwOfs)
		{
			case DIMOFS_X:
				pPaddle->Move( (float)(int)didod->dwData * PADDLE_SPEED / RES_X );
				break;

			case DIMOFS_BUTTON0:
				if (didod->dwData & 0x80)		// przycisk nacisniety
					pPaddle->LaunchCatchedBalls();
				break;
		}

	if (!bCheats)
		return S_OK;

	if (didod->dwOfs == DIMOFS_BUTTON1 )
		if (didod->dwData & 0x80)		// przycisk nacisniety
			fGameSpeed = GAME_SPEED/5;
		else							// przycisk puszczony
			fGameSpeed = GAME_SPEED;

	return S_OK;
}


HRESULT CGameEngine::OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod )
{
	if ( didod->dwData & 0x80 )
		switch (didod->dwOfs) {
			case DIK_P:
			case DIK_PAUSE:
				bPaused = !bPaused;
				return S_OK;
			case DIK_SYSRQ:
				bCheats = !bCheats;
				return S_OK;
			case DIK_SPACE:
				if (bCheats)
					pBrickArray->Clear();
				return S_OK;
			case DIK_B:
				if (bCheats) {
					int type = listBonus.empty() ? 0 : (listBonus.front()->GetType()+1)%CBonus::MAX_TYPE;
					CBonus* pBonus = new CBonus( static_cast<CBonus::TypeEnum>(type), D3DXVECTOR2(0.50f,0.375f), D3DXVECTOR2(0,0) );
					listBonus.push_front( pBonus );
				}
				return S_OK;
		}

	return CGameBoard::OnKeyboardEvent( didod );
}


HRESULT CGameEngine::FrameMove( float fElapsedTime )
{
	if (bPaused)
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
		
		if (dwLevelNum >= NUM_LEVELS) {
			SetCurrentScene( NULL );	// TODO: highscores :)
			return S_OK;
		}

		BoardReset();
		pBrickArray->Load( dwLevelNum );
	}

	return S_OK;
}


HRESULT CGameEngine::FrameRender()
{
	// renderujemy
	pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x40,0x60,0x60), 1.0f, 0 );

	// sceneria
	CGameBoard::FrameRender();

	pSprite->Begin();

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

	// wybuchy
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	list<CSpriteAnimated*>::iterator iExplosion;
	for (iExplosion = listExplosion.begin(); iExplosion != listExplosion.end(); iExplosion++)
		(*iExplosion)->Render( pSprite );
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// liczniki
	pScoreCounter->Render( pSprite );
	pLivesCounter->Render( pSprite );

	pSprite->End();

	return S_OK;
}


void CGameEngine::MoveObjects( float fElapsedTime )
{
	// wybuchaj¹ce cegie³ki
	fTimeToExplosion -= fElapsedTime * listExplodingPos.size();

	
	while (fTimeToExplosion < 0) {
		if (!listExplodingPos.empty()) {
			POINT pos = listExplodingPos.front();
			listExplodingPos.pop_front();
			DoExplosion( pos );
		}
		fTimeToExplosion += EXPL_PROP_TIME;
	}

	// ogon dla fireballa
	if (bFireBall)
		fTimeToBallTail -= fElapsedTime;

	while (fTimeToBallTail < 0) {
		// kulki
		list<CBall*>::iterator iBall;
		for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
			CreateFireballTail( *iBall );
		fTimeToBallTail += FIREBALL_TAIL;
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
	pPaddle->SetBallSpeed( pBall );

    if (bFallingBricks)
		pBrickArray->FallBricks();
}

void CGameEngine::CollideBallBricks( CBall* pBall )
{
	for (LONG x=0; x<2; x++)
		for (LONG y=0; y<2; y++) {
			D3DXVECTOR2 vPos = pBall->vPosition - pBall->vSize/2 + D3DXVECTOR2( pBall->vSize.x*x, pBall->vSize.y*y );
			POINT pos = pBrickArray->GetArrayCoordsAt( vPos );
			if (!pBrickArray->IsValid( pos ))
				continue;

			CBrick* pBrick = pBrickArray->GetBrick( pos );
			if ( pBrick == NULL )
				continue;

			D3DXVECTOR2 vSide = pBall->GetCollisionSide( pBrick );

			if (! bThruBrick ) {
				pBall->Reflect( pBrick, vSide );
				CreateSparkles( pBall, vSide );
			}

			if (bFireBall)
				DoExplosion( pos );
			else {
				BYTE idNextType = pBrick->GetNextType();
				if ( idNextType && !bThruBrick ) {
					listEffect.push_front( pBrick->CreateBlendEffect( D3DXVECTOR2(0,0) ) );
					pBrickArray->RemoveBrick( pos );
					pBrickArray->InsertBrick( idNextType, pos );
				}
				else
					DestroyBrick( pos, vSide * BALL_BLOW );
			}
		}
}


void CGameEngine::DoExplosion( const POINT & pos )
{
	for(int i=0; i<2; i++) {
		POINT ptFramePixels = {64,64};
		CSpriteAnimated* pAnimation = new CSpriteAnimated( pExplosionTex, D3DXVECTOR2(1.0f, 1.0f)/8, frand(0, D3DX_PI*2),
			pBrickArray->GetPositionAt( pos ) + D3DXVECTOR2(frand(-0.01f,0.01f),frand(-0.01f,0.01f)), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 0xFFFFFFFF, 1.0f, 0, 44, ptFramePixels );
		listExplosion.push_back( pAnimation );
	}

	POINT posAdj;
	for (posAdj.x=pos.x-1; posAdj.x<=pos.x+1; posAdj.x++)
		for (posAdj.y=pos.y-1; posAdj.y<=pos.y+1; posAdj.y++)
			if ( pBrickArray->IsValid(posAdj) )
				DestroyBrick( posAdj, D3DXVECTOR2( float(posAdj.x-pos.x), float(posAdj.y-pos.y) )*EXPL_BLOW );
}


void CGameEngine::DestroyBrick( const POINT & pos, const D3DXVECTOR2 & vSpeed )
{
	CBrick* pBrick = pBrickArray->GetBrick( pos );
	if (!pBrick)
		return;

	if (pBrick->IsExplosive())
		listExplodingPos.push_back( pos );

	listEffect.push_front( pBrick->CreateBlendEffect( vSpeed ) );
	pScoreCounter->lValue += pBrick->GetScore();

	if (frand(0,1) < BONUS_PROB) {
		CBonus* pBonus = new CBonus( static_cast<CBonus::TypeEnum>(rand()%CBonus::MAX_TYPE), pBrick->vPosition, vSpeed );
		listBonus.push_back( pBonus );
	}

	pBrickArray->RemoveBrick( pos );
}


void CGameEngine::ApplyBonus( DWORD Type )
{
	pScoreCounter->lValue += BONUS_SCORE;

	list<CBall*>::iterator iBall;

	switch ( Type ) {
		case CBonus::ThruBrick:
			bThruBrick = true;
			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetColor( 0xFF7FAFFF );
			break;

		case CBonus::SetOffExploding:
			pBrickArray->PushExplosive( &listExplodingPos );
			break;

		case CBonus::FireBall:
			bFireBall = true;
			if (bThruBrick)
				break;

			for (iBall = listBall.begin(); iBall != listBall.end(); iBall++)
				(*iBall)->SetColor( 0xFFFFFF7F );
			break;
		
		/*
		TODO: ShootingPaddle,
		*/

		case CBonus::GrabPaddle:
			pPaddle->bGrabPaddle = true;
			break;

//////


		case CBonus::ExtraLife:
			pLivesCounter->lValue++;
			break;

		case CBonus::LevelWarp:
			pBrickArray->Clear();
			break;

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

		case CBonus::ExpandExploding:
			pBrickArray->ExpandExploding();
			break;

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
			D3DXMatrixRotationZ( &matRotation, D3DX_PI/4 );
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


void CGameEngine::CreateSparkles( CBall* pBall, const D3DXVECTOR2 & vSide )
{
	D3DXVECTOR2 vSparkSize		= D3DXVECTOR2(1.0f/256, 1.0f/256);
	D3DXVECTOR2 vSparkPosition	= pBall->vPosition + vSide;
	D3DXVECTOR2 vSparkGravity	= D3DXVECTOR2( 0, GRAV_ACCEL );
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

void CGameEngine::CreateFireballTail( CBall* pBall )
{
	POINT ptFramePixels = {64,64};
	CSpriteAnimated* pAnimation = new CSpriteAnimated( pExplosionTex, pBall->vSize*2, frand(0, D3DX_PI*2),
		pBall->vPosition + D3DXVECTOR2(frand(-0.5f,0.5f) * pBall->vSize.x, frand(-0.5f,0.5f) * pBall->vSize.y),
		D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), bThruBrick ? 0xFF0000FF : 0xFFFFFFFF, frand(0.05f, 0.2f), 8, 44, ptFramePixels );
	listExplosion.push_back( pAnimation );
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


void CGameEngine::KillPaddle()
{
	pPaddle->LaunchCatchedBalls();
	SAFE_DELETE( pPaddle );
}
