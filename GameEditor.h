#pragma once
#include "GameBoard.h"

class CCursor;
class CCounter;

	//! Edytor poziomów
class CGameEditor :
	public CGameBoard
{
public:
	CGameEditor();
	virtual ~CGameEditor();

		//! Inicjalizuje obiekty w pamiêci systemowej
		/*!
			\copydoc CGameBoard::InitDeviceObjects()
			Tworzy kursor.
		*/
	HRESULT InitDeviceObjects();

		//! Zwalnia obiekty w pamiêci systemowej
	HRESULT DeleteDeviceObjects();

		//! Przetwarza zdarzenia myszki
		/*!
			Lewym przyciskiem u¿ytkownik tworzy cegie³ki lub zmienia ich typ.
			Prawym usuwa cegie³ki. 	Edytor zapamiêtuje typ ostatnio 
			zmodyfikowanej\stworzonej\usuniêtej cegie³ki.
		*/
            
	HRESULT ProcessMouseEvent( LPDIDEVICEOBJECTDATA didod );

		//! Przetwarza zdarzenia klawiatury
		/*!
			- Klawisz Escape wychodzi bez zapisania.
			- Klawisz Prawo przechodzi poziom wy¿ej
			- Klawisz Lewo przechodzi poziom wy¿ej
			- Klawisz S zapisuje poziom.
			- Klawisz L wczytuje poziom.
			- Klawisz C kasuje wszystke cegie³ki.
		*/
	HRESULT ProcessKeybrdEvent( LPDIDEVICEOBJECTDATA didod );
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();

protected:
	CCursor*	pCursor;
	DWORD		curType;
	CCounter*	pLevelCounter; 
};
