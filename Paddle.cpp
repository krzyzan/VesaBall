// Paddle.cpp: implementation of the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Paddle.h"
#include "Ball.h"


CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECTINPUTDEVICE8 DIDevice )
	: CMovingSprite( Texture, D3DXVECTOR2(1.0f/8, 1.0f/64), 0, 
		D3DXVECTOR2(0.5f, 0.75f-1.0f/64), D3DXVECTOR2(0, 0), 
		D3DXVECTOR2(0, 0), 0xFFFFFFFF)
{
	pLightningPaddle = new CSprite( LightningTex, D3DXVECTOR2( vSize.x, vSize.y*2), fRotation, vPosition - D3DXVECTOR2(0,vSize.y/2), dwBlending );
	pLightningBall = new CSprite( LightningTex, vSize, fRotation, vPosition - D3DXVECTOR2(0,vSize.y), dwBlending );
	pDIDevice = DIDevice;
}

CPaddle::~CPaddle()
{
	delete pLightningBall;
	delete pLightningPaddle;
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
	FLOAT fHorizMovement = (FLOAT)dims2.lX * 1.5f / RES_X;
	if (vPosition.x + fHorizMovement < 0.0f + vSize.x/2) fHorizMovement = 0.0f + vSize.x/2 - vPosition.x;
	if (vPosition.x + fHorizMovement > 1.0f - vSize.x/2) fHorizMovement = 1.0f - vSize.x/2 - vPosition.x;
	vPosition.x	+= fHorizMovement;

	// przesuwamy z³apane pi³ki razem z desk¹
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++) {
		(*iBall)->vPosition.x += fHorizMovement;
	}

	// startujemy pi³ki
	if (dims2.rgbButtons[0]) {
		for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
			LaunchBall( *iBall, 0.75f );
		listCatchedBalls.clear();
	}

	// ustawiamy b³yskawice
	if ( listCatchedBalls.empty() == false ) {
		pLightningPaddle->vPosition.x = vPosition.x;
		if ( fTimeToLightningChange < 0 ) {
			fTimeToLightningChange = 0.05f;
			if (rand()%2) pLightningPaddle->vScaling.x *= -1;
			if (rand()%2) pLightningPaddle->vScaling.y *= -1;
		}

		fTimeToLightningChange -= fElapsedTime;
	}

	return S_OK;
}


void CPaddle::Render( LPD3DXSPRITE pSprite ) const
{
	CSprite::Render( pSprite );

	if ( listCatchedBalls.empty() == false ) {
		pLightningPaddle->Render( pSprite );

		// b³yskawice na kulkach :)
		/*
		list<CBall*>::const_iterator iBall;
		for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++) {
			pLightningBall->vPosition = (*iBall)->vPosition;
			pLightningBall->SetSize( (*iBall)->vSize );
			pLightningBall->Render( pSprite );
		}
		*/
	}
}


void CPaddle::CatchBall( CBall* pBall )
{
	//TODO: Minimize, Maximize ¿eby pi³ka by³a na desce :)
	pBall->vSpeed = D3DXVECTOR2(0,0);
	listCatchedBalls.push_back( pBall );
}

void CPaddle::LaunchBall( CBall* pBall, FLOAT speed )
{
	D3DXVECTOR2 vDirection = pBall->vPosition - D3DXVECTOR2(vPosition.x, 0.75f);
	D3DXVec2Normalize( &vDirection, &vDirection );
	pBall->vSpeed = vDirection * speed;
}

void CPaddle::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide ) 
{
	if (vSide.y)
		pBall->vPosition.y = vPosition.y - vSize.y/2 - pBall->vSize.y/2;

	if (vSide.y && true)
		CatchBall( pBall );
	else {
		LaunchBall( pBall, D3DXVec2Length( &pBall->vSpeed ) );
		pBall->StrikeSparkles( vSide );
	}
}
