#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include "SpriteEffect.h"

//! Animated sprite
/*!
	Handles displaying an animation. When it finishes, #HasExpired() returns true.
*/

class CSpriteAnimated : public CSpriteEffect
{
public:
	//! Constructor
	/*!
		\param Texture		Texture containing an 8x8 grid of frames used for rendering.
		\param Size			Size
		\param Rotation		Rotation in radians
		\param Position		Initial position
		\param Speed		Initial speed
		\param Accel		Acceleration
		\param Color		The color and alpha channel are multiplied by this value. The value 0xFFFFFFFF preserves the original source color.
		\param Duration		Duration; after this time #HasExpired() returns \b true
		\param FirstFrame	First frame of the animation
		\param LastFrame	Last frame of the animation
		\param FramePixels	Frame size within the texture, in pixels
	*/
	CSpriteAnimated(SDL_Texture* Texture, const Vec2 & Size, float Rotation,
					const Vec2 & Position, const Vec2 & Speed, const Vec2 & Accel, Color Color,
					float Duration, DWORD FirstFrame, DWORD LastFrame, const POINT & FramePixels);

	virtual ~CSpriteAnimated();

	//! Animation movement
	/*!
		Computes the current animation frame.
		\param fElapsedTime	Time since the last call
	*/
	void FrameMove(float fElapsedTime);

	//! Renders the animation
	/*!
		\param pRenderer The SDL renderer used for rendering.
	*/
	void Render(SDL_Renderer* pRenderer) const;

private:
	DWORD dwFirstFrame;
	DWORD dwLastFrame;
	POINT ptFramePixels;
};

#endif
