#pragma once

#include "D3DAppStage.h"
#include "Paddle.h"		//TMP
#include "Brick.h"		//TMP
#include "Ball.h"

class CLevel : public CD3DAppStage
{
public:
	CLevel( LPDIRECT3DDEVICE8 pd3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CLevel();

	HRESULT InitDeviceObjects();
	HRESULT FrameMove( FLOAT fElapsedTime );
	HRESULT Render();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	LPDIRECT3DTEXTURE8		pTex[256];
	LPD3DXSPRITE			pSprite;

	list<CMovingSprite*>	listFrameMove;
	list<CBall*>			listBall;

	BOOL bThruBrick;
};
