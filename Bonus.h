#pragma once
#include "movingobject.h"

#include <list>
using namespace std;


class CBonus :
	public CMovingSprite
{
public:
	enum TYPE
	{
		GhostBall,
		MagneticPaddle,
		//FireBall,
		MAX_TYPE
	};

	CBonus( TYPE Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBonus();

	HRESULT FrameMove( FLOAT fElapsedTime );

	static void PrepareEnvironment( LPDIRECT3DTEXTURE8* pTexture);

	TYPE	eType;

protected:
	static LPDIRECT3DTEXTURE8*		s_pTextures;
};
