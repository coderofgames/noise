//-----------------------------------------------------------------------------------------------
//
// Texture.cpp : Defines the Texture class
//-----------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Texture.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Texture::Texture(void) : ResourceItem(), InstanceCounter<Texture>()
{
	m_pTexture = NULL;

}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Texture::~Texture(void)
{
	LOG( "Destroyed Texture: %d", InstanceCounter<Texture>::GetRefID());
	Cleanup();
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 Texture::GetTexture(void)
{
	return m_pTexture;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
HRESULT Texture::LoadTexture(char* filename, LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;
	WCHAR Wfilename[256];
	OutputMessage::ToWchar( filename, Wfilename );
	
	ResourceItem::SetName(filename);

	hr = D3DXCreateTextureFromFile( pd3dDevice, Wfilename, &m_pTexture );

	if( FAILED(hr) )
	{
		//ERROR(FORMAT_TEXT("Failed to load texture: %s, texture id: %d", 
		//									filename, InstanceCounter<Texture>::GetRefID()));

		
	}
	return hr;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Texture::Cleanup(void)
{
	SAFE_RELEASE(m_pTexture);
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LockableTexture::LockableTexture() : Texture()
{
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LockableTexture::~LockableTexture()
{
	LOG( "Destroyed Texture(lockable): %d", InstanceCounter<Texture>::GetRefID());
	Cleanup();
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
HRESULT LockableTexture::LoadTexture(char* filename, LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;
	WCHAR Wfilename[256];
	OutputMessage::ToWchar( filename, Wfilename );

	ResourceItem::SetName(filename);
	
	hr = D3DXCreateTextureFromFileExW( pd3dDevice, Wfilename, D3DX_DEFAULT, D3DX_DEFAULT, 
										D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
										D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture);
	if( FAILED(hr) )
	{
		ERROR(FORMAT_TEXT("Failed to load texture: %s, texture id: %d", 
											filename, InstanceCounter<Texture>::GetRefID()));

		return hr;
	}

	hr = m_pTexture->GetLevelDesc(0, &pDesc);

	if( FAILED(hr) )
	{
		ERROR(FORMAT_TEXT("Cannot Get Level Description for texture: %s, texture id: %d", 
											filename, InstanceCounter<Texture>::GetRefID()));

		return hr;
	}

	return hr;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
bool LockableTexture::IsLocked(void)
{
	return bLocked;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void LockableTexture::Lock(int mip_level)
{
	bLocked = true;
	m_pTexture->LockRect(mip_level,&rect,0,D3DLOCK_DISCARD);
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void LockableTexture::Unlock(int mip_level)
{
	bLocked = false;
	m_pTexture->UnlockRect(mip_level);
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int LockableTexture::GetWidth(void)
{
	return pDesc.Width;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int LockableTexture::GetHeight(void)
{
	return pDesc.Height;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
DWORD LockableTexture::GetPixel (int x, int y)
{
	if( !bLocked )
	{
		ERROR( "Texture Reads are not possible if texture is not locked" );
		return 0;
	}

	DWORD *pDwordData = (DWORD *)rect.pBits;

	return pDwordData[ x + y*GetWidth() ];
	
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void LockableTexture::SetPixel (int x, int y, DWORD color)
{
	if( !bLocked )
	{
		ERROR( "Cannot Write to non-locked Texture" );
		return;
	}

	DWORD **pDwordData = (DWORD **)&rect.pBits;

	(*pDwordData)[ x + y*GetWidth() ] = color;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void LockableTexture::Create(LPDIRECT3DDEVICE9 pd3dDevice, int width, int height)
{
	HRESULT hr = S_OK;

	ResourceItem::SetName(FORMAT_TEXT("LockableTexture%d", InstanceCounter<Texture>::GetRefID()));
	
/*	hr = D3DXCreateTexture( pd3dDevice, width, height, 1, 
							D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, 
							D3DPOOL_SYSTEMMEM, &m_pTexture);*/

	hr = pd3dDevice->CreateTexture( width, height, 1, 
							D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, 
							D3DPOOL_SYSTEMMEM, &m_pTexture, 0);


	if(FAILED(hr))
	{
		throw CTextureException(FORMAT_TEXT("Cannot create texture: %d", 
							    InstanceCounter<Texture>::GetRefID()));
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void LockableTexture::Save(LPDIRECT3DDEVICE9 pd3dDevice, char* filename)
{
	HRESULT hr = S_OK;

	WCHAR Wfilename[256];
	OutputMessage::ToWchar( filename, Wfilename );

	hr = D3DXSaveTextureToFile( Wfilename, D3DXIFF_PNG, m_pTexture,0);
	
	if(FAILED(hr))
	{
		throw CTextureException(FORMAT_TEXT("Cannot save texture: %d", 
							    InstanceCounter<Texture>::GetRefID()));
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
VolumeTexture::VolumeTexture() : ResourceItem(), InstanceCounter<VolumeTexture>()
{
	pVol = NULL;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
VolumeTexture::~VolumeTexture()
{
	Cleanup();
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LPDIRECT3DVOLUMETEXTURE9 VolumeTexture::GetTexture(void)
{
	return pVol;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void VolumeTexture::Cleanup(void)
{
	LOG( "Destroyed Texture: %d", InstanceCounter<VolumeTexture>::GetRefID());
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
HRESULT VolumeTexture::CreateVolumeTextureByte4(LPDIRECT3DDEVICE9 pd3dDevice, int width, int height, int depth)
{
	ResourceItem::SetName(FORMAT_TEXT("VolumeTexture%d", InstanceCounter<VolumeTexture>::GetRefID()));
	
	return D3DXCreateVolumeTexture(
			pd3dDevice,
			width,
			height,
			depth,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SYSTEMMEM, // has to be uploaded later
			&pVol
			);
}

//-----------------------------------------------------------------------------------------------
// test function for filling volume textures.
//-----------------------------------------------------------------------------------------------
VOID WINAPI VolumeFill (D3DXVECTOR4* pOut, const D3DXVECTOR3* pTexCoord, 
const D3DXVECTOR3* pTexelSize, LPVOID pData)
{
   *pOut = D3DXVECTOR4(pTexCoord->x,pTexCoord->y,pTexCoord->z,1.0f);
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
CubicTexture::CubicTexture() : ResourceItem(), InstanceCounter<CubicTexture>()
{
	m_pCubeMap = NULL;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
CubicTexture::~CubicTexture()
{
	LOG( "Destroyed cube Texture: %d", InstanceCounter<CubicTexture>::GetRefID());
	Cleanup();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
HRESULT CubicTexture::CreateCubeTextureByte4(LPDIRECT3DDEVICE9 pd3dDevice, int size, int numMipLevels) 
{
	ResourceItem::SetName(FORMAT_TEXT("cubeTexture%d", InstanceCounter<CubicTexture>::GetRefID()));
	
	return D3DXCreateCubeTexture( pd3dDevice,
								  size,
								  numMipLevels,
								  D3DUSAGE_RENDERTARGET, // bRenderTarget ? D3DUSAGE_RENDERTARGET : 
								  D3DFMT_A8R8G8B8,
								  D3DPOOL_DEFAULT,
								  &m_pCubeMap );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LPDIRECT3DCUBETEXTURE9 CubicTexture::GetTexture()
{
	return m_pCubeMap;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LPDIRECT3DSURFACE9 CubicTexture::GetCubeTextureSurface( int face, int level )
{
	LPDIRECT3DSURFACE9 pSurf = NULL;

	HRESULT hr = m_pCubeMap->GetCubeMapSurface( (D3DCUBEMAP_FACES)face, 0, &pSurf );

	if( FAILED(hr) )
	{
		throw new CCubicTextureException(
			FORMAT_TEXT("Cannot create cube Texture: %s", 
						 ResourceItem::GetName() ));

		return NULL;
	}
	return pSurf;

}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void CubicTexture::Cleanup(void)
{
	SAFE_RELEASE(m_pCubeMap);
}