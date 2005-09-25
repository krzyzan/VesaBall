#ifndef D3DAPP_H
#define D3DAPP_H

#include <d3dx8.h>
#include <stack>
using namespace std;

#include "d3dscene.h"
#include "timer.h"


//TODO: pozbyæ siê tego

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
	U³atwia korzystanie z Direct3D. Zajmuje siê inicjalizacj¹ interfejsu D3D oraz enumeracj¹ dostepnych trybów graficznych.
	Zarz¹dza obiektami #CD3DScene wykonuj¹c w odpowiedniej kolejnoœci ich inicjalizacjê, resetowanie oraz zamykanie.
*/

class CD3DApp 
{
	// Struktura przechowuj¹ca informacjê dotycz¹c¹ trybu graficznego.
	struct D3DModeInfo
	{
		DWORD	   Width;				// Rozdzielczoœæ pozioma w pikselach
		DWORD	   Height;				// Rozdzielczoœæ pionowa w pikselach
		D3DFORMAT  Format;				// Format piksela
		DWORD	   RefreshRate;
		DWORD	   dwBehavior;			// Przetwarzanie wierzcho³ków Hardware / Software / Mixed
		D3DFORMAT  DepthStencilFormat;	// Który format bufora g³êbi/stencil u¿yæ z tym trybem
	};

	// Struktura przechowuj¹ca informacjê dotycz¹c¹ urz¹dzenia Direct3D, 
	// wraz z list¹ trybów compatybilnych z tym urz¹dzeniem.
	struct D3DDeviceInfo
	{
		// Dane urz¹dzenia
		D3DDEVTYPE	 DeviceType;	  // Reference, HAL, itp.
		D3DCAPS8	 d3dCaps;		  // Mo¿liwoœci urz¹dzenia
		const TCHAR* strDesc;		  // Nazwa urz¹dzenia

		// Tryby graficzne dla tego urz¹dzenia
		DWORD		 dwNumModes;
		D3DModeInfo  modes[150];

		// Bie¿¹cy stan
		DWORD		 dwCurrentMode;
		D3DMULTISAMPLE_TYPE MultiSampleType;
	};


	// Struktura przechowuj¹ca informacjê dotycz¹c¹ karty graficznej, 
	// wraz z list¹ urz¹dzeñ zgodnych z Direct3D dostêpnych na tej karcie.
	struct D3DAdapterInfo
	{
		// Dane karty graficznej
		D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
		D3DDISPLAYMODE d3ddmDesktop;	  // Desktop display mode for this adapter

		// Urz¹dzenia na tej karcie graficznej
		DWORD		   dwNumDevices;
		D3DDeviceInfo  devices[5];

		// Bie¿¹cy stan
		DWORD		   dwCurrentDevice;
	};

public:
		//! Konstruktor
		/*!
			\param	WindowTitle	Tytu³ okna aplikacji
		*/
	CD3DApp( TCHAR* WindowTitle );

		//! Tworzy aplikacje Direct3D
		/*!
			- Tworzy okno aplikacji.
			- Enumeruje i wybiera odpowiedni tryb obs³ugiwany przez kartê graficzn¹.
			- Inicjalizuje asynchroniczny (buforowany) odczyt z klawiatury i myszy.
			- Startuje wewnetrzny timer
			- Inicjalizuje generator liczb losowych
		*/
	HRESULT Create( HINSTANCE hInstance );

		//! Ustawia bie¿¹c¹ scenê
		/*!
			Powinna byæ wywo³ana po wykonaniu funkcji #Create();
		*/
	HRESULT StartNewScene( CD3DScene* pScene );

		//! Uruchamia g³ówn¹ pêtlê programu
		/*!
			Zajmuje siê pobieraniem i przetwarzaniem zdarzeñ Windows
			Jeœli nie ma ¿adnych zdarzeñ, renderuje bie¿¹c¹ scenê.
		*/
	HRESULT Run();

private:
	// Wewnêtrzne funkcje zarz¹dzaj¹ce i renderuj¹ce sceny 3D
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

	// Wewnêtrzne funkcje enumeruj¹ce tryby karty
	HRESULT BuildDeviceList();
	BOOL	FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
				D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat );

	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	// Wewnêtrzne zmienne okreœlaj¹ce stan aplikacji
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

	// G³ówny timer
	CTimer					Timer;

private:
	// Obiekty u¿ywane do tworzenia i renderowania sceny 3D

	// Parametry dla CreateDevice/Reset
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DAdapterInfo			Adapter;
	
	// Mo¿liwoœci urz¹dzenia
	D3DCAPS8	d3dCaps;

	// Wskazuje sprzetowe/programowe przetwarzanie wierzcho³ków
	DWORD		CreateFlags;

	// Minimalna liczba bitów w buforze g³êbi kolorów
	DWORD		MinDepthBits;	
	// Minimalna liczba bitów w buforze stencil
	DWORD		MinStencilBits;	

	// Tytu³ okna aplikacji
	TCHAR*		strWindowTitle;

private:
	// G³ówne okno aplikacji
	HWND		hWnd;

	// Statyczny wskaŸnik do aplikacji, potrzebny dla statycznej WndProc()
	static CD3DApp* spD3DApp;

	// Statyczny handler zdarzeñ, przekazuje zdarzenia do aplikacji spD3DApp.
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif
