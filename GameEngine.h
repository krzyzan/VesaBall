#pragma once

#define BRICK_X		20
#define BRICK_Y		30

#include "D3DAppScene.h"
#include "timer.h"

class CBall;
class CBrick;
class CMovingSprite;
class CPaddle;
class CBonus;

class CGameEngine :
	public CD3DAppScene
{
public:
	CGameEngine( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 pd3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CGameEngine();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT RenderLoop();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	CBall*   AddBall( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed );
	CBrick*  AddBrick( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Size );
	CPaddle* AddPaddle( LPDIRECT3DTEXTURE8 PaddleTex, LPDIRECT3DTEXTURE8 LightningTex, LPDIRECT3DTEXTURE8 BallTex );
	
protected:
	LPD3DXSPRITE			pSprite;

	list<CMovingSprite*>	listFrameMove;

	CPaddle*				pPaddle;
	list<CBall*>			listBall;
	list<CBrick*>			listBrick;
	list<CBonus*>			listBonus;

	HWND					hWnd;					//TODO: TMP
	CTimer					timerRenderLimiter;
	FLOAT					fTimeToRender;
	DWORD					numFrameMove, numRender;
	
	BOOL*					pGameStates;
	LPDIRECT3DTEXTURE8*		pBonusTextures;


private:
	HRESULT MoveObjects( FLOAT fElapsedTime );
	HRESULT RenderObjects();
	HRESULT DestroyObjects();
};
