#pragma once
#include "MovingSprite.h"

class CCounter :
	public CMovingSprite
{
public:
	CCounter( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, 
		const D3DXVECTOR2 & Position, DWORD DigitHeight, DWORD NumDigits );
	virtual ~CCounter();

	void Render( LPD3DXSPRITE pSprite ) const;
	HRESULT CCounter::FrameMove( FLOAT fElapsedTime );

	void Reset();
	void Increment( DWORD Delta );

private:
	float* pRoller;
	DWORD* pRollerDest;

	DWORD dwDigitHeight;
	DWORD dwNumRollers;

	DWORD dwScore;
};
