// D3DBallApp.h: interface for the CD3DBallApp class.
// v0.15
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v0.10
	- Wersja poczatkowa (zrobiona u ZIKO)

v0.11
	- Dodana klasa CSprite
	- Dodana klasa CBall
	- Dodana klasa CDeck

v0.12
	- G³ówna tablica spritów jest teraz list¹ (szybko kasuje w œrodku)
	- CSprite::Draw() podzielone na Render() i FrameMove()
	- CSprite'y s¹ usuwane z listy w CD3DBallApp::FrameMove() je¿eli bDeleteMe = TRUE;
	- CBall kasuje siê gdy wyleci za dolna krawêdŸ ekranu
	- Grafika przeniesiona do katalogu gfx

v0.13
	- Kana³ alpha w teksturach
	- CSprite::vPosition jest na œrodku obiektu (by³ w rogu)
	- Ruch kulek niezale¿ny od czasu

v0.14
	- Skalowanie obiektów i t³a zale¿nie od rozdzielczoœci
	- CSprite::vPosition jest typu FLOAT od 0.0f do 1.0f (!)
	- dodany Reset() w konstruktorze CTimer, 
		bo inaczej nie dzia³a w konfiguracji Release u Laski (dziwne, nie???)
	- CSprite::FrameMove() jest teraz CSprite::FrameMove( FLOAT fElapsedTime )
		i wszystkie obiekty dostaj¹ wspólny czas z timerFrameMove
	- Poprawne obliczanie odbiæ
	- ZIKO: Ustawianie RotationCenter na œrodku
	- ZIKO: Nowa grafika
v0.15
	- Na brzegach deska odbija kulki pod innym k¹tem
	- Obliczanie kolizji obiektów w osobnej pêtli (po wykonaniu ruchów)
	- Obs³uga "Precompiled Headers" i ogólne porzadki z plikami projektu :)
	- Dodana klasa CBrick
	- Iskry przy odbiciu
	- Poczatek kodu Game Over (na razie wy³¹czony)
v0.16
	- CSprite::CSprite pobiera teraz Size a nie Scaling =>
		ca³kowita niezale¿noœæ od rozdzielczoœci i rozmiarów tekstur
  
ToDo:
	- Zrobic wspólna klasê bazow¹ np. CMovingObject
	- CBall::Bounce przenieœæ do CDeck i do CBrick
	- Zrobiæ Game Over
	- CSprite::CSprite() pobiera vSize, a nie oblicza z tekstury 
	- Podzieliæ CSprite::FrameMove() na virtual CSprite::Move() i virtual CSprite::Bounce()
	- Co robiæ gdy wjedziemy bokiem deski w kulkê???
	- zrobiæ cegie³ki czyli CBrick
	- zrobiæ CBrickArray
	- zrobiæ obs³ugê dŸwiêku (np. kasuj¹c niepotrzebny kod z sampli do DSound )

ToDo:
	- Gumowa deska
	- Deska, która losowo odbija 
*/


#pragma once

#include <list>
#include "D3DApp.h"

#include "Deck.h"
#include "Ball.h"
#include "Brick.h"
#include "SparkEffect.h"	//TMP

using namespace std;

class CD3DBallApp : public CD3DApp  
{
public:
	CD3DBallApp();
	virtual ~CD3DBallApp();

	HRESULT LoadTexture( LPCTSTR nameTexture, DWORD numTex );
	HRESULT InitDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	LPDIRECT3DTEXTURE8		pTex[256];
	LPD3DXSPRITE			pSprite;
	list<CSprite*>			listRender;
	list<CBall*>			listBall;

	CSprite*				pBackground;
	
	CTimer					timerFrameMove;
	FLOAT					fTimeToRender;

	FLOAT					numFrameMove, numRender;
};
