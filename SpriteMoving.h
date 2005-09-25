#ifndef SPRITE_MOVING_H
#define SPRITE_MOVING_H

#include "Sprite.h"

//! Ruchomy obiekt
class CSpriteMoving :
	public CSprite
{
public:
	CSpriteMoving( LPDIRECT3DTEXTURE8 Texture, const D3DXVECTOR2 & Size, float Rotation, 
		const D3DXVECTOR2 & Position, const D3DXVECTOR2 & Speed, const D3DXVECTOR2 & Accel, D3DCOLOR Color );
	
	virtual ~CSpriteMoving(void);

		/*!
			Porusza obiektem wed³ug równania ruchu: r = a*t^2 + v*t + p0
			Odbija obiekt od œcian bocznych oraz sufitu.
		*/
	virtual void FrameMove( float fElapsedTime );

		//! Zwraca \b true jeœli obiekt jest poni¿ej dolnej krawêdzi ekranu i powinien zostaæ skasowany
	virtual bool HasExpired() const;

		//! Ustawia prêdkoœæ obiektu
		/*!
			/param Speed	Nowa prêdkoœæ
		*/
	void CSpriteMoving::SetSpeed( const D3DXVECTOR2 & Speed )
		{vSpeed = Speed;}
	const D3DXVECTOR2 & GetSpeed() const 
		{return vSpeed;}

		//! Zwraca punkt w którym nastêpuje kolizja z obiektem. 
		/*!
			Jeœli obiekty nie koliduja zwraca \b D3DXVECTOR(0,0)
		*/
	D3DXVECTOR2 GetCollisionSide( CSprite* pSprite ) const;

	D3DXVECTOR2	vOldPosition;

protected:
	D3DXVECTOR2	vSpeed;
	D3DXVECTOR2	vAccel;
};

#endif
