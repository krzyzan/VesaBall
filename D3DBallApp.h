// D3DBallApp.h: interface for the CD3DBallApp class.
// v0.12
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
	- CBall siê kasuje gdy wyleci za dolna krawêŸ ekranu
	- Grafika przeniesiona do katalogu gfx

ToDo:
	- zrobiæ cegie³ki czyli CCell?
	- jak obliczamy odbicia - tablica cegie³ek czy sprawdzamy ka¿d¹ CBall z ka¿d¹ CCell?
	- wyczyœciæ wspó³rzêdne za pomoc¹ ResX, ResY ( za pomoc¹ #DEFINE? )
	- zrobiæ kana³ alpha w teksturach
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

	HRESULT InitDeviceObjects();
	HRESULT FrameMove();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT Render();
	CD3DBallApp( HINSTANCE hInstance );
	virtual ~CD3DBallApp();

	LPDIRECT3DTEXTURE8		pTex[10];
	LPD3DXSPRITE			pSprite;
	list<CSprite*>			listSprite;
};

