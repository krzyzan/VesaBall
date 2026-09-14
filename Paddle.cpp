#include "StdAfx.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bonus.h"

const float BALL_ACCEL = 1.0f / 200;

SDL_Texture* CPaddle::spTexture;
SDL_Texture* CPaddle::spLightningTexture;

CPaddle::CPaddle()
	: CSprite(spTexture, Vec2(1.0f / 8, 1.0f / 64), 0,
			  Vec2(BOARD_L + BOARD_W / 2, BOARD_B - 1.0f / 64), 0xFFFFFFFF)
{
	pLightning = new CSprite(spLightningTexture, Vec2(vSize.x, vSize.y * 2), 0, vPosition - Vec2(0, vSize.y / 2), dwColor);
	bGrabPaddle = false;
}

CPaddle::~CPaddle()
{
	delete pLightning;
}

void CPaddle::Render(SDL_Renderer* pRenderer) const
{
	CSprite::Render(pRenderer);

	if (bGrabPaddle || !listCatchedBalls.empty())
	{
		if (rand() % 20 == 0)
			pLightning->FlipV();
		if (rand() % 20 == 0)
			pLightning->FlipH();
		pLightning->Render(pRenderer);
	}
}

void CPaddle::Move(float fHorizMovement)
{
	fHorizMovement = max(BOARD_L + vSize.x / 2 - vPosition.x, fHorizMovement);
	fHorizMovement = min(BOARD_R - vSize.x / 2 - vPosition.x, fHorizMovement);
	vPosition.x += fHorizMovement;
	pLightning->vPosition.x += fHorizMovement;

	// move the caught balls together with the paddle
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
	{
		(*iBall)->vPosition.x += fHorizMovement;
		(*iBall)->vOldPosition.y = (*iBall)->vPosition.y = vPosition.y - vSize.y / 2 - (*iBall)->vSize.y / 2 - 0.001f;
	}
}

void CPaddle::CatchBall(CBall* pBall)
{
	if (pBall->bCatched)
		return;

	pBall->vOldPosition.y = pBall->vPosition.y = vPosition.y - vSize.y / 2 - pBall->vSize.y / 2 - 0.001f;
	listCatchedBalls.push_back(pBall);
	pBall->bCatched = true;
}

void CPaddle::SetBallSpeed(CBall* pBall) const
{
	Vec2 vNormal = pBall->vPosition - Vec2(vPosition.x, vPosition.y + vSize.x / 4);
	Vec2Normalize(&vNormal, &vNormal);
	float fSpeedVal = Vec2Length(&pBall->GetSpeed()) + (BALL_SPEED_VAL_MAX - BALL_SPEED_VAL_MIN) * BALL_ACCEL;
	pBall->SetSpeed(vNormal * fSpeedVal);
}

void CPaddle::LaunchCatchedBalls()
{
	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
	{
		(*iBall)->bCatched = false;
	}
	listCatchedBalls.clear();
}

void CPaddle::SetWidth(float fNewWidth)
{
	fNewWidth = max(fNewWidth, PADDLE_WIDTH_MIN);
	fNewWidth = min(fNewWidth, PADDLE_WIDTH_MAX);

	list<CBall*>::iterator iBall;
	for (iBall = listCatchedBalls.begin(); iBall != listCatchedBalls.end(); iBall++)
		(*iBall)->vPosition.x = ((*iBall)->vPosition.x - vPosition.x) * fNewWidth / vSize.x + vPosition.x;

	SetSize(Vec2(fNewWidth, vSize.y));
	pLightning->SetSize(Vec2(fNewWidth, pLightning->vSize.y));
}
