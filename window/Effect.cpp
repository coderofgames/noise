#include "stdafx.h"


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Effect::Effect(): ResourceItem(), InstanceCounter<Effect>()
{
	m_pEffect = NULL;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Effect::~Effect()
{
	SAFE_RELEASE( m_pEffect );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Effect::LoadEffect( char* name, LPDIRECT3DDEVICE9 pd3dDevice )
{
	ResourceItem::SetName( name );
	InitEffect( name, pd3dDevice, &m_pEffect );
}


//-----------------------------------------------------------------------------
// Name: InitEffect
// Desc: This function creates an effect
//-----------------------------------------------------------------------------
void InitEffect( char* filename, LPDIRECT3DDEVICE9 device, LPD3DXEFFECT *effect )
{
	HRESULT hr;

	LPD3DXBUFFER pBufferErrors = NULL;

	WCHAR Wfilename[256];
	OutputMessage::ToWchar( filename, Wfilename );
	hr = D3DXCreateEffectFromFile( device, Wfilename, 0, 0, 0, 0, effect, &pBufferErrors );

	if( FAILED(hr) )
	{
		LPVOID pCompilErrors = pBufferErrors->GetBufferPointer();
		ERROR((char*)pCompilErrors);
	}
}