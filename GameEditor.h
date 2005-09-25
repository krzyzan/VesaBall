#ifndef GAMEEDITOR_H
#define GAMEEDITOR_H

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
			\copydoc CGameBoard::OnInitDevice()
			Tworzy kursor.
		*/
	HRESULT OnInitDevice();

		//! Zwalnia obiekty w pamiêci systemowej
	HRESULT OnDeleteDevice();

		//! Przetwarza zdarzenia myszki
		/*!
			Lewym przyciskiem u¿ytkownik tworzy cegie³ki.
			Prawym cegie³ki s¹ usuwane. Typ cegie³ki mozna okreœliæ 
			klikaj¹c lewym przyciskiem na dolnej tablicy.
		*/
            
	HRESULT OnMouseEvent( LPDIDEVICEOBJECTDATA didod );

		//! Przetwarza zdarzenia klawiatury
		/*!
			- Klawisz Escape wychodzi bez zapisania.
			- Klawisz Prawo przechodzi poziom wy¿ej
			- Klawisz Lewo przechodzi poziom ni¿ej
			- Klawisz S zapisuje poziom.
			- Klawisz L wczytuje poziom.
			- Klawisz C kasuje wszystke cegie³ki.
		*/
	HRESULT OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod );
	
	HRESULT FrameMove( float fElapsedTime );
	HRESULT FrameRender();

protected:
	CCursor*		pCursor;
	BYTE			curType;
	CCounter*		pLevelCounter; 

	CBrickArray*	pBrickToolkit;

	// stan przycisków myszy
	bool bLMB;
	bool bRMB;
};

#endif
