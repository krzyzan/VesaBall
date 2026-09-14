#include "StdAfx.h"
#include "SpriteAnimated.h"

CSpriteAnimated::CSpriteAnimated( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation,
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, 
		DWORD Color, float Duration, DWORD FirstFrame, DWORD LastFrame, const POINT & FramePixels )
	: CSpriteEffect( Texture, Size, Position, Speed, Accel, Color, Duration )
{
	fRotation = Rotation;

	dwFirstFrame = FirstFrame;
	dwLastFrame = LastFrame;
	ptFramePixels = FramePixels;

	D3DSURFACE_DESC sd;
	pTexture->GetLevelDesc(0, &sd);
	vScaling.y *= 8;
	vScaling.x *= 8;
}


CSpriteAnimated::~CSpriteAnimated()
{
}


void CSpriteAnimated::Render( LPD3DXSPRITE pSprite ) const
{
	D3DXVECTOR2 Position = (vPosition - vSize/2) * (float)RES_X;
	LONG lFrame = LONG((1.0f-fRemaining/fDuration)*(dwLastFrame-dwFirstFrame))+dwFirstFrame;
	CONST RECT SrcRect = {(lFrame/8)*ptFramePixels.x, (lFrame%8)*ptFramePixels.y, (lFrame/8+1)*ptFramePixels.x, (lFrame%8+1)*ptFramePixels.y };
	pSprite->Draw( pTexture, &SrcRect, &vScaling, &vRotationCenter, fRotation, &Position, dwColor );
}


void CSpriteAnimated::FrameMove( float fElapsedTime )
{
	fRemaining -= fElapsedTime;
}
