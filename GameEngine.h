#pragma once

#include "GameBoard.h"
#include "timer.h"

class CBall;
class CMovingSprite;
class CPaddle;
class CBonus;
class CCounter;


class CGameEngine :
	public CGameBoard
{
public:
	CGameEngine( /*TODO: TMP*/HWND wnd, LPDIRECT3DDEVICE8 pd3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	~CGameEngine();

	HRESULT InitDeviceObjects();
	HRESULT RenderLoop();
	HRESULT DeleteDeviceObjects();

private:
	void InsertPaddle( CPaddle* Paddle );

	void MoveObjects( FLOAT fElapsedTime );
	void CollideObjects();
	void DestroyObjects();

	void ApplyBonus( DWORD Type, bool Value );
	void CollideBallBrick( CBall* pBall, const POINT & pos );
	void CollideBallPaddle( CBall* pBall );
	
	list<CBall*>			listBall;
	list<CBonus*>			listBonus;
	list<CEffectSprite*>	listEffect;
	CCounter*				pCounter; 
	CPaddle*				pPaddle;

	LPDIRECT3DTEXTURE8*		pBonusTextures;
	LPDIRECT3DTEXTURE8		pSparkTex;
	LPDIRECT3DTEXTURE8		pCounterTex;
	LPDIRECT3DTEXTURE8		pPaddleTex;
	LPDIRECT3DTEXTURE8		pLightningTex;
	LPDIRECT3DTEXTURE8		pBallTex;

	HWND					hWnd;					//TODO: TMP
	CTimer					timerRenderLimiter;
	FLOAT					fTimeToRender;
	DWORD					numFrameMove, numRender;

	bool					bThruBrick;
	bool					bFallingBricks;

	float					fGameSpeed;				//TODO: TMP
};
