// D3DBallApp.h: interface for the CD3DBallApp class.
// v0.18
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v0.10
	- Wersja poczatkowa dziedziczona od CD3DApp

v0.11
	- Dodana klasa CSprite
	- Dodana klasa CBall
	- Dodana klasa CPaddle

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
	- ZIKO: Ustawianie RotationCenter na œrodku sprita
	- ZIKO: Nowa grafika

v0.15
	- Na brzegach deska odbija kulki pod innym k¹tem
	- Obliczanie kolizji obiektów w osobnej pêtli (po wykonaniu ruchów)
	- Obs³uga "Precompiled Headers" i ogólne porzadki z plikami projektu :)
	- Dodana klasa CBrick
	- Iskry przy odbiciu
	- Poczatek kodu Game Over (na razie wy³¹czony)

v0.16
	- CSprite::CSprite pobiera teraz Size, a nie oblicza z rozmiarów tekstury
	- Wspólna klasa bazowa dla ruchomych sprite'ów: CMovingSprite

v0.17
	- Przystosowanie do nowej wersji CD3DApp, która poprawnie wykrywa 
		w³aœciwoœci kart graficznych => dzia³a na zabytkowych S3 VIRGE
	- Refaktoryzacja i optymalizacja kodu oraz ogólne porz¹dki
	- ZIKO: Znikaj¹ce cegie³ki
	- ZIKO: siê zmêczy³ :)
	- Zmiana nazwy klasy CSparkEffect na CEffectSprite
	- P³ynnie znikajace cegie³ki :)

v0.18
	- Poprawione sta³e w celu zwiêkszenia grywalnoœci
	- Klasa CLevel

ToDo:
	- Zrobic start pi³ki z deski
	- Zrobic start pi³ki z deski
	- Rzeczy niezwi¹zane z ruchem (np. blending efekty) wrzucic do Render
	- Zrobiæ Game Over
	- Co robiæ gdy wjedziemy bokiem deski w kulkê???
	- zrobiæ CBrickArray
	- zrobiæ obs³ugê dŸwiêku (np. kasuj¹c niepotrzebny kod z sampli do DSound )
	- Porz¹dek z protected, public, private
	- Przyspieszanie pi³ki przy odbiciu (a¿ do 1.0f ?)

ToDo:
	- Poœwiata/ogon dla fireballa
	- Gumowa deska
	- Deska, która losowo odbija 
*/


#pragma once

#include <list>			//TMP
#include "D3DApp.h"
#include "Level.h"

#include "Paddle.h"		//TMP
#include "Brick.h"		//TMP



using namespace std;

class CD3DBallApp : public CD3DApp  
{
public:
	CD3DBallApp();

	HRESULT LoadTexture( LPCTSTR nameTexture, DWORD numTex );
	HRESULT InitDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT	FinalCleanup();

	LPDIRECT3DTEXTURE8		pTex[256];
	LPD3DXSPRITE			pSprite;
	
	CTimer					timerFrameMove;
	FLOAT					fTimeToRender;

	DWORD					numFrameMove, numRender;

	CLevel*					pLevel;
};
