// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MovingObject.h"

#include <list>
using namespace std;

class CBall :
	public CMovingSprite  
{
public:
	CBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBall();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void StrikeSparkles( const D3DXVECTOR2 & vSize );

	static void PrepareEnvironment( list<CObject*>* pListRender, 
		list<CMovingSprite*>* pListFrameMove, LPDIRECT3DTEXTURE8 pSparkTexture);

protected:
	static list<CMovingSprite*>*	s_pListFrameMove;
	static list<CObject*>*			s_pListRender;
	static LPDIRECT3DTEXTURE8		s_pSparkTexture;
};
