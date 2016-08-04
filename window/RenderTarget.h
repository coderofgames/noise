//-----------------------------------------------------------------------------------------------
//
// RenderTarget.h : Defines the RenderTarget class
//-----------------------------------------------------------------------------------------------

#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include <d3d9.h>
#include <d3dx9.h>
#include "ResourceItem.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class RenderTarget : public ResourceItem, public InstanceCounter<RenderTarget> 
{
public:

	RenderTarget(void);
	~RenderTarget(void);

	void SetSize( Int32 x, Int32 y );
	Int32 Cleanup(void);
	inline LPDIRECT3DSURFACE9 GetRenderPixelSurface(void){ return m_pPixelSurf; }
	inline LPDIRECT3DSURFACE9 GetRenderDepthSurface(void){ return m_pDepthMapZ; }
	inline D3DVIEWPORT9 *GetViewPort(void){ return &m_viewPort; }
	inline LPDIRECT3DTEXTURE9 GetRenderedImage(void) { return m_pTexture; }
	HRESULT CreateRenderTarget( LPDIRECT3DDEVICE9 device, 
								D3DFORMAT pixel_format, 
								D3DFORMAT depth_format, 
								bool createDepthStencil );


	LPDIRECT3DTEXTURE9		m_pTexture;       // the color texture
	LPDIRECT3DTEXTURE9		m_pDepthTexture;		// the render Z-texture
	LPDIRECT3DSURFACE9		m_pPixelSurf;   // the color surface
	LPDIRECT3DSURFACE9		m_pDepthMapZ;		// the Z-Surface
	Int32 size_x;
	Int32 size_y;
	D3DVIEWPORT9 m_viewPort;   // note: should be in camera
	bool m_bMipped;
	bool hasDepthStencil;

	NO_COPY( RenderTarget )
};




#endif