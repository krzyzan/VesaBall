#pragma once
#include "d3dappscene.h"

class CGameEditor :
	public CD3DAppScene
{
public:
	CGameEditor( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice );
	virtual ~CGameEditor(void);
};
