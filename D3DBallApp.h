// D3DBallApp.h: interface for the CD3DBallApp class.
// v0.14
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v0.10
	- Wersja poczatkowa (zrobiona u Laski)
	- obs³uga Direct3D

v0.11
	- Dodana klasa CSprite
	- Dodana klasa CBall
	- Dodana klasa CDeck

v1.12
	- G³ówna tablica spritów jest teraz list¹ (mozna kasowaæ w œrodku)
	- CSprite::Draw() podzielone na Render() i FrameMove()
	- CSprite'y s¹ usuwane z listy w CD3DBallApp::FrameMove() je¿eli bDeleteMe = TRUE;
	- CBall kasuje siê gdy wyleci za dolna krawêdŸ ekranu
	- Grafika przeniesiona do katalogu gfx

v1.13
	- Kana³ alpha w teksturach
	- CSprite::vPosition jest na œrodku obiektu (by³ w rogu)
	- Ruch kulek niezale¿ny od czasu

v1.14
	- Skalowanie obiektów i t³a zale¿nie od rozdzielczoœci
	- CSprite::vPosition jest typu FLOAT od 0.0f do 1.0f (!)
	- dodany Reset() w konstruktorze CTimer, 
		bo inaczej nie dzia³a w konfiguracji Release u Laski (dziwne, nie???)
	- CSprite::FrameMove() jest teraz CSprite::FrameMove( FLOAT fElapsedTime )
		i wszystkie obiekty dostaj¹ wspólny czas z timerFrameMove
	- Poprawne obliczanie odbiæ
    
ToDo:
	- Co robiæ gdy wjedziemy bokiem deski w kulkê???
	- Przerzuciæ odbicia do osobnej funkcji ( virtual Collide?? )
	- zrobiæ cegie³ki czyli CCell
	- zrobiæ CCellArray
	- zrobiæ obs³ugê dŸwiêku (np. kasuj¹c niepotrzebny kod z sampli do DSound )

ToDo:
	- Iskry przy odbiciu
	- Gumowa deska
	- Deska co losowo odbija 
*/


#pragma once

#include <list>
#include "D3DApp.h"

#include "Deck.h"
#include "Ball.h"

using namespace std;

class CD3DBallApp : public CD3DApp  
{
public:
	HRESULT LoadTexture( LPCTSTR nameTexture, DWORD numTex );
	HRESULT InitDeviceObjects();
	HRESULT FrameMove();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT Render();
	CD3DBallApp( HINSTANCE hInstance );
	virtual ~CD3DBallApp();

	LPDIRECT3DTEXTURE8		pTex[256];
	LPD3DXSPRITE			pSprite;
	list<CSprite*>			listSprite;
	
	CTimer					timerFrameMove;
	CTimer					timerRender;

	FLOAT					numFrameMove;
	FLOAT					numRender;
};
