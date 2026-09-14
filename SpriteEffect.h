#ifndef SPRITE_EFFECT_H
#define SPRITE_EFFECT_H

#include "SpriteMoving.h"

//! Vanishing-texture effect.
/*!
	Smoothly fades out a texture. When it finishes, #HasExpired() returns true.
*/
class CSpriteEffect :
	public CSpriteMoving
{
public:
		//! Constructor
		/*!
			\param Texture		Texture used for rendering.
			\param Size			Size
			\param Position		Initial position
			\param Speed		Initial speed
			\param Accel		Acceleration
			\param Color		The color and alpha channel are multiplied by this value. The value \c 0xFFFFFFFF preserves the original source color.
			\param Duration		Duration of the effect; after this time #HasExpired() returns \b true
		*/
	CSpriteEffect( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size,
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, 
		DWORD Color, float Duration );
	
	virtual ~CSpriteEffect();

		//! Effect movement
		/*!
			Movement as in #CSpriteMoving::FrameMove().
			Changes the alpha channel value depending on time.
		*/
	void FrameMove( float fElapsedTime );
		
		//! Returns \b true if the effect's duration has elapsed 
	bool HasExpired() const;

protected:
	float fRemaining;
	float fDuration;
};

#endif
