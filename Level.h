#pragma once

#define BRICK_X		20
#define BRICK_Y		40

#include "D3DAppScene.h"
#include "timer.h"

class CBall;
class CMovingSprite;


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

	HRESULT AddBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, LPDIRECT3DTEXTURE8 SparkTexture );
	HRESULT AddBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	HRESULT AddPaddle( LPDIRECT3DTEXTURE8 Texture );
	
protected:
	LPD3DXSPRITE				pSprite;

	list<CMovingSprite*>		listFrameMove;
	list<CBall*>				listBall;
	list<CSprite*>				listBallObst;

	BOOL bThruBrick;

	HWND						hWnd;					//TODO: TMP
	CTimer						timerRenderLimiter;
	FLOAT						fTimeToRender;
	DWORD						numFrameMove, numRender;

private:
	HRESULT MoveObjects( FLOAT fElapsedTime );
	HRESULT RenderObjects();
	HRESULT DestroyObjects();
};
