#ifndef SPRITE_MOVING_H
#define SPRITE_MOVING_H

#include "Sprite.h"

//! Moving object
class CSpriteMoving : public CSprite
{
public:
	CSpriteMoving(SDL_Texture* Texture, const Vec2 & Size, float Rotation,
				  const Vec2 & Position, const Vec2 & Speed, const Vec2 & Accel, Color Color);

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
	void SetSpeed(const Vec2 & Speed)
	{
		vSpeed = Speed;
	}
	const Vec2 & GetSpeed() const
	{
		return vSpeed;
	}

	//! Returns the point where the collision with the object occurs.
	/*!
		If the objects do not collide, returns \b Vec2(0,0)
	*/
	Vec2 GetCollisionSide(CSprite* pSprite) const;

	Vec2 vOldPosition;

protected:
	Vec2 vSpeed;
	Vec2 vAccel;
};

#endif
