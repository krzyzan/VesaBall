// Paddle.cpp: implementation of the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Paddle.h"
#include "Ball.h"


CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice )
 : CMovingSprite( Texture, D3DXVECTOR2(1.0f/8, 1.0f/64), 0, 
		D3DXVECTOR2(0.5f, 0.75f-1.0f/64), D3DXVECTOR2(0, 0), 
		D3DXVECTOR2(0, 0), 0xFFFFFFFF)
{
	pDIDevice = DIDevice;
}

CPaddle::~CPaddle()
{

}

HRESULT CPaddle::FrameMove( FLOAT fElapsedTime )
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) 
		return S_OK;

    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	vPosition.x	+= (FLOAT)dims2.lX * 1.5f / RES_X;

	if (vPosition.x < 0.0f + vSize.x/2) vPosition.x = 0.0f + vSize.x/2;
	if (vPosition.x > 1.0f - vSize.x/2) vPosition.x = 1.0f - vSize.x/2;

	return S_OK;
}

void CPaddle::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide ) 
{
	FLOAT speed = D3DXVec2Length( &pBall->vSpeed );
	pBall->vSpeed = pBall->vPosition - D3DXVECTOR2(vPosition.x, 0.75f);
	pBall->vSpeed *= speed / D3DXVec2Length( &pBall->vSpeed );
	pBall->StrikeSparkles( vSide );

	//TODO: TMP pi³ka wychodzi z deski pod innym k¹tem ni¿ wchodzi
	// co powoduje tworzenie siê du¿ej iloœci iskier przy odbiciu na brzegach
	// z tego powodu ustawiamy pi³kê na powierzchni deski 
	if (vSide.y)
		pBall->vPosition.y = vPosition.y - vSize.y/2 - pBall->vSize.y/2;			
}
