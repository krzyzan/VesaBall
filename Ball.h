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
	CBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBall();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void StrikeSparkles( const D3DXVECTOR2 & vSize );

	static void PrepareEnvironment( BOOL* pGameStates, list<CSprite*>* pListObst, list<CSprite*>* pListRender, 
		list<CMovingSprite*>* pListFrameMove, LPDIRECT3DTEXTURE8 pSparkTexture);

protected:
	static list<CSprite*>*			s_pListObst;
	static list<CMovingSprite*>*	s_pListFrameMove;
	static list<CSprite*>*			s_pListRender;
	static LPDIRECT3DTEXTURE8		s_pSparkTexture;
	static BOOL*					s_pGameStates;
};
