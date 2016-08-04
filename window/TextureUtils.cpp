#include "stdafx.h"
#include <d3d9Types.h>
#include "NoisePlanet.h"


// had to add this here for the RGBA_MAKE macro
void noise::utils::WriterD3DTexture::WriteDestFile ()
{
	if (m_pSourceImage == NULL) {
		throw noise::ExceptionInvalidParam ();
	}

	int width  = m_pSourceImage->GetWidth  ();
	int height = m_pSourceImage->GetHeight ();

	// This buffer holds one horizontal line in the destination file.
	noise::uint8* pLineBuffer = NULL;

	LockableTexture *pTex = new LockableTexture();
	//pTex->LoadTexture((char*)m_destFilename.c_str(), app->renderer->GetDevice());
	pTex->Create(app->renderer->GetDevice(), width, height );

	pTex->Lock(0);

	// Build and write each horizontal line to the file.
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++)
		{
			noise::utils::Color* pSource = m_pSourceImage->GetSlabPtr (x, height-1-y);
			//pTex->SetPixel( x, y, *(DWORD*)pSource);//
			pTex->SetPixel( x, y, (DWORD)RGBA_MAKE( (DWORD)pSource->red, (DWORD)pSource->green, (DWORD)pSource->blue, (DWORD)pSource->alpha) );		
		}
	}

	pTex->Unlock(0);

	pTex->Save( app->renderer->GetDevice(), (char*)m_destFilename.c_str() );

	delete pTex;
}

UInt32 GetColor( LockableTexture *tex, int px, int py, int color )
{
	bool bUnlock = false;
	if( !tex->IsLocked() )
	{
		tex->Lock(0);
		bUnlock = true; // sort of thing that can cause a bug
	}
	
	DWORD col1 = tex->GetPixel(px, py); 
	UInt32 c;
	switch( color )
	{
	case 0:
		c = RGBA_GETALPHA( col1 );
		break;
	case 1:
		c = RGBA_GETRED( col1 );
		break;
	case 2:
		c = RGBA_GETGREEN( col1 );
		break;
	case 3:
		c = RGBA_GETBLUE( col1 );
		break;
	} 

	if( bUnlock )
		tex->Unlock(0);

	return c;
}

void SubtractTexture(LockableTexture* result, LockableTexture *source1, LockableTexture *source2)
{
	source1->Lock(0);
	source2->Lock(0);
	result->Lock(0);

	for( Int32 x = 0; x < (Int32)source1->GetWidth(); x++ )
	{
		for( Int32 y = 0; y < (Int32)source1->GetHeight(); y++ )
		{
			DWORD col1 = source1->GetPixel(x, y); 
			DWORD col2 = source2->GetPixel(x, y);

			D3DCOLORVALUE c1;
			UInt32 lower = 0;
			UInt32 upper = 255;

			UInt32 a = RGBA_GETALPHA( col1 )	- RGBA_GETALPHA( col2 );
			UInt32 r = RGBA_GETRED( col1 )		- RGBA_GETRED( col2 );
			UInt32 g = RGBA_GETGREEN( col1 )	- RGBA_GETGREEN( col2 );
			UInt32 b = RGBA_GETBLUE( col1 )		- RGBA_GETBLUE( col2 );

			c1.a = (float)UTIL::Clamp<UInt32>( a, lower, upper );
			c1.r = (float)UTIL::Clamp<UInt32>( r, lower, upper );
			c1.g = (float)UTIL::Clamp<UInt32>( g, lower, upper );
			c1.b = (float)UTIL::Clamp<UInt32>( b, lower, upper );
			
			result->SetPixel( x, y, (DWORD)RGBA_MAKE( (DWORD)c1.r, (DWORD)c1.g, (DWORD)c1.b, (DWORD)c1.a ) );
		}
	}

	source1->Unlock(0);
	source2->Unlock(0);
	result->Unlock(0);
}