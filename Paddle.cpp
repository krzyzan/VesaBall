// Paddle.cpp: implementation of the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"

CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECTINPUTDEVICE8 DIDevice )
	: CMovingSprite( Texture, D3DXVECTOR2(1.0f/8, 1.0f/64), 0, 
		D3DXVECTOR2(BOARD_L+BOARD_W/2, BOARD_B-1.0f/64), 
		D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), 0xFFFFFFFF)
{
	pPaddleLightning = new CObject( LightningTex, D3DXVECTOR2( vSize.x, vSize.y*2), fRotation, vPosition - D3DXVECTOR2(0,vSize.y/2), dwBlending );
	pDIDevice = DIDevice;
}


CPaddle::~CPaddle()
{
	delete pPaddleLightning;
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
	//if ( bMagnetic || !listCatchedBalls.empty() ) {
		pPaddleLightning->vPosition.x = vPosition.x;
		if ( fTimeToLightningChange < 0 ) {
			if (rand()%2) pPaddleLightning->vScaling.x *= -1;
			if (rand()%2) pPaddleLightning->vScaling.y *= -1;
			fTimeToLightningChange = 0.05f;
		}
		fTimeToLightningChange -= fElapsedTime;
	//}

	return S_OK;
}


void CPaddle::Render( LPD3DXSPRITE pSprite ) const
{
	CObject::Render( pSprite );

	if ( /*bMagnetic ||*/ !listCatchedBalls.empty() )
		pPaddleLightning->Render( pSprite );
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
