#pragma once

#include "GameBoard.h"
#include "timer.h"

class CBall;
class CMovingSprite;
class CEffectSprite;
class CPaddle;
class CBonus;
class CCounter;


const DWORD INITIAL_LIVES = 2;

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
	void MoveObjects( FLOAT fElapsedTime );
	void CollideObjects();
	void DestroyObjects();

	void ApplyBonus( DWORD Type );
	void ResetBoard();
	void KillPaddle();

	void CollideBallPaddle( CBall* pBall );

	list<CBall*>			listBall;
	list<CBonus*>			listBonus;
	list<CEffectSprite*>	listEffect;
	CCounter*				pScoreCounter; 
	CCounter*				pLivesCounter; 
	CPaddle*				pPaddle;

	LPDIRECT3DTEXTURE8		pDigitsTex;

	HWND					hWnd;					//TODO: TMP
	CTimer					timerRenderLimiter;
	FLOAT					fTimeToRender;
	DWORD					numFrameMove, numRender;

	bool					bThruBrick;
	bool					bFallingBricks;

	float					fGameSpeed;				//TODO: TMP
};
