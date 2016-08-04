//-----------------------------------------------------------------------------------------------
//
// RenderTarget.cpp : Defines the RenderTarget class
//-----------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "RenderTarget.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
RenderTarget::RenderTarget() : ResourceItem (),InstanceCounter<RenderTarget>()
{
	ResourceItem::SetName("RenderTarget");

	m_pTexture		= NULL;
	m_pPixelSurf	= NULL;
	m_pDepthMapZ	= NULL;

	m_bMipped		= false;
	hasDepthStencil = false;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
RenderTarget::~RenderTarget()
{
	LOG( "DESTROYING RENDERTARGET: %d on line: %d", InstanceCounter<RenderTarget>::GetRefID(), __LINE__ );

	try {
		Cleanup();
	}
	catch(...)
	{
		ERROR("ERROR CLEANING UP DESTROYING RENDERTARGET: %d", InstanceCounter<RenderTarget>::GetRefID() );
	}
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void RenderTarget::SetSize( Int32 x, Int32 y )
{
	size_x = x;
	size_y = y;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
HRESULT RenderTarget::CreateRenderTarget(LPDIRECT3DDEVICE9 device, D3DFORMAT pixel_format, D3DFORMAT depth_format, bool createDepthStencil )
{
	HRESULT hr;

	// Setup viewport
	// Note: this should be in the camera
	m_viewPort.X = 0;
	m_viewPort.Y = 0;
	m_viewPort.Width  = size_x;
	m_viewPort.Height = size_y;
	m_viewPort.MinZ = 0.0f;
	m_viewPort.MaxZ = 1.0f;

	D3DFORMAT zFormat = D3DFMT_D24S8;

	hr = device->CreateTexture(
		size_x, 
		size_y, 
		1,						// set to 1 by default
		D3DUSAGE_RENDERTARGET, 
		pixel_format, 
		D3DPOOL_DEFAULT, 
		&m_pTexture, 
		NULL );

	if( FAILED(hr) )
	{
		ERROR( "Failed to create Texture" ); 
		return E_FAIL;
	}

	hasDepthStencil = createDepthStencil;

	if( !createDepthStencil ) 
	{
		return S_OK;
	}
		
	hr = device->CreateTexture(
		size_x, 
		size_y, 
		1,						// set to 1 by default
		D3DUSAGE_DEPTHSTENCIL, 
		zFormat, 
		D3DPOOL_DEFAULT, 
		&m_pDepthTexture, 
		NULL );

	if( FAILED(hr) )
	{
		ERROR( "Failed to create depth stencil" );
		return E_FAIL;
	}

	hr =  m_pTexture->GetSurfaceLevel( 0, &m_pPixelSurf );

	if( FAILED(hr) )
	{
		ERROR( "Failed to obtain pixel surface" );
		return E_FAIL;
	}

	hr =  m_pDepthTexture->GetSurfaceLevel( 0, &m_pDepthMapZ );

	if( FAILED(hr) )
	{
		ERROR( "Failed to obtain depth stencil surface" );
		return E_FAIL;
	}

	return hr;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 RenderTarget::Cleanup()
{
	//LOG("RenderTarget::Cleanup()");
	
	SAFE_RELEASE_LOG (m_pTexture, InstanceCounter<RenderTarget>::GetRefID());
	SAFE_RELEASE_LOG (m_pPixelSurf, InstanceCounter<RenderTarget>::GetRefID());
	
	if( this->hasDepthStencil )
	{
		SAFE_RELEASE_LOG (m_pDepthMapZ, InstanceCounter<RenderTarget>::GetRefID());
		SAFE_RELEASE_LOG (m_pDepthTexture, InstanceCounter<RenderTarget>::GetRefID());
	}

	return 1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------