#pragma once

#include "GameBoard.h"
#include "timer.h"

class CBall;
class CMovingSprite;
class CEffectSprite;
class CPaddle;
class CBonus;
class CCounter;


class CGameEngine :
	public CGameBoard
{
public:
	CGameEngine( LPDIRECT3DDEVICE8 pD3DDevice );
	~CGameEngine();

	HRESULT InitDeviceObjects();
	HRESULT DeleteDeviceObjects();

	HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod );
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();
	CD3DAppScene* GetNextScene();

private:
	void MoveObjects( float fElapsedTime );
	void CollideObjects();

	void ApplyBonus( CBonus* pBonus );
	void ResetBoard();
	void KillPaddle();

	void CollideBallPaddle( CBall* pBall );
	void CollideBallBricks( CBall* pBall );
	void CreateSparkles( CBall* pBall, const D3DXVECTOR2 & vSide );

private:
	list<CBall*>			listBall;
	list<CBonus*>			listBonus;
	list<CEffectSprite*>	listEffect;
	CCounter*				pScoreCounter; 
	CCounter*				pLivesCounter; 
	CPaddle*				pPaddle;

	LPDIRECT3DTEXTURE8		pSparkTex;

	bool					bThruBrick;
	bool					bFallingBricks;

	float					fGameSpeed;				//TODO: TMP
};
