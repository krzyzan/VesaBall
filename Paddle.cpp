// Paddle.cpp: implementation of the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"

LPDIRECT3DTEXTURE8 CPaddle::s_pTexture;
LPDIRECT3DTEXTURE8 CPaddle::s_pLightningTexture;

CPaddle::CPaddle()
	: CSprite( s_pTexture, D3DXVECTOR2( 1.0f/8, 1.0f/64 ), 0, 
		D3DXVECTOR2( BOARD_L+BOARD_W/2, BOARD_B-1.0f/64 ), 
		0xFFFFFFFF )
{
	bGrabPaddle = false;
	pLightning = new CSprite( s_pLightningTexture, D3DXVECTOR2(vSize.x, vSize.y*2), 0, vPosition - D3DXVECTOR2(0,vSize.y/2), dwBlending );
}


CPaddle::~CPaddle()
{
	delete pLightning;
}


HRESULT CPaddle::MouseMove( DIMOUSESTATE2* dims2 )
{
	// obliczamy 
	float fHorizMovement = (FLOAT)dims2->lX * 1.5f / RES_X;
	fHorizMovement = max( BOARD_L + vSize.x/2 - vPosition.x, fHorizMovement );
	fHorizMovement = min( BOARD_R - vSize.x/2 - vPosition.x, fHorizMovement );
	vPosition.x += fHorizMovement;
	pLightning->vPosition.x += fHorizMovement;

	// przesuwamy z³apane kulki razem z desk¹
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++) {
		(*iBall)->vPosition.x += fHorizMovement;
		(*iBall)->vOldPosition.y = (*iBall)->vPosition.y = vPosition.y - vSize.y/2 - (*iBall)->vSize.y/2 - 0.001f;	//TODO: hack :(
	}

	// startujemy kulki
	if ( dims2->rgbButtons[0] ) {
		LaunchAllBalls();
	}

	return S_OK;
}

void CPaddle::Render( LPD3DXSPRITE pSprite ) const
{
	CSprite::Render( pSprite );

	if ( bGrabPaddle || !listCatchedBalls.empty() ) {
		if (rand()%20 == 0) pLightning->vScaling.x *= -1;
		if (rand()%20 == 0) pLightning->vScaling.y *= -1;
		pLightning->Render( pSprite );
	}
}

//TODO: kulki nie moga by³ z³apane przez ten sam punkt deski

void CPaddle::CatchBall( CBall* pBall )
{
	if (pBall->bCatched)
		return;

	pBall->vPosition.x = max(pBall->vPosition.x, (vPosition.x - vSize.x/3)*frand(0.99f, 1.0f));
	pBall->vPosition.x = min(pBall->vPosition.x, (vPosition.x + vSize.x/3)*frand(0.99f, 1.0f));
	listCatchedBalls.push_back( pBall );
	pBall->bCatched = true;
}

void CPaddle::LaunchBall( CBall* pBall )
{
	D3DXVECTOR2 vDirection = pBall->vPosition - D3DXVECTOR2(vPosition.x, vPosition.y + vSize.x/16 );	//TODO: poprawiæ w zale¿noœci od rozmiaru deski
	D3DXVec2Normalize( &vDirection, &vDirection );
	pBall->vSpeed = vDirection * D3DXVec2Length( &pBall->vSpeed );
	pBall->bCatched = false;
}

void CPaddle::LaunchAllBalls()
{
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
		LaunchBall( *iBall );
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
