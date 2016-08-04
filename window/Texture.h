//-----------------------------------------------------------------------------------------------
//
// Texture.h : Defines the Texture class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "InstanceCounter.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class Texture : public ResourceItem, public InstanceCounter<Texture>
{
public:
	Texture(void);
	~Texture(void);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	virtual HRESULT LoadTexture(char* filename, LPDIRECT3DDEVICE9 pd3dDevice);
	void Cleanup(void);


	LPDIRECT3DTEXTURE9 m_pTexture;

	NO_COPY(Texture)
};

DECLARE_EXCEPTION(CTextureException)

//-----------------------------------------------------------------------------------------------
//	Purpose:	To provide a texture class with per-pixel accress
//	Status:		Not Tested
//	Notes:		This texture is in system memory (RAM)
//				and should be uploaded to the graphics memory
//-----------------------------------------------------------------------------------------------
class LockableTexture : public Texture
{
public:
	LockableTexture(void);
	~LockableTexture(void);

	HRESULT LoadTexture(char* filename, LPDIRECT3DDEVICE9 pd3dDevice);
	void Create(LPDIRECT3DDEVICE9 pd3dDevice, int width, int height);
	void Save(LPDIRECT3DDEVICE9 pd3dDevice, char* filename);

	bool IsLocked(void);
	void Lock(int mip_level);
	void Unlock(int mip_level);
	int GetWidth(void);
	int GetHeight(void);
	DWORD GetPixel (int x, int y);
	void SetPixel (int x, int y, DWORD color);


private:

	D3DLOCKED_RECT rect;
	D3DSURFACE_DESC pDesc;
	bool bLocked;
};

DECLARE_EXCEPTION(CLockableTextureException)

//-----------------------------------------------------------------------------------------------
//	Purpose:	To Create a volume texture object
//	Status:		Can be created and filled but not displayed
//-----------------------------------------------------------------------------------------------
class VolumeTexture: public ResourceItem, public InstanceCounter<VolumeTexture>
{
public:

	VolumeTexture();
	~VolumeTexture();

	LPDIRECT3DVOLUMETEXTURE9 GetTexture(void);
	//virtual void LoadTexture(char* filename, LPDIRECT3DDEVICE9 pd3dDevice);
	void Cleanup(void);

	HRESULT CreateVolumeTextureByte4(LPDIRECT3DDEVICE9 pd3dDevice, int width, int height, int depth);

	LPDIRECT3DVOLUMETEXTURE9 pVol;

	NO_COPY(VolumeTexture)

};

DECLARE_EXCEPTION(CVolumeTextureException)

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
VOID WINAPI VolumeFill (D3DXVECTOR4* pOut, const D3DXVECTOR3* pTexCoord, 
const D3DXVECTOR3* pTexelSize, LPVOID pData);



//-----------------------------------------------------------------------------------------------
//	Purpose:	To Create a CubeTexture object
//	Status:		not tested
//-----------------------------------------------------------------------------------------------
class CubicTexture : public ResourceItem, public InstanceCounter<CubicTexture>
{
public:

	CubicTexture();
	~CubicTexture();

	void Cleanup(void);

	LPDIRECT3DCUBETEXTURE9 GetTexture();
	LPDIRECT3DSURFACE9 GetCubeTextureSurface( int face, int level = 1);
	HRESULT CreateCubeTextureByte4(LPDIRECT3DDEVICE9 pd3dDevice, int size, int numMipLevels = 1) ;

	LPDIRECT3DCUBETEXTURE9 m_pCubeMap;

	NO_COPY(CubicTexture)
};

DECLARE_EXCEPTION(CCubicTextureException)