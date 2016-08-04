//-----------------------------------------------------------------------------------------------
//
// Quad.h : Defines the Quad class
//-----------------------------------------------------------------------------------------------
#pragma once

#include "VertexTypes.h"
#include "VertexBuffer.h"
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class Quad
{
public:

	Quad(LPDIRECT3DDEVICE9 pDevice, int x, int y, int width, int height);
	~Quad(void);

	void Draw(LPDIRECT3DDEVICE9 pDevice);
	void SetTexture( Texture *tex );

	Texture *GetTexture() {return pTex; }

	Texture		*pTex;
	PPVERT points[4];
	VertexBuffer *buff;
	bool bHasExternalTextureSource;
};


