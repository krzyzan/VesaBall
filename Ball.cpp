// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"

LPDIRECT3DTEXTURE8 CBall::s_pTexture;

CBall::CBall( const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed )
	: CMovingSprite( s_pTexture, BALL_SIZE_AVG, 0, 
		Position, Speed, D3DXVECTOR2(0, 0), 0xFFFFFFFF )
{
	bCatched	= false;
}


void CBall::FrameMove( float fElapsedTime )
{  
	if (bCatched)
		return;
		
	CMovingSprite::FrameMove( fElapsedTime );
} 


void CBall::Reflect( CBrick* pBrick, const D3DXVECTOR2 & vSide )
{
	if (vSide.y > 0) {
		vPosition.y = 2*pBrick->vPosition.y - vSize.y - vPosition.y - pBrick->vSize.y;
		vSpeed.y *= -1;
	}

	if (vSide.y < 0) {
		vPosition.y = 2*pBrick->vPosition.y + vSize.y - vPosition.y + pBrick->vSize.y;
		vSpeed.y *= -1;
	}

	if (vSide.x > 0) {
		vPosition.x = 2*pBrick->vPosition.x - vSize.x - vPosition.x - pBrick->vSize.x;
		vSpeed.x *= -1;
	}

	if (vSide.x < 0) {
		vPosition.x = 2*pBrick->vPosition.x + vSize.x - vPosition.x + pBrick->vSize.x;
		vSpeed.x *= -1;
	}
};


void CBall::SetSpeed( const D3DXVECTOR2 & vNewSpeed )
{
	float fNewSpeedVal = D3DXVec2Length( &vNewSpeed );
	fNewSpeedVal = min( fNewSpeedVal, BALL_SPEED_VAL_MAX );
	fNewSpeedVal = max( fNewSpeedVal, BALL_SPEED_VAL_MIN );
	D3DXVec2Normalize( &vSpeed, &vNewSpeed );
	vSpeed *= fNewSpeedVal;
}