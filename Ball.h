// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include "Sprite.h"

using namespace std;

class CBall : public CSprite  
{
public:
	CBall( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, D3DXVECTOR2 iDirection, list<CSprite*>* pListRender, LPDIRECT3DTEXTURE8 iSparkTexture );
	virtual ~CBall();

	void FrameMove( FLOAT fElapsedTime );
	void Bounce( CSprite* pDeck );

protected:
	D3DXVECTOR2	vDirection;
	D3DXVECTOR2	vOldPosition;

	list<CSprite*>* pListRender;
	LPDIRECT3DTEXTURE8 pSparkTexture;
};
