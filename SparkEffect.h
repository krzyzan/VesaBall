#pragma once
#include "sprite.h"

class CSparkEffect : public CSprite
{
public:
	CSparkEffect( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, 
		D3DXVECTOR2 iDirection, FLOAT iDuration );
	virtual ~CSparkEffect(void);

	void FrameMove( FLOAT fElapsedTime );

protected:
	D3DXVECTOR2	vDirection;
	FLOAT fRemaining;
	FLOAT fDuration;
};

