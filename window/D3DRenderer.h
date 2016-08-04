//-----------------------------------------------------------------------------------------------
//
// D3DRenderer.h : Defines the D3D Renderer class
//-----------------------------------------------------------------------------------------------

#ifndef D3DRENDERER_H
#define D3DRENDERER_H

//#include "Window.h"

class Window;



#include "CareTaker.h"
#include "d3dfont.h"

#define CLEAR_BLACK		D3DCOLOR_COLORVALUE( 0.0f, 0.0f, 0.0f, 1.0f )
#define CLEAR_WHITE		D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f )

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class Renderer : public CareTaker
{
public:
	Renderer( Window *wind );
	~Renderer(void);

	Int32 Cleanup(void);

	void Clear(void);
	void Clear(D3DCOLOR color);
	void Begin(void);
	void End(void);
	void Flip(void);

	Int32 GetWidth(void);
	Int32 GetHeight(void);

	
	void CreateRenderTargets(void);
	void ScreenQuad( int width, int height );

	int LoadEffect( char* filename );

	void SetRenderSurface( LPDIRECT3DSURFACE9 pSurf ); 


	LPDIRECT3DDEVICE9 GetDevice(){ return m_pd3dDevice; }
	Texture *GetTexture(int id){ return textureManager[id]; }
	int AddTexture(char* name);

	CubicTexture *GetCubicTexture(int id){ return cubicTextureManager[id]; }
	int AddCubicTexture(char* name);
	int CreateCubicTexture(int size, int mipLevels = 1 );

	LPD3DXEFFECT GetEffect(int id){ return effectManager[id]; }
	int AddEffect(char* name);


	void EnableLighting(bool bLit);
	void SetDepthBias(float depth_bias);
	void SetSlopeScaleBias(float slope_scale_bias);
	void EnableAlphaBlending(bool bEnable);

	void SaveScreenRenderTarget();
	void ResetScreenRenderTarget();

	void DrawText( char* text, int x, int y );


	int GetVertexShaderModel(){ return vs_shader_model; }
	int GetPixelShaderModel(){ return ps_shader_model; }
	 
private:

	void GetDeviceCaps();

	LPDIRECT3D9					m_pD3D;
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	D3DDISPLAYMODE				m_d3ddm;
	D3DPRESENT_PARAMETERS		m_d3dpp;
	D3DCAPS9					m_d3dCaps;  

	std::vector< RenderTarget* >	renderTargets;
	TextureManager					textureManager;
	CubicTextureManager				cubicTextureManager;
	EffectManager					effectManager;

	IDirect3DVertexBuffer9			*pVB;


	// rendertarget stuff
	LPDIRECT3DSURFACE9 pScreenSurf;
	LPDIRECT3DSURFACE9 pDepthSurf;

	int vs_shader_model;
	int ps_shader_model;

	

	CD3DFont *m_font;

	NO_COPY( Renderer )
};

DECLARE_EXCEPTION(CRendererException)


#endif