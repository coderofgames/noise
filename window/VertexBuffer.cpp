#include "stdafx.h"
#include "VertexBuffer.h"



//-----------------------------------------------------------------------------
// Name: A Vertex Buffer resembling the one from a very old version of 
//		 OGRE (Open Graphics Rendering Engine)
// Desc:
//-----------------------------------------------------------------------------
VertexBuffer::VertexBuffer(	int vertexSize, 
							int numVertices, 
							int usage, 
							LPDIRECT3DDEVICE9 pDev, 
							bool useSystemMemory)
{
	mVertexSize = vertexSize;
	m_numVertices = numVertices;
	mSizeInBytes = mVertexSize * numVertices;
	mUsage = usage;

    // Create the vertex buffer
    HRESULT hr = pDev->CreateVertexBuffer(
        mSizeInBytes, 
        usage, 
        0,
		useSystemMemory? D3DPOOL_SYSTEMMEM : D3DPOOL_DEFAULT, 
        &m_pVB,
        NULL);
    
	if( FAILED( hr ))
    {
		ERROR( "Failed to Create Buffer" );
    }
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
    SAFE_RELEASE(m_pVB);
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void* VertexBuffer::lock( int offset, int length, DWORD options )
{
    void* pBuf = 0;

	if( !( mUsage & D3DUSAGE_DYNAMIC ) && 
		 ( options & D3DLOCK_DISCARD || options & D3DLOCK_NOOVERWRITE ))
	{
		options = 0;
	}

    HRESULT hr = m_pVB->Lock( offset, length, &pBuf, options );

    if( FAILED( hr ))
    {
        ERROR( "Cannot lock vertex buffer" ); 
    }

    return pBuf;
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void VertexBuffer::unlock( void )
{
    m_pVB->Unlock();
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void VertexBuffer::readData( int offset, int length, void* pDest )
{
    void* pSrc = lock( offset, length, D3DLOCK_READONLY );
    memcpy( pDest, pSrc, length );
    unlock();
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
void VertexBuffer::writeData( int offset, int length, const void* pSource )
{
    void* pDst = lock( offset, length, D3DLOCK_DISCARD );
    memcpy( pDst, pSource, length );
    this->unlock();
}
