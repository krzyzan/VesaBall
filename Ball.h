// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprite.h"
#include "Deck.h"

class CBall : public CSprite  
{
public:
	CBall( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, CDeck* ipDeck );
	virtual ~CBall();

	void FrameMove();

protected:
	D3DXVECTOR2 vDirection;
	CDeck* pDeck;
};
