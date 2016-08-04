#include "stdafx.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
TextureManager::TextureManager() : Singleton<TextureManager>(*this), CareTaker ()
{
	assert (Singleton <TextureManager>::GetInstance());
	
	CareTaker::SetName ("TextureManager");
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
TextureManager::~TextureManager()
{
	Cleanup();
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int TextureManager::addTexture( char* name,  LPDIRECT3DDEVICE9 pd3dDevice )
{
	char strBMPName[256];
	char strPNGName[256];

	strcpy( strBMPName, name );
	strcpy( strPNGName, name );

	strcat( strBMPName, ".bmp");
	strcat( strPNGName, ".png");

	int i = 0;
	for( i = 0; i < m_textures.size(); i++ )
	{
		if( !strcmp( m_textures[i]->GetName(), strBMPName ))
			return i;

		if( !strcmp( m_textures[i]->GetName(), strPNGName ))
			return i;
	}

	Texture* tex = new Texture();
	HRESULT hr = tex->LoadTexture( strPNGName, pd3dDevice );

	if( FAILED(hr) )
	{
		hr = tex->LoadTexture( strBMPName, pd3dDevice );

		if( FAILED(hr) )
		{
			ERROR( FORMAT_TEXT( "Cannot load texture in either .png or .bmp format : %s", name ));
			return m_textures.size()-1;
		}
	}


	m_textures.push_back( tex );

	return m_textures.size()-1;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Texture* TextureManager::retrieveTexture( int i )
{
	return m_textures[i];
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void TextureManager::Cleanup()
{
	for( int i = 0; i < m_textures.size(); i++ )
	{
		delete m_textures[i];
		m_textures[i] = NULL;
	}

	m_textures.clear();
	
}



//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
CubicTextureManager::CubicTextureManager() : Singleton<CubicTextureManager>(*this), CareTaker ()
{
	assert (Singleton <CubicTextureManager>::GetInstance());
	
	CareTaker::SetName ("CubicTextureManager");
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
CubicTextureManager::~CubicTextureManager()
{
	Cleanup();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int CubicTextureManager::CreateCubeTextureByte4( LPDIRECT3DDEVICE9 pd3dDevice, int size, int numMipLevels )
{
	CubicTexture* tex = new CubicTexture();
	tex->CreateCubeTextureByte4( pd3dDevice, size, numMipLevels );

	m_cubicTextures.push_back( tex );

	return m_cubicTextures.size()-1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int CubicTextureManager::addTexture( char* name, LPDIRECT3DDEVICE9 pd3dDevice )
{
	return 0;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
CubicTexture* CubicTextureManager::retrieveTexture( int i )
{
	return m_cubicTextures[i];
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void CubicTextureManager::Cleanup()
{
	for( int i = 0; i < m_cubicTextures.size(); i++ )
	{
		delete m_cubicTextures[i];
		m_cubicTextures[i] = NULL;
	}

	m_cubicTextures.clear();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
