// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MovingSprite.h"

#include <list>
using namespace std;

class CBall : public CMovingSprite  
{
public:
	CBall( LPDIRECT3DTEXTURE8 Texture, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, 
		list<CSprite*>*	ListRender,	list<CMovingSprite*>* ListFrameMove,
		LPDIRECT3DTEXTURE8 SparkTexture );
	virtual ~CBall();

	void FrameMove( FLOAT fElapsedTime );
	void ThrowSparkles( const D3DXVECTOR2 & vPositionFromCenter );

protected:
	list<CSprite*>*			pListRender;
	list<CMovingSprite*>*	pListFrameMove;
	LPDIRECT3DTEXTURE8		pSparkTexture;
};
