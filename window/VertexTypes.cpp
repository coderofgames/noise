#include "stdafx.h"
#include "VertexTypes.h"


const D3DVERTEXELEMENT9 PPVERT::Decl[4] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
    { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1 },
    D3DDECL_END()
};

IDirect3DVertexDeclaration9* g_pVertDeclPP=NULL;


void CreateVertexDeclerations(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->CreateVertexDeclaration( PPVERT::Decl, &g_pVertDeclPP );
}
//

