// D3DBallApp.h: interface for the CD3DBallApp class.
// v0.13
//
//////////////////////////////////////////////////////////////////////

/*
Changelog:

v0.10
	- Wersja poczatkowa (zrobiona u Laski)
v0.11
	- Dodana klasa CSprite
	- Dodana klasa CBall
	- Dodana klasa CDeck
v1.12
	- G³ówna tablica spritów jest teraz list¹ (mozna kasowaæ w œrodku)
	- CSprite::Draw() podzielone na Render i FrameMove()
	- CSprite'y s¹ usuwane z listy w CD3DBallApp::FrameMove() je¿eli bDeleteMe = TRUE;
	- CBall kasuje siê gdy wyleci za dolna krawêdŸ ekranu
	- Grafika przeniesiona do katalogu gfx
v1.13
	- kana³ alpha w teksturach
	- CSprite::vPosition jest na œrodku obiektu ( by³ w rogu )
	- Ruch kulek niezale¿ny od czasu

ToDo:
	- zrobic skalowanie do rozdzielczoœci
	- zrobic pozycje od 0.0f do 1.0f
	- Dok³adne odbicia kulki (na wektorach)
	- zrobiæ cegie³ki czyli CCell?
	- jak obliczamy odbicia - tablica cegie³ek czy sprawdzamy ka¿d¹ CBall z ka¿d¹ CCell?

ToDo:
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
	CTimer					RenderingTimer;

	FLOAT					numFrameMove;
	FLOAT					numRender;
};

