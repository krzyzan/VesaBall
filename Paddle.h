// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;
#include <list>
#include <dinput.h>
#include "MovingObject.h"


class CBall;		//TODO: TMP?

class CPaddle : 
	public CMovingSprite  
{
public:
	CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CPaddle();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void Render( LPD3DXSPRITE pSprite ) const;
	
	void LaunchBall( CBall* pBall, FLOAT speed );		//TODO: protected?
	void CatchBall( CBall* pBall );						//TODO: protected?

private:
	LPDIRECTINPUTDEVICE8 pDIDevice;
	CObject* pPaddleLightning;
	FLOAT fTimeToLightningChange;

	list<CBall*> listCatchedBalls;
};
