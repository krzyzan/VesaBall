#include "StdAfx.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"


const float BALL_ACCEL = 1.0f / 200;

LPDIRECT3DTEXTURE8 CPaddle::spTexture;
LPDIRECT3DTEXTURE8 CPaddle::spLightningTexture;


CPaddle::CPaddle()
	: CSprite( spTexture, D3DXVECTOR2( 1.0f/8, 1.0f/64 ), 0, 
		D3DXVECTOR2( BOARD_L+BOARD_W/2, BOARD_B-1.0f/64 ), 0xFFFFFFFF )
{
	pLightning = new CSprite( spLightningTexture, D3DXVECTOR2(vSize.x, vSize.y*2), 0, vPosition - D3DXVECTOR2(0,vSize.y/2), dwColor );
	bGrabPaddle = false;
}


CPaddle::~CPaddle()
{
	delete pLightning;
}


void CPaddle::Render( LPD3DXSPRITE pSprite ) const
{
	CSprite::Render( pSprite );

	if ( bGrabPaddle || !listCatchedBalls.empty() ) {
		if (rand()%20 == 0) pLightning->FlipV();
		if (rand()%20 == 0) pLightning->FlipH();
		pLightning->Render( pSprite );
	}
}


void CPaddle::Move( float fHorizMovement )
{
	fHorizMovement = max( BOARD_L + vSize.x/2 - vPosition.x, fHorizMovement );
	fHorizMovement = min( BOARD_R - vSize.x/2 - vPosition.x, fHorizMovement );
	vPosition.x += fHorizMovement;
	pLightning->vPosition.x += fHorizMovement;

	// przesuwamy z³apane kulki razem z desk¹
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++) {
		(*iBall)->vPosition.x += fHorizMovement;
		(*iBall)->vOldPosition.y = (*iBall)->vPosition.y = vPosition.y - vSize.y/2 - (*iBall)->vSize.y/2 - 0.001f;
	}
}


void CPaddle::CatchBall( CBall* pBall )
{
	if (pBall->bCatched)
		return;

	pBall->vOldPosition.y = pBall->vPosition.y = vPosition.y - vSize.y/2 - pBall->vSize.y/2 - 0.001f;
	listCatchedBalls.push_back( pBall );
	pBall->bCatched = true;
}


void CPaddle::SetBallSpeed( CBall* pBall ) const
{
	D3DXVECTOR2 vNormal = pBall->vPosition - D3DXVECTOR2(vPosition.x, vPosition.y + vSize.x/4 );
	D3DXVec2Normalize( &vNormal, &vNormal );
	float fSpeedVal = D3DXVec2Length( &pBall->GetSpeed() ) + (BALL_SPEED_VAL_MAX - BALL_SPEED_VAL_MIN) * BALL_ACCEL;
	pBall->SetSpeed( vNormal * fSpeedVal );
}


void CPaddle::LaunchCatchedBalls()
{
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++) {
		(*iBall)->bCatched = false;
	}
	listCatchedBalls.clear();
}


void CPaddle::SetWidth( float fNewWidth )
{
	fNewWidth = max( fNewWidth, PADDLE_WIDTH_MIN );
	fNewWidth = min( fNewWidth, PADDLE_WIDTH_MAX );

	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
		(*iBall)->vPosition.x = ((*iBall)->vPosition.x - vPosition.x) * fNewWidth/vSize.x + vPosition.x;
	
	SetSize( D3DXVECTOR2( fNewWidth, vSize.y) );
	pLightning->SetSize( D3DXVECTOR2( fNewWidth, pLightning->vSize.y) );
}
