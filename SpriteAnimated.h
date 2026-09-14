#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include "SpriteEffect.h"

//! Animated sprite
/*!
	Handles displaying an animation. When it finishes, #HasExpired() returns true.
*/

class CSpriteAnimated :
	public CSpriteEffect
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
			\param Color		The color and alpha channel are multiplied by this value. The value \c 0xFFFFFFFF preserves the original source color.
			\param Duration		Duration; after this time #HasExpired() returns \b true
			\param FirstFrame	First frame of the animation
			\param LastFrame	Last frame of the animation
			\param FramePixels	Frame size within the texture, in pixels
		*/
	CSpriteAnimated( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size,  float Rotation,
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, DWORD Color, 
		float Duration, DWORD FirstFrame, DWORD LastFrame, const POINT & FramePixels );
	
	virtual ~CSpriteAnimated();

		//! Animation movement
		/*!
			Computes the current animation frame.
			\param fElapsedTime	Time since the last call
		*/ 
	void FrameMove( float fElapsedTime );

		//! Renders the animation
		/*!
			If balls are caught, also renders the lightning effect.
			\param pSprite The \b ID3DXSprite object used for rendering.
		*/
	void Render( LPD3DXSPRITE pSprite ) const;

private:
	DWORD dwFirstFrame;
	DWORD dwLastFrame;
	POINT ptFramePixels;
};

#endif
