#pragma once
#include "Object.h"


class CMovingSprite :
	public CObject
{
public:
	CMovingSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Blending );
	virtual ~CMovingSprite(void);

	D3DXVECTOR2	vOldPosition;
	D3DXVECTOR2	vSpeed;
	D3DXVECTOR2	vAccel;

	virtual HRESULT FrameMove( FLOAT fElapsedTime );

	D3DXVECTOR2 IsColliding( CObject* pSprite );		//TODO: zmieniæ nazwê
};
