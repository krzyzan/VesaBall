#ifndef PADDLE_H
#define PADDLE_H

#include <list>
using namespace std;

#include "SpriteMoving.h"

class CBall;

//! Paddle
/*!
	Before creating instances of this class, the texture must be loaded
	into #spTexture, #spLightningTexture.
*/
class CPaddle : public CSprite
{
public:
	//! Constructor
	/*!
		Creates the paddle at the center of the bottom edge of the screen using the #spTexture texture.
	*/
	CPaddle();

	virtual ~CPaddle();

	//! Renders the paddle
	/*!
		If balls are caught, also renders the lightning effect.
		\param pSprite The \b ID3DXSprite object used for rendering.
	*/
	void Render(LPD3DXSPRITE pSprite) const;

	//! Moves the paddle horizontally
	/*!
		The function ensures the paddle does not leave the screen.
		If balls are caught, moves them too.
		\param fHorizMovement Horizontal displacement value
	*/
	void Move(float fHorizMovement);

	//! Catches a ball
	/*!
		If the ball is already caught, does nothing.
		\param pBall	Pointer to the ball being caught
	*/
	void CatchBall(CBall* pBall);

	//! Sets the ball's speed vector
	/*!
		The direction is parallel to the line through the ball's center
		and a point below the paddle's center. The speed value is slightly increased.
		\param pBall	Pointer to the ball
	*/
	void SetBallSpeed(CBall* pBall) const;

	//! Launches the caught balls
	void LaunchCatchedBalls();

	//! Sets the paddle's width
	/*!
		The function ensures the new width stays within the <PADDLE_WIDTH_MIN, PADDLE_WIDTH_MAX> range
		Distances between caught balls are scaled proportionally.
		\param fNewWidth	Width value
	*/
	void SetWidth(float fNewWidth);

	//! Determines whether the paddle "catches" balls
	bool bGrabPaddle;

	static LPDIRECT3DTEXTURE8 spTexture;
	static LPDIRECT3DTEXTURE8 spLightningTexture;

private:
	list<CBall*> listCatchedBalls;
	CSprite* pLightning;
};

#endif
