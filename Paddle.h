// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;
#include <list>
#include "MovingSprite.h"

class CBall;


class CPaddle : 
	public CSprite  
{
public:
	CPaddle();
	virtual ~CPaddle();

	void Render( LPD3DXSPRITE pSprite ) const;
	void Move( float fHorizMovement );
	
	void LaunchBall( CBall* pBall );
	void CatchBall( CBall* pBall );
	void LaunchAllBalls();

	void SetWidth( float fNewWidth );

	BOOL bGrabPaddle;

	static LPDIRECT3DTEXTURE8 s_pTexture;
	static LPDIRECT3DTEXTURE8 s_pLightningTexture;

private:
	list<CBall*> listCatchedBalls;
	CSprite* pLightning;
};
