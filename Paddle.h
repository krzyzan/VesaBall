// Paddle.h: interface for the CPaddle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

using namespace std;
#include <list>
#include <dinput.h>
#include "MovingSprite.h"

class CBall;

//TODO: jeœli przytrzymujê klawisz b³yskawica znika

class CPaddle : 
	public CSprite  
{
public:
	CPaddle();
	virtual ~CPaddle();

	void Render( LPD3DXSPRITE pSprite ) const;
	HRESULT MouseMove(DIMOUSESTATE2* dims2 );
	
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
