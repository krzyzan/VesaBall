#pragma once

#define BRICK_X		20
#define BRICK_Y		40

#include "D3DAppScene.h"
#include "timer.h"

class CBall;
class CBrick;
class CMovingSprite;
class CPaddle;

// Flagi gry
#define	GS_GHOSTBALL	0x00000001
#define	GS_FIREBALL		0x00000002
#define GS_MAGNETIC		0x00000004
//TODO: dokoñczyæ

class CLevel :
	public CD3DAppScene
{
public:
	CLevel( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 pd3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CLevel();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT RenderLoop();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	CBall*   AddBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, LPDIRECT3DTEXTURE8 SparkTexture );
	CBrick*  AddBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	CPaddle* AddPaddle( LPDIRECT3DTEXTURE8 PaddleTex, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECT3DTEXTURE8 BallTex, LPDIRECT3DTEXTURE8 SparkleTex );
	
protected:
	LPD3DXSPRITE			pSprite;

	list<CMovingSprite*>	listFrameMove;
	list<CSprite*>			listBallObst;

	BOOL bThruBrick;

	HWND					hWnd;					//TODO: TMP
	CTimer					timerRenderLimiter;
	FLOAT					fTimeToRender;
	DWORD					numFrameMove, numRender;

private:
	HRESULT MoveObjects( FLOAT fElapsedTime );
	HRESULT RenderObjects();
	HRESULT DestroyObjects();
};
