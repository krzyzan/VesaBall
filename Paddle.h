// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <dinput.h>
#include "MovingSprite.h"

class CPaddle : public CMovingSprite  
{
public:
	CPaddle::CPaddle( CLevel* Level, LPDIRECT3DTEXTURE8 Texture, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CPaddle();

	void FrameMove( FLOAT fElapsedTime );
	void Collide( list<CBall*>* pListBall );

private:
	LPDIRECTINPUTDEVICE8	pDIDevice;
};
