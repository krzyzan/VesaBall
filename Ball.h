// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include "MovingSprite.h"

using namespace std;

class CBall : public CMovingSprite  
{
public:
	CBall( CLevel* Level, LPDIRECT3DTEXTURE8 Texture, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, 
		LPDIRECT3DTEXTURE8 SparkTexture );
	virtual ~CBall();

	void FrameMove( FLOAT fElapsedTime );
	void ThrowSparkles( const D3DXVECTOR2 & vPositionFromCenter );

protected:
	LPDIRECT3DTEXTURE8 pSparkTexture;
};
