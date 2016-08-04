//-----------------------------------------------------------------------------------------------
//
// D3DRenderer.cpp : Defines the Renderer class
//-----------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "d3dRenderer.h"
#include "VertexTypes.h"


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Renderer::Renderer(Window *win) : CareTaker()
{
	CareTaker::SetName ("Renderer");

	m_pD3D          = NULL;
	m_pd3dDevice    = NULL;
	HRESULT hr		= S_OK;

	m_pD3D = Direct3DCreate9 (D3D_SDK_VERSION);

	if (!m_pD3D)
	{
		throw CRendererException( 
			FORMAT_TEXT ("cannot create d3d: %s, FILE %s, LINE %d", 
						 "Renderer::Renderer(Window *win)",
						 __FILE__, __LINE__)); 

		return; // have to exit app here, otherwise I will crash on CreateDevice()
	}
	else
	{
		LOG ("Created D3D9 in routine %s", CareTaker::GetName());
	}
	
	m_pD3D->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &m_d3ddm);
	ZeroMemory (&m_d3dpp, sizeof(m_d3dpp));
    
	m_d3dpp.Windowed               = TRUE;
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat       = D3DFMT_A8R8G8B8;//d3ddm.Format;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.BackBufferWidth = 1024;
	m_d3dpp.BackBufferHeight = 768;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	//m_d3dpp.MultiSampleType = D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_4_SAMPLES;
	hr = m_pD3D->CreateDevice (D3DADAPTER_DEFAULT, 
							   D3DDEVTYPE_HAL,
							   win->hwnd, 
							   D3DCREATE_HARDWARE_VERTEXPROCESSING,
							   &m_d3dpp, 
							   &m_pd3dDevice);

	if (FAILED(hr))
	{
		throw CRendererException( 
			FORMAT_TEXT("cannot create device: %s, FILE %s, LINE %d", 
						"Renderer::Renderer(Window *win)",
						__FILE__, __LINE__)); 
	}
	else
	{
		LOG ("RENDERER INITIALIZED");
	}

	GetDeviceCaps();

	pVB = NULL;

	//this->CreateRenderTargets();
	//this->CreateTextures();

	m_font = new CD3DFont(L"Arial",15);
	m_font->InitDeviceObjects( m_pd3dDevice );
	m_font->RestoreDeviceObjects();
	
	CreateVertexDeclerations(m_pd3dDevice);

	this->EnableLighting(false);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Renderer::~Renderer(void)
{
	LOG("Renderer::~Renderer()");
	Cleanup();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Renderer::Cleanup(void)
{
	static Int32 count = 0;

	LOG( "Renderer::Cleanup() %d ",count++ );

	for( Int32 i = 0; i < (Int32)renderTargets.size(); i++ )
	{
		SAFE_DELETE( renderTargets[i] );
	}

	renderTargets.clear();

	m_font->InvalidateDeviceObjects();
	m_font->DeleteDeviceObjects();

	SAFE_DELETE( m_font );


	SAFE_RELEASE( pVB );

	if( m_pd3dDevice != NULL )
		m_pd3dDevice->Release();

	if( m_pD3D != NULL )
		m_pD3D->Release();
	
	return 1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::Clear(void) 
{
	m_pd3dDevice->Clear( 0, 
						 NULL, 
						 D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
						 D3DCOLOR_COLORVALUE( 0.0f, 0.0f, 0.0f, 1.0f ), 
						 1.0f, 
						 0 );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::Clear(D3DCOLOR color)
{
	m_pd3dDevice->Clear( 0, 
						 NULL, 
						 D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
						 color, 
						 1.0f, 
						 0 );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::Begin(void) 
{
	m_pd3dDevice->BeginScene();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::End(void) 
{
	m_pd3dDevice->EndScene();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::Flip(void) 
{
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Renderer::GetWidth(void)
{
	return m_d3dpp.BackBufferWidth;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Renderer::GetHeight(void)
{
	return m_d3dpp.BackBufferHeight;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::EnableLighting( bool bLit ) 
{
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, bLit );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::SetDepthBias(float depth_bias) 
{
    m_pd3dDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depth_bias);
    
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::SetSlopeScaleBias(float slope_scale_bias) 
{
	m_pd3dDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&slope_scale_bias);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::EnableAlphaBlending(bool bEnable) 
{
	if( bEnable ) 
	{
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	} 
	else 
	{
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::SetRenderSurface( LPDIRECT3DSURFACE9 pSurf )
{
	m_pd3dDevice->SetRenderTarget( 0, pSurf );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::CreateRenderTargets(void)
{

}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int Renderer::AddTexture(char* name)
{
	return textureManager.addTexture(name, m_pd3dDevice);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int Renderer::AddCubicTexture( char* name )
{
	return cubicTextureManager.addTexture( name,  m_pd3dDevice );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int Renderer::CreateCubicTexture( int size, int mipLevels )
{
	return cubicTextureManager.CreateCubeTextureByte4( m_pd3dDevice, size, mipLevels);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int Renderer::AddEffect(char* name)
{
	return effectManager.addEffect( name, m_pd3dDevice );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::DrawText( char* text, int x, int y )
{

	EnableAlphaBlending(true);
	RECT rectDest;
	SetRect(&rectDest,10, 10,0,0);

	WCHAR message[256];

	m_font->DrawText(rectDest.left-rectDest.right, 
					 rectDest.top-rectDest.bottom,
					 D3DCOLOR_XRGB(255,255,255),
					 OutputMessage::ToWchar(text,message));

	EnableAlphaBlending(false);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::ScreenQuad( int width, int height )
{
	static bool first_time = true;
    HRESULT hr;
		
	if( first_time )
	{
		hr = m_pd3dDevice->CreateVertexBuffer( sizeof(PPVERT) * 4,
												D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
												0,
												D3DPOOL_DEFAULT,
												&pVB,
												NULL );

		if( FAILED(hr) )
			throw new CRendererException("Cannot create screen quad buffer");

		first_time = false;
	}

	PPVERT quad[4] =
	{
		// x				y				z		 rhw		tu1	  tv1			tu2	  tv2
		{ -0.5f,			-0.5f,			1.0f,	 0.0f,		0.0f, 0.0f,			0.0f, 0.0f },
		{ width-0.5f,		-0.5,			1.0f,	 0.0f,		1.0f, 0.0f,			1.0f, 0.0f },
		{ -0.5,             height-0.5f,	1.0f,	 0.0f,		0.0f, 1.0f,			0.0f, 1.0f },
		{ width-0.5f,		height-0.5f,	1.0f,	 0.0f,		1.0f, 1.0f,			1.0f, 1.0f }
	};

	// Fill in the vertex buffer
	LPVOID pVBData;
	if( SUCCEEDED( pVB->Lock( 0, 0, &pVBData, D3DLOCK_DISCARD ) ) )
	{
		CopyMemory( pVBData, quad, sizeof(quad) );
		pVB->Unlock();
	}

	m_pd3dDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX2);
	m_pd3dDevice->SetStreamSource( 0, pVB, 0, sizeof(PPVERT) );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


//	pVB->Release();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::SaveScreenRenderTarget() 
{

	m_pd3dDevice->GetRenderTarget(0, &pScreenSurf );
	m_pd3dDevice->GetDepthStencilSurface( &pDepthSurf );
	//m_pd3dDevice->GetViewport( &screen->m_viewPort );
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::ResetScreenRenderTarget() 
{
	m_pd3dDevice->SetRenderTarget( 0, pScreenSurf );  // just one texture
	m_pd3dDevice->SetDepthStencilSurface( pDepthSurf );  
	//device->SetViewport( screen->GetViewPort() );


	pScreenSurf->Release();
	pDepthSurf->Release();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Renderer::GetDeviceCaps()
{
	m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );

	if( m_d3dCaps.VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		this->vs_shader_model = 1;
	}
	if( m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(2,0) )
	{
		this->vs_shader_model = 2;
	}
	if( m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(3,0) )
	{
		this->vs_shader_model = 3;
	}

	if( m_d3dCaps.VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		this->ps_shader_model = 1;
	}
	if( m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(2,0) )
	{
		this->ps_shader_model = 2;
	}
	if( m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(3,0) )
	{
		this->ps_shader_model = 3;
	}
}