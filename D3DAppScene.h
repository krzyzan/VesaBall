#pragma once


#include <list>
#include <vector>
using namespace std;

#include <d3dx8.h>
#include <dinput.h>

class CObject;

class CD3DAppScene
{
public:
	CD3DAppScene( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CD3DAppScene();

	virtual HRESULT InitDeviceObjects()			= 0;
	virtual HRESULT RestoreDeviceObjects()		= 0;
	virtual HRESULT RenderLoop()				= 0;
	virtual HRESULT InvalidateDeviceObjects()	= 0;
	virtual HRESULT DeleteDeviceObjects()		= 0;

	HRESULT LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex );

protected:
	LPDIRECT3DDEVICE8			pd3dDevice;
	LPDIRECTINPUTDEVICE8		pDIDevice;
	list<CObject*>				listRender;			//TODO: wywaliæ do dziedziczonych

private:
	vector<LPDIRECT3DTEXTURE8>	vtrTexture;
};
