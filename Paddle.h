// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <dinput.h>
#include "MovingSprite.h"


class CPaddle : 
	public CMovingSprite  
{
public:
	CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CPaddle();

	HRESULT FrameMove( FLOAT fElapsedTime );
	
	void CPaddle::BallHits( CBall* pBall, const D3DXVECTOR2 & vSide );

private:
	LPDIRECTINPUTDEVICE8	pDIDevice;
};
