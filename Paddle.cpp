// Paddle.cpp: implementation of the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"

BOOL*		CPaddle::s_pGameStates;

CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECTINPUTDEVICE8 DIDevice )
	: CMovingSprite( Texture, D3DXVECTOR2(1.0f/8, 1.0f/64), 0, 
		D3DXVECTOR2(BOARD_L+BOARD_W/2, BOARD_B-1.0f/64), 
		D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0xFFFFFFFF)
{
	pLightningPaddle = new CSprite( LightningTex, D3DXVECTOR2( vSize.x, vSize.y*2), fRotation, vPosition - D3DXVECTOR2(0,vSize.y/2), dwBlending );
	pDIDevice = DIDevice;
}

CPaddle::~CPaddle()
{
	delete pLightningPaddle;
}

CPaddle::PrepareEnvironment( BOOL* pGameStates )
{
	s_pGameStates = pGameStates;
}

HRESULT CPaddle::FrameMove( FLOAT fElapsedTime )
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) 
		return S_OK;
    
    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	// obliczamy 
	// TODO: CMovingSprite::FrameMove?
	vOldPosition = vPosition;
	vPosition.x += (FLOAT)dims2.lX * 1.5f / RES_X;

	vPosition.x = max( vPosition.x, BOARD_L + vSize.x/2 );
	vPosition.x = min( vPosition.x, BOARD_R - vSize.x/2 );

	// przesuwamy z³apane pi³ki razem z desk¹
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++) {
		(*iBall)->vPosition.x += vPosition.x - vOldPosition.x;
	}

	// startujemy pi³ki
	if (dims2.rgbButtons[0]) {
		for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
			LaunchBall( *iBall, 0.75f );
		listCatchedBalls.clear();
	}

	// ustawiamy b³yskawice
	if ( s_pGameStates[ CBonus::MagneticPaddle ] || !listCatchedBalls.empty() ) {
		pLightningPaddle->vPosition.x = vPosition.x;
		if ( fTimeToLightningChange < 0 ) {
			if (rand()%2) pLightningPaddle->vScaling.x *= -1;
			if (rand()%2) pLightningPaddle->vScaling.y *= -1;
			fTimeToLightningChange = 0.05f;
		}
		fTimeToLightningChange -= fElapsedTime;
	}

	return S_OK;
}


void CPaddle::Render( LPD3DXSPRITE pSprite ) const
{
	CSprite::Render( pSprite );

	if ( s_pGameStates[CBonus::MagneticPaddle] || !listCatchedBalls.empty() )
		pLightningPaddle->Render( pSprite );
}


void CPaddle::CatchBall( CBall* pBall )
{
	pBall->vPosition.x = min(pBall->vPosition.x, vPosition.x + vSize.x/3);
	pBall->vPosition.x = max(pBall->vPosition.x, vPosition.x - vSize.x/3);
	pBall->vSpeed = D3DXVECTOR2(0,0);
	listCatchedBalls.push_back( pBall );
}

void CPaddle::LaunchBall( CBall* pBall, FLOAT speed )
{
	D3DXVECTOR2 vDirection = pBall->vPosition - D3DXVECTOR2(vPosition.x, BOARD_B );
	D3DXVec2Normalize( &vDirection, &vDirection );
	pBall->vSpeed = vDirection * speed;
}

void CPaddle::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide ) 
{
	if (vSide.y)
		pBall->vPosition.y = vPosition.y - vSize.y/2 - pBall->vSize.y/2;

	if (vSide.y && s_pGameStates[ CBonus::MagneticPaddle ])
		CatchBall( pBall );
	else {
		LaunchBall( pBall, D3DXVec2Length( &pBall->vSpeed ) );
		pBall->StrikeSparkles( vSide );
	}
}

void CPaddle::BonusHits( CBonus* pBonus, const D3DXVECTOR2 & vSide )
{
	s_pGameStates[pBonus->eType] = true;
	pBonus->bDeleteMe = true;
}

