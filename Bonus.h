#pragma once
#include "MovingSprite.h"

#include <list>
using namespace std;


class CBonus :
	public CMovingSprite
{
public:
	enum TYPE
	{
		ThruBrick,
		SetOffExploding,
		FireBall,
		ShootingPaddle,
		GrabPaddle,

		ExtraLife,
		LevelWarp,
		ZapBricks,
		SlowBall,
		ExpandExploding,
	
		KillPaddle,
		ShrinkBall,
		FastBall,
		SuperShrink,
		FallingBricks,

		ExpandPaddle,
		ShrinkPaddle,
		SplitBall,
		MegaBall,
		EightBall,

		MAX_TYPE
	};

	CBonus( DWORD Type, const LPDIRECT3DTEXTURE8* pBonusTextures, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBonus();

	HRESULT FrameMove( FLOAT fElapsedTime );
	DWORD	dwType;

protected:
	static LPDIRECT3DTEXTURE8*		s_pTextures;
};
