// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MovingSprite.h"

#include <list>
using namespace std;

class CBall :
	public CMovingSprite  
{
public:
	CBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, list<CSprite*>* ListObst, 
		list<CSprite*>* ListRender, list<CMovingSprite*>* ListFrameMove, LPDIRECT3DTEXTURE8 SparkTexture );
	virtual ~CBall();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void StrikeSparkles( const D3DXVECTOR2 & vPositionFromCenter );
	D3DXVECTOR2 IsColliding( CSprite* pSprite );

	list<CMovingSprite*>*	pListFrameMove;		//TODO: protected


protected:
	list<CSprite*>*			pListRender;
	list<CSprite*>*			pListObst;
	LPDIRECT3DTEXTURE8		pSparkTexture;
};
