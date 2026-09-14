#ifndef D3DAPP_H
#define D3DAPP_H

#include <d3dx8.h>
#include <stack>
using namespace std;

#include "d3dscene.h"
#include "Timer.h"


//TODO: pozby� si� tego

enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D		  0x82000001
#define D3DAPPERR_NOWINDOW			  0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE	  0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE	  0x82000006
#define D3DAPPERR_NOWINDOWEDHAL 	  0x82000007
#define D3DAPPERR_NODESKTOPHAL		  0x82000008
#define D3DAPPERR_NOHALTHISMODE 	  0x82000009
#define D3DAPPERR_NONZEROREFCOUNT	  0x8200000a
#define D3DAPPERR_MEDIANOTFOUND 	  0x8200000b
#define D3DAPPERR_RESIZEFAILED		  0x8200000c

const int MOUSE_BUFFER_SIZE		= 16;
const int KEYBRD_BUFFER_SIZE	= 16;


//! Aplikacja Direct3D
/*!
	U�atwia korzystanie z Direct3D. Zajmuje si� inicjalizacj� interfejsu D3D oraz enumeracj� dostepnych tryb�w graficznych.
	Zarz�dza obiektami #CD3DScene wykonuj�c w odpowiedniej kolejno�ci ich inicjalizacj�, resetowanie oraz zamykanie.
*/

class CD3DApp 
{
	// Struktura przechowuj�ca informacj� dotycz�c� trybu graficznego.
	struct D3DModeInfo
	{
		DWORD	   Width;				// Rozdzielczo�� pozioma w pikselach
		DWORD	   Height;				// Rozdzielczo�� pionowa w pikselach
		D3DFORMAT  Format;				// Format piksela
		DWORD	   RefreshRate;
		DWORD	   dwBehavior;			// Przetwarzanie wierzcho�k�w Hardware / Software / Mixed
		D3DFORMAT  DepthStencilFormat;	// Kt�ry format bufora g��bi/stencil u�y� z tym trybem
	};

	// Struktura przechowuj�ca informacj� dotycz�c� urz�dzenia Direct3D, 
	// wraz z list� tryb�w compatybilnych z tym urz�dzeniem.
	struct D3DDeviceInfo
	{
		// Dane urz�dzenia
		D3DDEVTYPE	 DeviceType;	  // Reference, HAL, itp.
		D3DCAPS8	 d3dCaps;		  // Mo�liwo�ci urz�dzenia
		const TCHAR* strDesc;		  // Nazwa urz�dzenia

		// Tryby graficzne dla tego urz�dzenia
		DWORD		 dwNumModes;
		D3DModeInfo  modes[150];

		// Bie��cy stan
		DWORD		 dwCurrentMode;
		D3DMULTISAMPLE_TYPE MultiSampleType;
	};


	// Struktura przechowuj�ca informacj� dotycz�c� karty graficznej, 
	// wraz z list� urz�dze� zgodnych z Direct3D dost�pnych na tej karcie.
	struct D3DAdapterInfo
	{
		// Dane karty graficznej
		D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
		D3DDISPLAYMODE d3ddmDesktop;	  // Desktop display mode for this adapter

		// Urz�dzenia na tej karcie graficznej
		DWORD		   dwNumDevices;
		D3DDeviceInfo  devices[5];

		// Bie��cy stan
		DWORD		   dwCurrentDevice;
	};

public:
		//! Konstruktor
		/*!
			\param	WindowTitle	Tytu� okna aplikacji
		*/
	CD3DApp( TCHAR* WindowTitle );

		//! Tworzy aplikacje Direct3D
		/*!
			- Tworzy okno aplikacji.
			- Enumeruje i wybiera odpowiedni tryb obs�ugiwany przez kart� graficzn�.
			- Inicjalizuje asynchroniczny (buforowany) odczyt z klawiatury i myszy.
			- Startuje wewnetrzny timer
			- Inicjalizuje generator liczb losowych
		*/
	HRESULT Create( HINSTANCE hInstance );

		//! Ustawia bie��c� scen�
		/*!
			Powinna by� wywo�ana po wykonaniu funkcji #Create();
		*/
	HRESULT StartNewScene( CD3DScene* pScene );

		//! Uruchamia g��wn� p�tl� programu
		/*!
			Zajmuje si� pobieraniem i przetwarzaniem zdarze� Windows
			Je�li nie ma �adnych zdarze�, renderuje bie��c� scen�.
		*/
	HRESULT Run();

private:
	// Wewn�trzne funkcje zarz�dzaj�ce i renderuj�ce sceny 3D
	HRESULT	ChangeScene();
	HRESULT StartParentScene();

	HRESULT ProcessMouseEvents();
	HRESULT ProcessKeyboardEvents();

	// DirectInput
	HRESULT InitializeMouseInput();
	HRESULT InitializeKeyboardInput();

	HRESULT Initialize3DEnvironment();
	HRESULT Render3DEnvironment();
	HRESULT Reset3DEnvironment();
	HRESULT	Cleanup3DEnvironment();

	// Wewn�trzne funkcje enumeruj�ce tryby karty
	HRESULT BuildDeviceList();
	BOOL	FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
				D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );

	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	// Wewn�trzne zmienne okre�laj�ce stan aplikacji
	bool					bActive;
	bool					bReady;

	// Stos scen
	stack<CD3DScene*>		sD3DScenes;

	// Direct3D
	LPDIRECT3D8 			pD3D;
	LPDIRECT3DDEVICE8		pD3DDevice;

	// DirectInput
	LPDIRECTINPUT8			pDI;
	LPDIRECTINPUTDEVICE8	pDIMouse;
	LPDIRECTINPUTDEVICE8	pDIKeyboard;

	// G��wny timer
	CTimer					Timer;

private:
	// Obiekty u�ywane do tworzenia i renderowania sceny 3D

	// Parametry dla CreateDevice/Reset
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DAdapterInfo			Adapter;
	
	// Mo�liwo�ci urz�dzenia
	D3DCAPS8	d3dCaps;

	// Wskazuje sprzetowe/programowe przetwarzanie wierzcho�k�w
	DWORD		CreateFlags;

	// Minimalna liczba bit�w w buforze g��bi kolor�w
	DWORD		MinDepthBits;	
	// Minimalna liczba bit�w w buforze stencil
	DWORD		MinStencilBits;	

	// Tytu� okna aplikacji
	TCHAR*		strWindowTitle;

private:
	// G��wne okno aplikacji
	HWND		hWnd;

	// Statyczny wska�nik do aplikacji, potrzebny dla statycznej WndProc()
	static CD3DApp* spD3DApp;

	// Statyczny handler zdarze�, przekazuje zdarzenia do aplikacji spD3DApp.
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif
