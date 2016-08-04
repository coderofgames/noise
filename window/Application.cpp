//-----------------------------------------------------------------------------------------------
//
// Application.cpp : Defines the Application class
//-----------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Application.h"


#include "../noise/src/noise.h"
#include "../noiseutils/noiseutils.h"
#include "mynoise.h"
#include "noisesky.h"
#include "noiseplanet.h"
using namespace noise;


noise::utils::RendererImage render_image;
noise::uint the_int;


extern utils::NoiseMapBuilderSphere planet;
extern utils::NoiseMap elevGrid;

extern int G_WIDTH;
extern int G_HEIGHT;

#ifdef GENERATE_MAPS
int CUBEMAP_FACE_SIZE = 1024;
#else
int CUBEMAP_FACE_SIZE = 2048;
#endif

const int nWIDTH	= G_WIDTH;
const int nHEIGHT	= G_HEIGHT;

int percentage_complete;
char c_mode[256];
int i_face;

int negX;

HeightPatch *heightPatch[6];

HeightPatchImpl* height_patch_impl[6];


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Application::Application() : Singleton<Application>(*this), CareTaker ()
{
	assert (Singleton <Application>::GetInstance());
	
	CareTaker::SetName ("Application");
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Application::~Application()
{
	Cleanup();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Application::Initialize (Window *wind)
{
	LOG ("FUNCTION: %s", TO_STRING(Initialize));
	
	try 
	{
		window = wind;
		renderer = new Renderer(wind);

		cam = new Camera();
		cam->SetupProjectionMatrix( PI/4, 0.01f, 10000.0f, 1.3333f );

		timer.Start();

		// first create a noise planet
		
#ifdef GENERATE_MAPS
		
		create_noise_planet(0,
			CUBEMAP_FACE_SIZE,
			CUBEMAP_FACE_SIZE);
		
#endif

		// should really use surface maps here since the diffuse maps
		// are lit by a light source, however the normal
		// maps are not working properly and the shader is tweaked
		// for the diffuse lit maps
		char *diffuse_bmps[6] = { "Resources/Diffuse/terrain_00",
							     "Resources/Diffuse/terrain_01",
							     "Resources/Diffuse/terrain_02",
							     "Resources/Diffuse/terrain_03",
							     "Resources/Diffuse/terrain_04",
							     "Resources/Diffuse/terrain_05"};
/*
		// these are not rendered in the current configuration
		char *diffuse_bmps[6] = { "Resources/surf/terrain_00surface",
							     "Resources/surf/terrain_01surface",
							     "Resources/surf/terrain_02surface",
							     "Resources/surf/terrain_03surface",
							     "Resources/surf/terrain_04surface",
							     "Resources/surf/terrain_05surface"};*/
		char *spec_bmps[6] = {	"Resources/spec/terrain_00spec",
								"Resources/spec/terrain_01spec",
								"Resources/spec/terrain_02spec",
								"Resources/spec/terrain_03spec",
								"Resources/spec/terrain_04spec",
								"Resources/spec/terrain_05spec"};	

		// now load up the height patches
		for( int i = 0; i < 6; i++ )
		{
			// 2^1 = 2
			// 2^2 = 4; 2^3 = 8; 2^4 = 16; 2^5 = 32 etc
			//
			heightPatch[i] = new HeightPatch(6, 1.0f , 0.0005f, 8.0f);
			SphericalHeightPatch shp;
			shp.Generate( heightPatch[i], i );
			height_patch_impl[i] = new HeightPatchImpl();
			height_patch_impl[i]->Initialize( heightPatch[i], renderer->GetDevice());

			height_patch_impl[i]->SetTextureID(renderer->AddTexture(diffuse_bmps[i]));
			height_patch_impl[i]->SetSpecularTextureID(renderer->AddTexture(spec_bmps[i]));
			delete heightPatch[i];
		}


		
		noise_sky(1);
		cloud0 = renderer->AddTexture("Resources/NoiseSkySphere1");
		noise_sky(2);
		cloud1 = renderer->AddTexture("Resources/NoiseSkySphere2");
		noise_sky(3);
		cloud2 = renderer->AddTexture("Resources/NoiseSkySphere3");
		noise_sky(4);
		cloud3 = renderer->AddTexture("Resources/NoiseSkySphere4");

		//cmap = renderer->CreateCubicTexture(512,1);

		m_bUseEffect = false;
		if( renderer->GetVertexShaderModel() >= 1 )
		{
			m_bUseEffect = true;
			cbEffect = renderer->AddEffect("Resources/BasicHLSL.fx");
		}

		/*D3DXLoadMeshFromX( L"Resources/Mesh/marble_sphere.x", 
							D3DXMESH_MANAGED, 
							renderer->GetDevice(), NULL, 
							0, NULL, 0, &m_pMesh );*/

		// these matrices have to be
		// set to identity
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixIdentity(&matRotationX);
		D3DXMatrixIdentity(&matRotationY);
		
		// initial translation is set to 5
		D3DXMatrixTranslation(&matTrans, 0,0,5);

	}
	catch (CRendererException e)
	{
		ERROR ("Caught Exception: %s", e.Text());
		return 0;
	}
	catch (...)
	{
		ERROR ("Unknown Exception in: %s::Initialize()", CareTaker::GetName());
		return 0;
	}

	return 1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Application::Update()
{
	try
	{
		UpdateInput();
		Render();
	}
	catch (CRendererException e)
	{
		ERROR ("Caught Exception %s: ", e.Text());
	}

	return 1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Application::Render()
{
	static enum R_MODE
	{
		R_GENERATING,
		R_DISPLAY,
	};

	static float rot = 0.01f;
	rot += 0.001f;


	int i = 0;

#ifdef GENERATE_MAPS
	static int curr_mode = R_GENERATING;
#else
	static int curr_mode = R_DISPLAY;
#endif

	if( i_face == 6 && curr_mode != R_DISPLAY) 
		curr_mode = R_DISPLAY;


	
	
	D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matIdent);
	//D3DXMatrixIdentity(&matRotationX);
	//D3DXMatrixIdentity(&matRotationY);

	renderer->GetDevice()->SetTransform(D3DTS_WORLD, &matIdent);
	renderer->GetDevice()->SetTransform(D3DTS_VIEW, &matIdent);
	renderer->GetDevice()->SetTransform(D3DTS_PROJECTION, &matIdent);

	switch( curr_mode )
	{
	case R_GENERATING:
		{	
			renderer->Clear(CLEAR_BLACK);
			renderer->Begin();
			if( i_face == 5 && percentage_complete > 98 )
				renderer->DrawText( FORMAT_TEXT("finished rendering...\nPlease Wait..."),0,0);
			else
				renderer->DrawText( FORMAT_TEXT("PLANET RENDERING\n MODE: %s \n %d PERCENT COMPLETE\n FACE: %d\n This may take a few minutes\n",c_mode,percentage_complete,i_face),0,0); 
			renderer->End();
			renderer->Flip();
		}
		break;
	case R_DISPLAY:
		{
			if( window->m_bLButtonDown )
			{
				if( motion_dx != 0 )
				{
					static float mx = 0.01f;
					D3DXVECTOR3 vecTrans(matTrans._41, matTrans._42, matTrans._43);
					float vecLength = D3DXVec3Length(&vecTrans);
					float scalar = vecLength / 5;
					mx -= motion_dx * scalar;
					D3DXMatrixRotationY( &matRotationX,  
										 D3DXToRadian((float)mx / 3.0f));
				}

				if( motion_dy != 0 )
				{
					static float my = 0.01f;
					D3DXVECTOR3 vecTrans(matTrans._41, matTrans._42, matTrans._43);
					float vecLength = D3DXVec3Length(&vecTrans);
					float scalar = vecLength / 5;
					my -= motion_dy * scalar;
					D3DXMatrixRotationX( &matRotationY,  
										 D3DXToRadian((float)my / 3.0f));
				}
			}
			else if( window->m_bRButtonDown )
			{
				if( motion_dy != 0 )
				{
					static float mt = 5.0f;
					mt += motion_dy*0.01f;
					D3DXMatrixTranslation(&matTrans, 0.0f,0.0f, mt);
				}
			}

			matRot = matRotationX*matRotationY ;

			static float elapsedTime = 0.0f;
			elapsedTime+=timer.Update();

			D3DXMATRIX mWorldViewProj =  matRot * matTrans *
				*(D3DXMATRIX*)&cam->GetViewMatrix() *
				*(D3DXMATRIX*)&cam->GetProjMatrix();

			D3DXMATRIX mViewInv;
			D3DXMatrixInverse(&mViewInv, 0, (D3DXMATRIX*)&cam->GetViewMatrix() );

			float inc_angle = 0.00246;
			static float currAngle = 0.0f;
			currAngle+=inc_angle;
			if( currAngle > 2*D3DX_PI)
			{
				currAngle = 0;
			}

			D3DXVECTOR4 g_LightDir[3];
			g_LightDir[0] = D3DXVECTOR4( 0.0, -0.5, -1.0f, 1.0f );
			g_LightDir[1] = D3DXVECTOR4( cos(currAngle), sin(currAngle), sin(currAngle), 1.5f );
			g_LightDir[2] = D3DXVECTOR4( 0.5, -0.5, 0.0f, 0.0f);

			D3DXVECTOR4 g_LightDiffuse[3];
			g_LightDiffuse[0] = D3DXVECTOR4( 0.0, 0.5, 1.0f, 1.0f );
			g_LightDiffuse[1] = D3DXVECTOR4( 1.0, 0.5, 0.0f, 1.0f );
			g_LightDiffuse[2] = D3DXVECTOR4( 0.0, 1.0, 0.0f, 1.0f );

			D3DXVECTOR4 g_LightAmbient = D3DXVECTOR4( 1.4f, 1.4f, 1.4f, 1.0f);

			D3DXVECTOR4 g_MaterialAmbientColor = D3DXVECTOR4( 1,1,1,1);     // Material's ambient color
			D3DXVECTOR4 g_MaterialDiffuseColor = D3DXVECTOR4( 1,1,1,1);      // Material's diffuse color
			int g_nNumLights = 3;

			if( m_bWireFrame )
			{
				renderer->GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			}
			else
			{
				renderer->GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
			}

			renderer->Clear(CLEAR_BLACK);
			renderer->Begin();

			if( m_bUseEffect )
			{
				LPD3DXEFFECT effect = renderer->GetEffect(cbEffect);
				
				effect->SetMatrix( "g_mWorld", &(matRot *matTrans) );
				effect->SetMatrix( "g_mView", (D3DXMATRIX*)&cam->GetViewMatrix() );
				effect->SetMatrix( "g_mViewInv", &mViewInv );
				effect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProj );

				effect->SetFloat( "g_fTime", elapsedTime );
			
				effect->SetVectorArray( "g_LightDir", g_LightDir, 3 );
				effect->SetVectorArray( "g_LightDiffuse", g_LightDiffuse, 3 );

				effect->SetVector( "g_MaterialAmbientColor", &g_MaterialAmbientColor );
				effect->SetVector( "g_MaterialDiffuseColor", &g_MaterialDiffuseColor );
			
				effect->SetValue( "g_nNumLights", (void*)&g_nNumLights, 4 );

				//effect->SetTexture( "g_MeshTexture", renderer->GetCubicTexture(cmap)->GetTexture());//renderer->GetTexture(0)->GetTexture());
				//effect->SetTexture( "g_MeshClouds0", renderer->GetTexture(cloud0)->GetTexture());//

				effect->SetMatrix( "matWorld", &(matRot *matTrans) );
				effect->SetMatrix( "matWorldViewProj", &mWorldViewProj );
				effect->SetVector("vecLightDir", &g_LightDir[1]);
				D3DXVECTOR4 vecEye( cam->GetTranslation().x,
								cam->GetTranslation().y,
								cam->GetTranslation().z, 
								0.0f );
				effect->SetVector("vecEye", &vecEye);


				
				effect->SetTechnique( "RenderSceneNoTexture" );

				if( renderer->GetVertexShaderModel() >= 2 )
					effect->SetTechnique( "RenderSceneCookTorrence" );

				unsigned int uPasses = 0;
				unsigned int upass = 0;
				effect->Begin(&uPasses,0);
				for( upass= 0; upass < uPasses; upass++ )
				{
					effect->BeginPass(upass);
					
					for( int h = 0; h < 6; h++ )
					{
						effect->SetTexture( "g_MeshTexture", renderer->GetTexture(height_patch_impl[h]->GetTextureID())->GetTexture());			
						if( renderer->GetVertexShaderModel() >= 2 )
							effect->SetTexture( "g_MeshSpecular", renderer->GetTexture(height_patch_impl[h]->GetSpecularTextureID())->GetTexture());	
						
						effect->CommitChanges();
						height_patch_impl[h]->Render(renderer->GetDevice());
					}
					effect->EndPass();
				}
				effect->End();
			}
			else
			{
				renderer->GetDevice()->SetTransform(D3DTS_WORLD, &(matRot *matTrans));
				renderer->GetDevice()->SetTransform(D3DTS_VIEW, &cam->GetViewMatrix());
				renderer->GetDevice()->SetTransform(D3DTS_PROJECTION, &cam->GetProjMatrix());
			
				for( int h = 0; h < 6; h++ )
				{
					renderer->GetDevice()->SetTexture( 0, renderer->GetTexture(height_patch_impl[h]->GetTextureID())->GetTexture());//renderer->GetTexture(0)->GetTexture());				
			
					height_patch_impl[h]->Render(renderer->GetDevice());
				}
			}

			renderer->DrawText( "PLANET RENDERING COMPLETE\n See Resources/Diffuse/ for textures",0,0); 
			renderer->End();
			renderer->Flip();
		}
		break;


	};
	

	return 1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 Application::Cleanup()
{
	LOG ("Application::Cleanup()");
	
	SAFE_DELETE(renderer);

	for( Int32 i = 0; i < 6; i++ )
	{
		delete height_patch_impl[i];
	}

	delete cam;

	return 1;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void Application::UpdateInput()
{

	// mouse

	POINT mouse_pos;
	GetCursorPos( &mouse_pos );
	ScreenToClient( window->hwnd, &mouse_pos );

    mouseCurr.x = mouse_pos.x;
    mouseCurr.y = mouse_pos.y;

	motion_dx = float(mouseCurr.x - mousePrev.x);
    motion_dy = float(mouseCurr.y - mousePrev.y);
	
    mousePrev.x = mouseCurr.x;
    mousePrev.y = mouseCurr.y;


	// keyboard

	m_bWireFrame = window->m_bKeys[ 'W' ];

	if( window->m_bKeys[ VK_ESCAPE ] )
	{
		// force quit if possible
		PostQuitMessage(0);
	}

}