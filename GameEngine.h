#pragma once

#include "D3DAppScene.h"
#include "timer.h"

class CBall;
class CBrick;
class CMovingSprite;
class CPaddle;
class CBonus;
class CCounter;

/////////////////////

const int BRICK_X = 20;
const int BRICK_Y = 25;
const float BRICK_TABLE_H = 0.5f;

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

private:
	void AddBall( CBall* pBall );
	void AddBrick( CBrick* pBrick );
	void AddPaddle( CPaddle* Paddle );
	void AddBonus( CBonus* pBonus );

	void MoveObjects( FLOAT fElapsedTime );
	void CollideObjects();
	void RenderObjects();
	void DestroyObjects();

	void ApplyBonus( DWORD Type, bool Value );
	void CollideBallBrick( CBall* pBall, CBrick* pBrick );
	void CollideBallPaddle( CBall* pBall );
	void CatchBonus( CBonus* pBonus );
	
	void Reset();
	void LoadLevel( char* strFileName );
	void SaveLevel( char* strFileName );

	LPD3DXSPRITE			pSprite;

	list<CSprite*>			listRender;
	list<CMovingSprite*>	listFrameMove;
	list<CBall*>			listBall;
	CBrick*					pBrickTable[BRICK_X][BRICK_Y];
	list<CBonus*>			listBonus;
	CCounter*				pCounter; 
	CPaddle*				pPaddle;

	// TODO: poprawiæ tekstury scenerii
	LPDIRECT3DTEXTURE8*		pBonusTextures;
	LPDIRECT3DTEXTURE8		pSparkTex;
	LPDIRECT3DTEXTURE8		pStatusBarTex;
	LPDIRECT3DTEXTURE8		pWallTex;
	LPDIRECT3DTEXTURE8		pCounterTex;
	LPDIRECT3DTEXTURE8		pPaddleTex;
	LPDIRECT3DTEXTURE8		pBrickTex;
	LPDIRECT3DTEXTURE8		pLightningTex;
	LPDIRECT3DTEXTURE8		pBallTex;

	HWND					hWnd;					//TODO: TMP
	CTimer					timerRenderLimiter;
	FLOAT					fTimeToRender;
	DWORD					numFrameMove, numRender;

	bool					bThruBrick;
	bool					bFallingBricks;
};
