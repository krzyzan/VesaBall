#pragma once
#include "D3DScene.h"
#include "BrickArray.h"

	//! Sceneria gry
	/*!
		Ograniczenia boczne oraz tablica cegie³ek
	*/
class CGameBoard :
	public CD3DScene
{
protected:
	CGameBoard();
	virtual ~CGameBoard();

		//! Inicjalizuje obiekty w pamiêci systemowej
		/*!
			Wczytuje do pamiêci potrzebne tekstury. 
			Tworzy ograniczenia boczne i wczytuje tablicê cegie³ek z pliku.
		*/
	HRESULT InitDeviceObjects();

		//! Inicjalizuje obiekty w pamiêci karty graficznej
		/*!
			Tworzy obiekt \b ID3DXSprite u¿ywany do renderowania
		*/
	HRESULT RestoreDeviceObjects();

		//! Zwalnia obiekty w pamiêci karty graficznej
	HRESULT InvalidateDeviceObjects();

		//! Zwalnia obiekty w pamiêci systemowej
	HRESULT DeleteDeviceObjects();

		//! Koñczy scenê jeœli u¿ytkownik nacisn¹³ klawisz Escape
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );

		//! Renderuje sceneriê gry
	HRESULT FrameRender();

protected:
	LPD3DXSPRITE		pSprite;
	
	list<CSprite*>		listSprite;
	CBrickArray*		pBrickArray;
	DWORD				dwLevelNum;
};
