#pragma once
#include "Sprite.h"

class CMovingSprite :
	public CSprite
{
public:
	CMovingSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Blending );
	virtual ~CMovingSprite(void);

	virtual void FrameMove( float fElapsedTime );
	void SetSpeed( const D3DXVECTOR2 & vNewSpeed );
	const D3DXVECTOR2 & GetSpeed() const {return vSpeed;}

	bool IsColliding( CSprite* pSprite ) const;
	D3DXVECTOR2 GetCollisionSide( CSprite* pSprite ) const;

	D3DXVECTOR2	vOldPosition;

protected:
	D3DXVECTOR2	vSpeed;
	D3DXVECTOR2	vAccel;
};
