// Paddle.cpp: implementation of the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Paddle.h"
#include "Level.h"
#include "Ball.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice)
 : CMovingSprite( Texture, D3DXVECTOR2(1.0f/8, 1.0f/64), 0, 
		D3DXVECTOR2(0.5f, 0.75f-1.0f/64), D3DXVECTOR2(0, 0), 
		D3DXVECTOR2(0, 0), 0xFFFFFFFF)
{
	pDIDevice = DIDevice;
}

CPaddle::~CPaddle()
{

}

void CPaddle::FrameMove( FLOAT fElapsedTime )
{
	DIMOUSESTATE2 dims2;

    if ( NULL == pDIDevice ) return;
    ZeroMemory( &dims2, sizeof(dims2) );
  	if ( NULL == pDIDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 ))
		pDIDevice->Acquire();

	vPosition.x	+= (FLOAT)dims2.lX * 1.5f / RES_X;

	if (vPosition.x < 0.0f + vSize.x/2) vPosition.x = 0.0f + vSize.x/2;
	if (vPosition.x > 1.0f - vSize.x/2) vPosition.x = 1.0f - vSize.x/2;

}

void CPaddle::Collide( list<CBall*>* pListBall, BOOL & bThruBrick ) 
{
	list<CBall*>::iterator iBall;
	for (iBall = pListBall->begin(); iBall != pListBall->end(); iBall++) {
		CBall* pBall = (*iBall);

		if (fabs(pBall->vOldPosition.x - vPosition.x) < pBall->vSize.x/2 + vSize.x/2 &&
			fabs(pBall->vPosition.y - vPosition.y) < pBall->vSize.y/2 + vSize.y/2 )
		{
			bThruBrick = !bThruBrick;			//TMP
			pBall->vPosition.y = vPosition.y - vSize.y/2 - pBall->vSize.y/2;			//TMP

			FLOAT speed = D3DXVec2Length( &pBall->vSpeed );
			pBall->vSpeed = pBall->vPosition - D3DXVECTOR2(vPosition.x, 0.75f);
			pBall->vSpeed *= speed / D3DXVec2Length( &pBall->vSpeed );

			if (pBall->vSpeed.y > 0)
				pBall->ThrowSparkles( D3DXVECTOR2( 0,  pBall->vSize.y/2 ) );
			else
				pBall->ThrowSparkles( D3DXVECTOR2( 0, -pBall->vSize.y/2 ) );
		}

		if (fabs(pBall->vOldPosition.y - vPosition.y) < pBall->vSize.y/2 + vSize.y/2 &&
			fabs(pBall->vPosition.x - vPosition.x) < pBall->vSize.x/2 + vSize.x/2 )
		{
			bThruBrick = !bThruBrick;

			FLOAT speed = D3DXVec2Length( &pBall->vSpeed );
			pBall->vSpeed = pBall->vPosition /*- D3DXVECTOR2(vPosition.x, 0.75f)*/;
			pBall->vSpeed *= speed / D3DXVec2Length( &pBall->vSpeed );

			if (pBall->vSpeed.x > 0)
				pBall->ThrowSparkles( D3DXVECTOR2(  pBall->vSize.x/2, 0 ) );
			else
				pBall->ThrowSparkles( D3DXVECTOR2( -pBall->vSize.x/2, 0 ) );
			//pBall->bDeleteMe=true;
		}
	}
}
