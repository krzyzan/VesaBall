// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MovingSprite.h"

#include <list>
using namespace std;

class CEffectSprite;

class CBall :
	public CMovingSprite  
{
public:
	CBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBall();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void CreateSparkles( const D3DXVECTOR2 & vSize, list<CSprite*>* pListRender, list<CEffectSprite*>* pListEffect, LPDIRECT3DTEXTURE8 pSparkTexture );

	void MultiplySpeed( float fFactor );

	BOOL bCatched;
};
