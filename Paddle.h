// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;
#include <list>
#include <dinput.h>
#include "MovingSprite.h"

class CBall;


class CPaddle : 
	public CSprite  
{
public:
	CPaddle::CPaddle( LPDIRECT3DTEXTURE8 Texture, LPDIRECT3DTEXTURE8 LightningTex );
	virtual ~CPaddle();

	void Render( LPD3DXSPRITE pSprite ) const;
	HRESULT MouseMove(DIMOUSESTATE2* dims2 );
	
	void LaunchBall( CBall* pBall );
	void CatchBall( CBall* pBall );

	void MultiplyWidth( float fFactor );

	BOOL bGrabPaddle;

private:
	list<CBall*> listCatchedBalls;
	CSprite* pLightning;
};
