#include "stdafx.h"

/* 
	(c) David C. H. Nash
	This heightpatch is *loosely* based on the heightpatch in the
	scattering code by

	(c) 2002 Nathaniel Hoffman, Kenneth J. Mitchell and Arcot J. Preetham 

*/

extern int CUBEMAP_FACE_SIZE;

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatch::HeightPatch()
{
	PATCH_POWER_OF_2= 8; //7
	PATCH_WIDTH= (1<<PATCH_POWER_OF_2);
	PATCH_SIZE= (PATCH_WIDTH+1);
	NUM_PATCH_VERTICES= (PATCH_SIZE*PATCH_SIZE);
	PATCH_SCALE= 1.0f;
	PATCH_WORLD_WIDTH= (PATCH_SCALE*PATCH_WIDTH);
	INV_PATCH_WORLD_WIDTH= (1.0f/PATCH_WORLD_WIDTH);
	HALF_PATCH_WORLD_WIDTH= (PATCH_WORLD_WIDTH*0.5f);
	HEIGHT_SCALE= (256.0f/256.0f); //(256/256)
	TEX_COORD_RANGE= 8.0f;

	m_NumVertices = NUM_PATCH_VERTICES;
	int z,x;
	int size = PATCH_SIZE;
	float fx, fz;
	float scale = PATCH_SCALE;
	m_vertices = new HeightPatchVertex[ m_NumVertices ];

	for (z=0; z<size; z++)
	{
		for (x=0; x<size; x++)
		{
			float fy;

			// this converts the (x,z) into a one
			// dimensional number for accessing the vertices
			int indx = x + z * size;

			// -----------------------------------------------
			// Generate the grid
			// -----------------------------------------------
			fx = ((float)x) * scale - float(size-1) * scale / 2;
			fz = ((float)z) * scale - float(size-1) * scale / 2;
			fy = 0.0f;

			m_vertices[indx].x = fx;
			m_vertices[indx].z = fz;
			m_vertices[indx].y = fy;

			m_vertices[indx].tu = float(x) / float(size-1);
			m_vertices[indx].tv = float(z) / float(size-1);

			// try to correct the texture problem...
			if( m_vertices[indx].tu >= 1.0f )
				m_vertices[indx].tu = 0.999999f;
			if( m_vertices[indx].tv >= 1.0f )
				m_vertices[indx].tv = 0.999999f;

			// try to correct the texture problem...
			if( m_vertices[indx].tu <= 0.0f )
				m_vertices[indx].tu = 0.000001f;
			if( m_vertices[indx].tv <= 0.0f )
				m_vertices[indx].tv = 0.000001f;

			// need to set the normal again later
			m_vertices[indx].nx = 0.0f;
			m_vertices[indx].ny = 1.0f;
			m_vertices[indx].nz = 0.0f;
		}
	}

}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatch::HeightPatch( int power_of_two, float scale, float height_scale, float tex_coord_range )
{
	PATCH_POWER_OF_2= power_of_two;
	PATCH_WIDTH= (1<<PATCH_POWER_OF_2);
	PATCH_SIZE= (PATCH_WIDTH+1);
	NUM_PATCH_VERTICES= (PATCH_SIZE*PATCH_SIZE);
	PATCH_SCALE= scale;
	PATCH_WORLD_WIDTH= (PATCH_SCALE*PATCH_WIDTH);
	INV_PATCH_WORLD_WIDTH= (1.0f/PATCH_WORLD_WIDTH);
	HALF_PATCH_WORLD_WIDTH= (PATCH_WORLD_WIDTH*0.5f);
	HEIGHT_SCALE= height_scale;
	TEX_COORD_RANGE= tex_coord_range;

	m_NumVertices = NUM_PATCH_VERTICES;

	int z,x;
	int size = PATCH_SIZE;
	float fx, fz;

	m_vertices = new HeightPatchVertex[ m_NumVertices ];

	for (z=0; z<size; z++)
	{
		for (x=0; x<size; x++)
		{
			float fy;

			// this converts the (x,z) into a one
			// dimensional number for accessing the vertices
			int indx = x + z*size;
			
			// -----------------------------------------------
			// Generate the grid
			// -----------------------------------------------
			fx = ((float)x) * scale - float(size-1) * scale / 2;
			fz = ((float)z) * scale - float(size-1) * scale / 2;
			fy = 0.0f;

			m_vertices[indx].x = fx;
			m_vertices[indx].z = fz;
			m_vertices[indx].y = fy;

			m_vertices[indx].tu = float(x) / float(size-1);
			m_vertices[indx].tv = float(z) / float(size-1);

			// try to correct the texture problem...
			if( m_vertices[indx].tu >= 1.0f )
				m_vertices[indx].tu = 0.999999f;
			if( m_vertices[indx].tv >= 1.0f )
				m_vertices[indx].tv = 0.999999f;

			// try to correct the texture problem...
			if( m_vertices[indx].tu <= 0.0f )
				m_vertices[indx].tu = 0.000001f;
			if( m_vertices[indx].tv <= 0.0f )
				m_vertices[indx].tv = 0.000001f;


			// need to set the normal again later
			m_vertices[indx].nx = 0.0f;
			m_vertices[indx].ny = 1.0f;
			m_vertices[indx].nz = 0.0f;

		}
	}
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatch::~HeightPatch()
{
	SAFE_DELETE(m_vertices);
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
float HeightPatch::GetSize()
{
	return PATCH_SIZE * PATCH_SCALE;
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatch::GetListofVertices( D3DXVECTOR3 *out, HeightPatchVertex *in )
{
	for( int i = 0; i < NUM_PATCH_VERTICES; i++ )
	{
		out[i] = D3DXVECTOR3(in[i].x, in[i].y, in[i].z);
	}
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatch::GetListofTriangles( unsigned int *tris, D3DXVECTOR3 *verts )
{
	// Generate indices for each quad.
	unsigned int k = 0;
	for(unsigned int i = 0; i < PATCH_SIZE-1; ++i)
	{
		for(unsigned int j = 0; j < PATCH_SIZE-1; ++j)
		{
			tris[k]     =   i   * (PATCH_SIZE) + j;
			tris[k + 2] =   i   * (PATCH_SIZE) + j + 1;
			tris[k + 1] = (i+1) * (PATCH_SIZE) + j;
					
			tris[k + 3] = (i+1) * (PATCH_SIZE) + j;
			tris[k + 5] =   i   * (PATCH_SIZE) + j + 1;
			tris[k + 4] = (i+1) * (PATCH_SIZE) + j + 1;

			// next quad
			k += 6;
		}
	}
}


//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
SphericalHeightPatch::SphericalHeightPatch()
{
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
SphericalHeightPatch::~SphericalHeightPatch()
{
}

//
//	Problems to solve here:
//	1) use of D3DX
//

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void SphericalHeightPatch::Generate( HeightPatch *patch, int dir )
{

	// this is stored so that we can recall it later,
	m_patch = patch;

	//*****************************************************
	// This matrix sets the orientation of the mapping
	//*****************************************************

	D3DXMATRIX matRot;
	Matrix4<float> m;

	switch( dir )
	{
	case 0: // NEGX
		m = Matrix4<float>::zrotation( D3DX_PI/2 );
		break;
	case 1: // POSX	 
		m = Matrix4<float>::zrotation( -D3DX_PI/2 );
		break;
	case 2: // POSZ
		m = Matrix4<float>::xrotation( D3DX_PI/2 );
		break;
	case 3:	// NEGZ
		m = Matrix4<float>::xrotation( -D3DX_PI/2 );
		break;
	case 4: //POSY
		m = Matrix4<float>::identity();
		break;
	case 5: // NEGY
		m = Matrix4<float>::zrotation( D3DX_PI );
		break;
	}

	for (int z=0; z < patch->PATCH_SIZE; z++)
	{
		for (int x=0; x < patch->PATCH_SIZE; x++)
		{
			int indx = x + z * patch->PATCH_SIZE;

			// -----------------------------------------------
			// normalize the position vector, add height,
			// then transform according to cubemap matrix
			// -----------------------------------------------
			Vector3<float> vt2(patch->m_vertices[indx].x, 
							   patch->m_vertices[indx].y, 
							   patch->m_vertices[indx].z);

			Vector3<float> vt;

			// need to change the y coordinate to half the patch
			// size above the floor, in order to place it above a cube...
			vt2.y = - float(patch->PATCH_SIZE-1) * patch->PATCH_SCALE / 2;

			vt2.normalize();

			vt = m * vt2;

			patch->m_vertices[indx].x = vt.x;
			patch->m_vertices[indx].z = vt.z;
			patch->m_vertices[indx].y = vt.y;

			patch->m_vertices[indx].nx = vt.x;
			patch->m_vertices[indx].ny = vt.y;
			patch->m_vertices[indx].nz = vt.z;
		}
	}
}


//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatchDisplacer::HeightPatchDisplacer()
{
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatchDisplacer::~HeightPatchDisplacer()
{
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchDisplacer::Displace( HeightPatch *patch, int dispace_type, char* filename, Vector3<float> *dispdir )
{
	/*LockableTexture *lockable_texture = new LockableTexture();
	lockable_texture->LoadTexture(filename, pd3dDevice);
	lockable_texture->Lock(0);

	for (int z=0; z<patch->PATCH_SIZE; z++)
	{
		for (int x=0; x<patch->PATCH_SIZE; x++)
		{
			int indx = x + z*patch->PATCH_SIZE;

			Vector3<float> vt(patch->m_vertices[indx].x, 
							  patch->m_vertices[indx].y, 
							  patch->m_vertices[indx].z);

			UInt32 r = GetColor( lockable_texture, x, z, 1 );

			// add to the height
			float h = (((float)(r)) * HEIGHT_SCALE * 0.4);

			
			vt.x = vt.x + vt.x*h;
			vt.y = vt.y + vt.y*h;
			vt.z = vt.z + vt.z*h;
			
			m_vertices[indx].x = vt.x;
			m_vertices[indx].z = vt.z;
			m_vertices[indx].y = vt.y;
		}
	}

	lockable_texture->Unlock(0);
	*/
}



//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatchImpl::HeightPatchImpl()
{
	m_VertexBuffer = NULL;
	m_IndexBuffer = NULL;
	m_NumVertices = 0;
	m_NumIndices = 0;
	m_patch = NULL;
	texture_id = -1;

}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
HeightPatchImpl::~HeightPatchImpl()
{
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_IndexBuffer);
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchImpl::Initialize(HeightPatch *patch, LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_patch = patch;

	CreateVertexBuffer(pd3dDevice);
	CreateIndexBuffer(pd3dDevice);
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchImpl::SetTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchImpl::SetTextureID(int id)
{
	texture_id = id;
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
int HeightPatchImpl::GetTextureID()
{ 
	return texture_id; 
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchImpl::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	//pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pd3dDevice->SetFVF(HEIGHTPATCH_FVF);

	//pd3dDevice->SetTexture(0, m_pTex);

	pd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(HeightPatchVertex));
	
	// set indices
	pd3dDevice->SetIndices(m_IndexBuffer);

	// draw patch
	pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_NumVertices, 0, m_NumIndices);


}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchImpl::CreateVertexBuffer(LPDIRECT3DDEVICE9 pd3dDevice)
{

	HRESULT result;
	HeightPatchVertex* vertices;
	HeightPatchVertex* verts2;	

	m_NumVertices = m_patch->m_NumVertices;

	result = pd3dDevice->CreateVertexBuffer(m_NumVertices * sizeof(HeightPatchVertex), 
										    D3DUSAGE_WRITEONLY, 
											HEIGHTPATCH_FVF, 
										    D3DPOOL_MANAGED, 
											&m_VertexBuffer, 
											0);

	// load height field into buffer
	result = m_VertexBuffer->Lock(0, m_NumVertices * sizeof(HeightPatchVertex), (void**)&vertices, 0);

	verts2 = m_patch->m_vertices;

	memcpy((void*)vertices, (void*)m_patch->m_vertices, m_NumVertices * sizeof(HeightPatchVertex));
	 
	m_VertexBuffer->Unlock();
}

//-----------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------
void HeightPatchImpl::CreateIndexBuffer(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT result;

	// create index buffer
	int size = m_patch->PATCH_SIZE;
	int step = 1;
	int psize = (size-1)/step;

	// calculate number of indices
	m_NumIndices=((psize))*(((psize+1))*2)+(psize*2)-2;

	// Create index buffer
	result = pd3dDevice->CreateIndexBuffer((m_NumIndices+2)*sizeof(DWORD),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_IndexBuffer, 0);


	// create indices
	DWORD* indices;

	result = m_IndexBuffer->Lock(0, 0, (void**)&indices, 0);

	// generates indices to process patch in resonably cache friendly 'S' format
	int x,z,s_toggle=1;
	for (z=step; z<size; z+=step)
	{
		if (s_toggle)
		{
			for (x=0; x<(size-step); x+=step)
			{
				*indices++=(DWORD)((z-0)*size+(x));
				*indices++=(DWORD)((z-step)*size+(x));
			}

			*indices++=(DWORD)((z-0)*size+(size-1));
			*indices++=(DWORD)((z-step)*size+(size-1));

			// insert additional degenerates to start next row
			*indices++=indices[-2];
			*indices++=indices[-1];
		}
		else
		{
			*indices++=(DWORD)((z-step)*size+(size-1));
			*indices++=(DWORD)((z-0)*size+(size-1));

			for (x=size-1; x>0; x-=step)
			{
				*indices++=(DWORD)((z-step)*size+(x-step));
				*indices++=(DWORD)((z-0)*size+(x-step));
			}

			// insert additional degenerates to start next row
			*indices++=indices[-1];
			*indices++=indices[-1];
		}

		s_toggle=!s_toggle;
	}

	m_IndexBuffer->Unlock();
}