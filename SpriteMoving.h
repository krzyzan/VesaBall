#ifndef SPRITE_MOVING_H
#define SPRITE_MOVING_H

#include "Sprite.h"

//! Moving object
class CSpriteMoving : public CSprite
{
public:
	CSpriteMoving(LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation,
				  const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Color);

	virtual ~CSpriteMoving(void);

	/*!
		Moves the object according to the equation of motion: r = a*t^2 + v*t + p0
		Bounces the object off the side walls and the ceiling.
	*/
	virtual void FrameMove(float fElapsedTime);

	//! Returns \b true if the object is below the bottom edge of the screen and should be removed
	virtual bool HasExpired() const;

	//! Sets the object's speed
	/*!
		/param Speed	New speed
	*/
	void CSpriteMoving::SetSpeed(const D3DXVECTOR2 & Speed)
	{
		vSpeed = Speed;
	}
	const D3DXVECTOR2 & GetSpeed() const
	{
		return vSpeed;
	}

	//! Returns the point where the collision with the object occurs.
	/*!
		If the objects do not collide, returns \b D3DXVECTOR(0,0)
	*/
	D3DXVECTOR2 GetCollisionSide(CSprite* pSprite) const;

	D3DXVECTOR2 vOldPosition;

protected:
	D3DXVECTOR2 vSpeed;
	D3DXVECTOR2 vAccel;
};

#endif
