#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H


class TextureManager	
	: public CareTaker, Singleton <TextureManager>
{
public:
	TextureManager();
	~TextureManager();

	int addTexture( char* name, LPDIRECT3DDEVICE9 pd3dDevice );
	Texture* retrieveTexture( int i );

	Texture* operator[](int i){return m_textures[i];}

	void Cleanup();

private:

	std::vector< Texture* > m_textures;
};

DECLARE_EXCEPTION( CTextureManagerException )



class CubicTextureManager	
	: public CareTaker, Singleton <CubicTextureManager>
{
public:
	CubicTextureManager();
	~CubicTextureManager();

	int CreateCubeTextureByte4( LPDIRECT3DDEVICE9 pd3dDevice, int size, int numMipLevels = 1 );
	int addTexture( char* name, LPDIRECT3DDEVICE9 pd3dDevice );
	CubicTexture* retrieveTexture( int i );

	CubicTexture* operator[](int i){return m_cubicTextures[i];}

	void Cleanup();

private:

	std::vector< CubicTexture* > m_cubicTextures;
};

#endif