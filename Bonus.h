#pragma once
#include "MovingSprite.h"


class CBonus :
	public CMovingSprite
{
public:
	enum EType
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

	CBonus( EType Type, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBonus();

	HRESULT FrameMove( FLOAT fElapsedTime );
	DWORD	dwType;

	static LPDIRECT3DTEXTURE8 s_pTextures[MAX_TYPE];
};
