#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H


struct PositionTextureVertex
{
	float x, y, z;
	float tu, tv;
};

#define POSITION_TEXTURE_FVF ( D3DFVF_XYZ | D3DFVF_TEX1 )


struct Vertex
{
    float x, y, z;
	float nx, ny, nz;
	float tu, tv;
	float tx, ty, tz;

   enum FVF
    {
        FVF_Flags = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(2))
    }; 
};

#define D3DFVF_NSCUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(2))



//--------------------------------------------------------------------------------------
// This is the vertex format used with the quad during post-process.
struct PPVERT
{
    float x, y, z, rhw;
    float tu, tv;       // Texcoord for post-process source
    float tu2, tv2;     // Texcoord for the original scene

    const static D3DVERTEXELEMENT9 Decl[4];
};

extern IDirect3DVertexDeclaration9* g_pVertDeclPP;


void CreateVertexDeclerations(LPDIRECT3DDEVICE9 pDevice);


#endif





