#include "StdAfx.h"
#include "MovingSprite.h"


CMovingSprite::CMovingSprite( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Blending )
	: CSprite( Texture, Size, Rotation, Position, Blending )
{
	vOldPosition = vPosition;
	vSpeed = Speed;
	vAccel = Accel;
}

CMovingSprite::~CMovingSprite()
{
}

HRESULT CMovingSprite::FrameMove( FLOAT fElapsedTime )
{
	vOldPosition = vPosition;
	vSpeed += vAccel * fElapsedTime;
	vPosition += vSpeed * fElapsedTime;

	return S_OK;
}

D3DXVECTOR2 CMovingSprite::IsColliding( CSprite* pSprite )
{
	if (fabs(vOldPosition.x - pSprite->vPosition.x) < vSize.x/2 + pSprite->vSize.x/2 &&
			fabs(vPosition.y - pSprite->vPosition.y) < vSize.y/2 + pSprite->vSize.y/2 )
		return ( pSprite->vPosition.y - vPosition.y > 0) ? D3DXVECTOR2( 0, vSize.y/2 ) : D3DXVECTOR2( 0, -vSize.y/2 );

	if (fabs(vOldPosition.y - pSprite->vPosition.y) < vSize.y/2 + pSprite->vSize.y/2 &&
			fabs(vPosition.x - pSprite->vPosition.x) < vSize.x/2 + pSprite->vSize.x/2 )
		return ( pSprite->vPosition.x - vPosition.x > 0) ? D3DXVECTOR2( vSize.x/2, 0 ) : D3DXVECTOR2( -vSize.x/2, 0 );


	return D3DXVECTOR2(0,0);
}
