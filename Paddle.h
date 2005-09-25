#ifndef PADDLE_H
#define PADDLE_H

#include <list>
using namespace std;

#include "SpriteMoving.h"

class CBall;

    //! Deska
    /*!
		Przed utworzeniem obiektów klasy konieczne jest wczytanie tekstury
		pod adres #spTexture, #spLightningTexture.
    */ 
class CPaddle : 
	public CSprite  
{
public:
		//! Konstruktor
		/*!
			Tworzy deskê na œrodku dolnej krawedzi ekranu u¿ywaj¹c tekstury #spTexture.
		*/ 
	CPaddle();

	virtual ~CPaddle();

		//! Renderuje deskê
		/*!
			Jesli z³apano kulki renderuje te¿ efekt b³yskawicy.
			\param pSprite Obiekt \b ID3DXSprite u¿ywany do renderowania.
		*/
	void Render( LPD3DXSPRITE pSprite ) const;

		//! Przesuwa deskê w poziomie
		/*!
			Funkcja dba o to aby deska nie opuœci³a ekranu.
			Jeœli z³apano kulki przesuwa je.
			\param fHorizMovement Wartoœæ przesuniecia w poziomie
		*/
	void Move( float fHorizMovement );

		//! £apie kulkê
		/*!
			Jeœli kulka jest ju¿ z³apana, nie robi nic.
			\param pBall	WskaŸnik do ³apanej kulki
		*/
	void CatchBall( CBall* pBall );

		//! Ustawia wektor prêdkoœci kulki
		/*!
			Kierunek jest równoleg³y do prostej poprowadzonej przez œrodek kulki
			i punkt poni¿ej œrodka deski. Wartoœæ prêdkoœci jest nieznacznie zwiêkszana.
			\param pBall	WskaŸnik do kulki
		*/
	void SetBallSpeed( CBall* pBall ) const;

		//! Startuje z³apane kulki
	void LaunchCatchedBalls();

		//! Ustawia szerokoœæ deski
		/*!
			Funkcja dba o to aby nowa szerokoœæ zawiera³a siê w przedziale <PADDLE_WIDTH_MIN, PADDLE_WIDTH_MAX>
			Odleg³oœci pomiêdzy z³apanymi kulkami s¹ zmieniane proporcjonalnie.
			\param fNewWidth	Wartoœæ szerokoœci
		*/
	void SetWidth( float fNewWidth );

		//! Okreœla czy deska "³apie" kulki
	bool bGrabPaddle;

	static LPDIRECT3DTEXTURE8 spTexture;
	static LPDIRECT3DTEXTURE8 spLightningTexture;

private:

	list<CBall*> listCatchedBalls;
	CSprite* pLightning;
};

#endif
