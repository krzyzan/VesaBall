// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;
#include <list>
#include <dinput.h>
#include "MovingSprite.h"


class CPaddle : 
	public CMovingSprite  
{
public:
	CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CPaddle();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void Render( LPD3DXSPRITE pSprite ) const;
	
	void CPaddle::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide );

	void LaunchBall( CBall* pBall, FLOAT speed );		//TODO: protected?
	void CatchBall( CBall* pBall );						//TODO: protected?

private:
	LPDIRECTINPUTDEVICE8 pDIDevice;
	CSprite* pLightningPaddle;
	CSprite* pLightningBall;							//TODO?
	FLOAT fTimeToLightningChange;


	list<CBall*> listCatchedBalls;
};
