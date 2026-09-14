#ifndef D3DSCENE_H
#define D3DSCENE_H


#include <stack>
using namespace std;

#include <d3dx8.h>
#include <dinput.h>

	//! Scena
	/*!
		\par
		Klasa abstrakcyjna, opisuje pojedyncz¹ scenê - czêœæ programu stanowi¹c¹ logiczn¹ ca³oœæ, 
		jak np. g³ówne menu gry, ekran wyników, menu opcji itp. Stanowi "szkielet" dla klas które od niej dziedzicz¹.
		Jedynym interfejsem do sceny powinien byc konstruktor. 
		Wywo³ywaniem funkcji w odpowiedniej kolejnoœci oraz zakoñczeniem sceny zajmuje siê zaprzyjaŸniona klasa #CD3DApp.
		U³atwia to poprawne alokowanie obiektów w pamiêci karty graficznej, 
		które musza byæ zwalniane gdy urz¹dzenie karty jest resetowane 
		(np. u¿ytkownik zminimalizowa³ aplikacjê naciskaj¹c Alt-Tab). 
		\par
		Klasa zapewnia tak¿e proste zarz¹dzanie teksturami zwalniaj¹c je automatycznie, gdy scena siê koñczy.
	*/
class CD3DScene
{
public:
		//! Konstruktor
	CD3DScene();
		
protected:
		//! Destruktor
		/*!
			Automatycznie zwalnia wszystkie tekstury za³adowane przy pomocy funkcji #LoadTexture()
		*/
	virtual ~CD3DScene();

		//! Inicjalizuje obiekty u¿ywajace pamiêci systemowej
		/*!
			W tej funkcji klasa dziedziczona powinna zainicjowaæ wszystkie obiekty, 
			oprócz tych które znajduj¹ siê w pamiêci kart graficznej. W szczególnoœci tutaj
			nale¿y za³adowaæ potrzebne tekstury za pomoc¹ za pomoc¹ #LoadTexture().
			Jest to dozwolone poniewa¿ s¹ one automatycznie przerzucane do pamiêci systemowej, 
			gdy urz¹dzenie graficzne jest resetowane.
		*/
	virtual HRESULT OnInitDevice() = 0;
		
		//! Inicjalizuje obiekty u¿ywaj¹ce pamiêci karty
		/*!
			W tej funkcji klasa dziedziczona powinna zainicjowaæ tylko te obiekty, 
			które znajduj¹ siê w pamiêci kart graficznej. W szczególnoœci dotyczy to buforów wierzcho³ków, 
			wiêc równie¿ obiektów \e ID3DXSprite.
		*/
	virtual HRESULT OnRestoreDevice() = 0;
		
		//! Zwalnia obiekty u¿ywaj¹ce pamiêci karty
		/*!
			W tej funkcji klasa dziedziczona powinna zwolniæ obiekty zainicjowane w #OnRestoreDevice().
		*/
	virtual HRESULT OnInvalidateDevice() = 0;
		
		//! Zwalnia obiekty u¿ywaj¹ce pamiêci systemowej
		/*!
			W tej funkcji klasa dziedziczona powinna zwolniæ obiekty zainicjowane w #OnInitDevice().
		*/
	virtual HRESULT OnDeleteDevice() = 0;

		//! Przetwarza dane wejœciowe z myszki
		/*!
			Funkcja jest wywo³ywana dla ka¿dego elementu z bufora urz¹dzenia.
			\param didod	Dane z bufora
		*/
	virtual HRESULT OnMouseEvent( LPDIDEVICEOBJECTDATA didod ) = 0;
		
		//! Przetwarza dane wejœciowe z klawiatury
		/*!
			Funkcja jest wywo³ywana dla ka¿dego elementu z bufora urz¹dzenia.
			\param didod	Dane z bufora
		*/
	virtual HRESULT OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod ) = 0;
		
		//! Wykonuje ruch obiektów sceny
	virtual HRESULT FrameMove( float fElapsedTime ) = 0;
		
		//! Renderuje obiekty sceny
	virtual HRESULT FrameRender() = 0;
		
		//! Ustawia wskaŸnik do nastepnej sceny
		/*!
			Jeœli istnieje potrzeba utworzenia sceny podrzêdnej (np. wchodzimy z menu g³ównego do menu opcji) 
			funkcja umo¿liwia to. W tym celu nale¿y utworzyæ obiekt klasy dziedziczonej od #CD3DScene
			i jego adres podaæ jako argument. Jeœli bie¿¹ca scena zakoñczy³a siê podaæ na \b NULL. 
			\param pScene	Adres scene podrzêdnej lub null jeœli mamy zakoñczyæ tê scenê.
		*/
	void SetCurrentScene( CD3DScene* pScene ) 
		{ pCurrentScene = pScene; }

		//! Wczytuje teksturê z pliku. 
		/*!
			Tekstura zostanie automatycznie zwolniona w destruktorze klasy.
			\param strFileName	Nazwa pliku tekstury. Obs³ugiwane formaty: .bmp, .dds, .dib, .jpg, .png, and .tga
			\param pTex			Adres wskaŸnika do tekstury który otrzymuje za³adowan¹ teksturê.
		*/
	HRESULT LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex );

		//! Obiekt Direct3D wspólny dla wszystkich scen
	static LPDIRECT3DDEVICE8 pD3DDevice;

private:
	CD3DScene* GetNextScene();
	CD3DScene* pCurrentScene;
	stack<LPDIRECT3DTEXTURE8> stackTextures;
	friend class CD3DApp;
};

#endif
