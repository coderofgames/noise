//-----------------------------------------------------------------------------------------------
//
// Quad.cpp : Defines the Quad class
//-----------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Quad.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Quad::Quad(LPDIRECT3DDEVICE9 pDevice, int x, int y, int width, int height)
{
	buff = new VertexBuffer(sizeof(PPVERT), 6, D3DUSAGE_DYNAMIC, pDevice, false);
	PPVERT verts[6] = { 
		{x,			y,			1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f},
		{x + width, y,			1.0f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f},
		{x,			y + height, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f},
		{x,			y + height, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f},
		{x + width,	y,			1.0f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f},
		{x + width,	y + height, 1.0f, 0.0f,		1.0f, 1.0f,		1.0f, 1.0f}
	};

	buff->writeData(0,sizeof(PPVERT)*6, (void*)verts);

	pTex = new Texture();

	bHasExternalTextureSource = false;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Quad::~Quad()
{
	SAFE_DELETE(buff);

	if( !bHasExternalTextureSource )
	{
		delete pTex;
	}
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Quad::Draw(LPDIRECT3DDEVICE9 pDevice)
{
	//pDevice->SetVertexDeclaration(g_pVertDeclPP);
	pDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX2);
	pDevice->SetStreamSource(0, buff->m_pVB, 0, sizeof(PPVERT));
	pDevice->SetTexture(0, pTex->GetTexture());
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Quad::SetTexture( Texture *tex )
{
	delete pTex;
	pTex = tex;
	bHasExternalTextureSource = true;
}