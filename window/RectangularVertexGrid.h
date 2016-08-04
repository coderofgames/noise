#ifndef RECT_VERT_GRID_H
#define RECT_VERT_GRID_H


class HeightPatchVertex
{
public:
	float x,y,z;
	float nx, ny, nz;
	float tu, tv;

}; 

#define HEIGHTPATCH_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

class HeightPatchTriangle
{
public:
    D3DXVECTOR3 positions[3];
};

class Tile
{
public:
    HeightPatchTriangle triangles[2];
    D3DXVECTOR3 positions[4];
};


class HeightPatch
{
public:
	HeightPatch();
	HeightPatch( int power_of_two, float scale, float height_scale, float tex_coord_range );
	~HeightPatch();
	
	float GetSize();
	float GetWorldWidth(){ return PATCH_WORLD_WIDTH; }
	float GetHalfPatchWorldWidth(){ return HALF_PATCH_WORLD_WIDTH; }
	void GetListofVertices( D3DXVECTOR3 *out, HeightPatchVertex *in );
	void GetListofTriangles( unsigned int *tris, D3DXVECTOR3 *verts );
	DWORD GetNumVerts(){ return NUM_PATCH_VERTICES; }
	DWORD GetNumTriangles() { return (PATCH_SIZE-1)*(PATCH_SIZE-1)*2; }
	void GetVertsAndTriangles( D3DXVECTOR3** verts, unsigned int **tris )
	{
		(*verts) = new D3DXVECTOR3[NUM_PATCH_VERTICES];
		(*tris) = new unsigned int[ (PATCH_SIZE-1)*(PATCH_SIZE-1)*2*3 ];
		GetListofVertices( *verts, m_vertices );
		GetListofTriangles( *tris, *verts );
	}

	HRESULT Draw(LPDIRECT3DDEVICE9 pd3dDevice);

	DWORD					m_NumVertices;
	int						m_NumIndices;

	HeightPatchVertex*	m_vertices;

	int PATCH_POWER_OF_2; 
	int PATCH_WIDTH;
	int PATCH_SIZE; 
	int NUM_PATCH_VERTICES;
	float PATCH_SCALE; 
	float PATCH_WORLD_WIDTH;
	float INV_PATCH_WORLD_WIDTH;
	float HALF_PATCH_WORLD_WIDTH;
	float HEIGHT_SCALE;
	float TEX_COORD_RANGE;
};


class SphericalHeightPatch
{
public:

	SphericalHeightPatch();
	~SphericalHeightPatch();

	void Generate( HeightPatch *patch, int dir );

	HeightPatch *m_patch;

};


class HeightPatchDisplacer
{
public:

	enum DISPLACE_TYPE
	{
		DISPLACE_NORMAL,
		DISPLACE_DIR
	};

	HeightPatchDisplacer();
	~HeightPatchDisplacer();

	void Displace( HeightPatch *patch, int dispace_type, char* filename, Vector3<float> *dir = 0 );

	HeightPatch *m_patch;
};


class HeightPatchImpl
{
public:

	HeightPatchImpl();
	~HeightPatchImpl();

	void Render(LPDIRECT3DDEVICE9 pd3dDevice);
	void Initialize(HeightPatch *patch, LPDIRECT3DDEVICE9 pd3dDevice);

	void SetTexture(LPDIRECT3DTEXTURE9 pTex);
	void SetTextureID(int id);
	void SetSpecularTextureID(int id){specular_texture_id = id; }
	int GetTextureID();
	int GetSpecularTextureID(){ return specular_texture_id; }


	void CreateVertexBuffer(LPDIRECT3DDEVICE9 pd3dDevice);
	void CreateIndexBuffer(LPDIRECT3DDEVICE9 pd3dDevice);


	LPDIRECT3DTEXTURE9		m_pTex;
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;
	DWORD					m_NumVertices;
	LPDIRECT3DINDEXBUFFER9	m_IndexBuffer;
	int						m_NumIndices;
	HeightPatch				*m_patch;
	int						texture_id;
	int						specular_texture_id;

};

#endif
