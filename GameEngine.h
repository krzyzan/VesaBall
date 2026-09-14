#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GameBoard.h"

class CBall;
class CSpriteMoving;
class CSpriteEffect;
class CSpriteAnimated;
class CPaddle;
class CBonus;
class CCounter;

	//! Engine gry VesaBall
class CGameEngine :
	public CGameBoard
{
public:
	CGameEngine();
	virtual ~CGameEngine();

		
		//! Inicjalizuje obiekty w pamiêci systemowej
		/*!
			\copydoc CGameBoard::OnInitDevice()
			\par
			£aduje tekstury:
			- deski
			- kulek
			- bonusów
			- liczników
			- efektów
			\par
			Tworzy:
			- licznik punktów 
			- licznik ¿yæ
			- deskê z kulk¹
		*/
	HRESULT OnInitDevice();

		//! Zwalnia obiekty w pamiêci systemowej
		/*!
			\copydoc CGameBoard::OnDeleteDevice()
			\par
			Zwalnia:
			- deskê
			- kulki
			- efekty
			- bonusy
			- pozycje eksplozji
		*/
	HRESULT OnDeleteDevice();

		//! Przetwarza zdarzenia z bufora myszki
		/*!
			\par
			Porusza desk¹. Lewy przycisk wypuszcza wszystkie kulki "z³apane" przez deskê.
		*/
	HRESULT OnMouseEvent( LPDIDEVICEOBJECTDATA didod );

		//! Przetwarza zdarzenia z bufora klawiatury
		/*!
			\par
			Klawisz P zatrzymuje grê.
		*/
	HRESULT OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod );
	
		//! Wykonuje ruch obiektów sceny oraz oblicza kolizje
		/*!
			\par
			Ruch obiektów sceny oraz kolizje.
			Kasuje obiekty które "zakoñczy³y siê".
			Jeœli nie ma ju¿ cegie³ek które mo¿na zebraæ, przechodzi do nastêpnego poziomu
			Jeœli spad³y wszystkie kulki, u¿ywa ¿ycie.
			Jeœli nie ma ¿yæ wychodzi z gry.
			Jeœli gra jest zatrzymana nie robi nic.
			\param fElapsedTime	Czas który up³yn¹³ od ostatniego wywo³ania funkcji
		*/
	HRESULT FrameMove( float fElapsedTime );

		//! Renderuje scenê
	HRESULT FrameRender();

private:
		//! Wykonuje ruch obiektów sceny
	void MoveObjects( float fElapsedTime );
		
		//! Oblicza kolizje miêdzy obiektami
	void CollideObjects();
		
		//! Oblicza kolizjê pomiêdzy kulk¹ a desk¹
		/*!
			\par
			Jeœli zebrano bonus deska "³apie" kulki.
			W przeciwnym wypadku odbija je. K¹t odbicia roœnie na brzegach deski.
			\param pBall	Kulka
		*/
	void CollideBallPaddle( CBall* pBall );
	
		//! Oblicza kolizje pomiêdzy kulk¹ a cegie³kami
		/*!
			\par
			Niszczy cegie³ki w któr¹ trafia kulka.
			Tworzy iskry przy odbiciu.
			Jeœli z³apano #CBonus::FireBall tworzy eksplozjê.
			Jeœli \b nie z³apano #CBonus::FireBall odbija kulkê od cegie³ek.
		*/
	void CollideBallBricks( CBall* pBall );
	
		//! Przygotowuje grê do startu
	void BoardPrepare();

		//! Czyœci obiekty
	void BoardClear();

		//! Tworzy deske i kulkê
	void BoardReset();

		//! Niszczy deskê, wypuszczajêc wszystkie z³apane kulki
	void KillPaddle();

		//! Eksplozja, która "zbija" cegie³ki wokó³ siebie.
		/*!
			\param pos	Pozycja cegie³ki w tablicy
		*/
	void DoExplosion( const POINT & pos );

		//! Uderzenie w cegie³kê
		/*!
			\par
			Usuwa cegie³kê z tablicy cegie³ek tworz¹c efekt znikania cegie³ki.
			Dodaje punkty za "zbicie" cegie³ki.
			Z pewnym prawdopodobieñstwem mo¿e pojawiæ siê bonus.
			Jeœli cegie³ka jest wybuchowa tworzy eksplozjê.
			\param pos		Pozycja cegie³ki w tablicy
			\param vSpeed	Prêdkoœæ "zbicia" i tworzonego bonusa
		*/
	void DestroyBrick( const POINT & pos, const D3DXVECTOR2 & vSpeed );

		//! W³¹cza efekty z³apanego bonusa
		/*!
			\param Type Typ Z³apanego bonusa
			\sa CBonus::TypeEnum
		*/
	void ApplyBonus( DWORD Type );

		//! Tworzy efekt iskier przy odbiciu kulki
	void CreateSparkles( CBall* pBall, const D3DXVECTOR2 & vSide );

		//! Tworzy "ogon" fireballa
	void CreateFireballTail( CBall* pBall );

		//! Kasuje ruchome obiekty które zakoñczy³y siê
		/*!
			\param pList WskaŸnik do listy obiektów z których kasujemy
			\sa CMovingSprite::HasExpired()
		*/
	void DeleteExpiredObjects( list<CSpriteMoving*> *pList );

private:
	list<CBall*>			listBall;
	list<CBonus*>			listBonus;
	list<CSpriteEffect*>	listEffect;
	list<CSpriteAnimated*>	listExplosion;
	CCounter*				pScoreCounter; 
	CCounter*				pLivesCounter; 
	CPaddle*				pPaddle;

	LPDIRECT3DTEXTURE8		pSparkTex;
	LPDIRECT3DTEXTURE8		pExplosionTex;

	bool					bThruBrick;
	bool					bFireBall;
	bool					bFallingBricks;

	bool					bPaused;

	float					fGameSpeed;
	float					bCheats;

	float					fTimeToBallTail;
	float					fTimeToExplosion;
	list<POINT>				listExplodingPos;
};

#endif
