// D3DBallApp.h: interface for the CD3DBallApp class.
// v0.45
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
		i wszystkie obiekty dostaj¹ wspólny czas z timerRenderLimiter
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

v0.20
	- Poprawione tekstury
	- Przesk³adanie kodu

v0.30
	- Nowa klasa bazowa CD3DAppScene
	- Klasa CLevel
	- Klasa CGameMenu

v0.40
	- Klasa CMenuItem, CCursor
	- Menu gry
	- Przepisanie kodu obliczaj¹cego zderzenia obiektów (potrzebne do klasy CBonus)
	- Start kulki z deski
	- £apanie kulek za pomoc¹ deski

v0.45
	- CMenuItem: wciskanie, cienie
	- Poprawione tekstury
	- Wy³adowania elektryczne przy ³apaniu kulek
	- Czyszczenie kodu

    	
ToDo:
	- zrobic GameOver oraz wyjœcie do menu z levelu
	- zrobic ³adowanie levelów z pliku
	- zrobic boczne œciany
	- zrobic œciane górn¹ oraz licznik punktów
	- listBonusObst zamieniæ na CPaddle*
	- Wywaliæ niepotrzebne argumenty z konstruktorów
	- Zrobiæ porzadek z destruktorami ( wykasowaæ niepotrzebne, wszystkie musz¹ byæ virtual )
	- Rzeczy niezwi¹zane z ruchem (np. blending, efekty) wrzucic do Render
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

#include <d3dx8.h>
#include "D3DApp.h"

class CD3DAppScene;


class CD3DBallApp :
	public CD3DApp  
{
public:
	CD3DBallApp();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT RenderLoop();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT	FinalCleanup();

protected:
	CD3DAppScene*	pScene;
};
