#include "stdafx.h"
#include "sparkeffect.h"

CSparkEffect::CSparkEffect( LPDIRECT3DTEXTURE8 iTexture, D3DXVECTOR2 iPosition, D3DXVECTOR2 iDirection, FLOAT iDuration  )
: CSprite( iTexture, D3DXVECTOR2(1.0f/256, 1.0f/256), 0, iPosition, 0xFFFFFFFF )
{
	vDirection = iDirection;
	fDuration = fRemaining = iDuration;
}

CSparkEffect::~CSparkEffect(void)
{
}

void CSparkEffect::FrameMove( FLOAT fElapsedTime )
{
	vPosition += vDirection*fElapsedTime;
	vDirection.y += 0.3f*fElapsedTime;

	dwBlending &= 0x00FFFFFF;
	dwBlending += ((DWORD)(fRemaining/fDuration * 0xFF) << 24);

	fRemaining -= fElapsedTime;
    if (fRemaining < 0) 
		bDeleteMe = TRUE;
}
