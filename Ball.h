// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MovingSprite.h"

#include <list>
using namespace std;

class CEffectSprite;


const float BALL_SIZE_MIN = 1.0f / 128;
const float BALL_SIZE_MAX = 1.0f / 56;
const float BALL_SIZE_AVG = (BALL_SIZE_MIN + BALL_SIZE_MAX) / 2;

const float BALL_SPEED_MIN = 0.20f;
const float BALL_SPEED_MAX = 0.80f;
const float BALL_SPEED_AVG = (BALL_SPEED_MIN + BALL_SPEED_MAX) / 2;

class CBall :
	public CMovingSprite  
{
public:
	CBall( const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	virtual ~CBall();

	HRESULT FrameMove( FLOAT fElapsedTime );
	void CreateSparkles( const D3DXVECTOR2 & vSize, list<CEffectSprite*>* pListEffect ) const;
	void MultiplySpeed( float fFactor );

	BOOL bCatched;

	static LPDIRECT3DTEXTURE8 s_pTexture;
	static LPDIRECT3DTEXTURE8 s_pSparkTexture;
};
