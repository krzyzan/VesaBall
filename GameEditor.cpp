#include "stdafx.h"
#include "gameeditor.h"

CGameEditor::CGameEditor( LPDIRECT3DDEVICE8 d3dDevice, LPDIRECTINPUTDEVICE8 DIDevice )
	: CD3DAppScene( d3dDevice, DIDevice )
{
}

CGameEditor::~CGameEditor(void)
{
}
