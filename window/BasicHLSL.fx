//--------------------------------------------------------------------------------------
// File: BasicHLSL.fx
//
// The effect file for the BasicHLSL sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color
int g_nNumLights;

float3 g_LightDir[3];               // Light's direction in world space
float4 g_LightDiffuse[3];           // Light's diffuse color
float4 g_LightAmbient;              // Light's ambient color

texture g_MeshTexture;              // Color texture for mesh
texture g_MeshNormalMap;		// Normal map texture for mesh
texture g_MeshHeightMap;		// Height map texture for mesh
texture g_MeshSpecular;              // Gloss texture for mesh

texture g_MeshClouds0;              // cloud texture for mesh
texture g_MeshClouds1;              // cloud texture for mesh
texture g_MeshClouds2;              // cloud texture for mesh
texture g_MeshClouds3;              // cloud texture for mesh

float    g_fTime;			// App's time in seconds
float4x4 g_mWorld;			// World matrix for object
float4x4 g_mView;			// View matrix for scene
float4x4 g_mViewInv;			// inverse of View matrix for scene
float4x4 g_mWorldViewProjection;	 // World * View * Projection matrix



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler CloudTextureSampler0 = 
sampler_state
{
    Texture = <g_MeshClouds0>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler CloudTextureSampler1 = 
sampler_state
{
    Texture = <g_MeshClouds1>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler CloudTextureSampler2 = 
sampler_state
{
    Texture = <g_MeshClouds2>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler CloudTextureSampler3 = 
sampler_state
{
    Texture = <g_MeshClouds3>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
    float4 oEnvTex   : TEXCOORD1;   // cubemap coordinates
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform int nNumLights,
                         uniform bool bTexture,
                         uniform bool bAnimate )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
  
    float4 vAnimatedPos = vPos;
    
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(vAnimatedPos, g_mWorldViewProjection);
 
    // Transform the normal from object space to world space    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)

    Output.oEnvTex = float4(vNormalWorldSpace,1.0f);//mul( Output.oEnvTex, g_mViewInv );

   
    Output.Diffuse.rgb = float4(1.0f,1.0f,1.0f,1.0f);   
    Output.Diffuse.a = 1.0f; 
    
    // Just copy the texture coordinate through

    Output.TextureUV = vTexCoord0; 

    
    return Output;    
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse

        Output.RGBColor = texCUBE(MeshTextureSampler, In.oEnvTex);



	float4 cloud = //tex2D( CloudTextureSampler0, In.TextureUV )*
		tex2D( CloudTextureSampler1, In.TextureUV + float2( g_fTime/128, 0.0f) )
		*tex2D( CloudTextureSampler2, In.TextureUV + float2( g_fTime/64, 0.0f) )
		*tex2D( CloudTextureSampler3, In.TextureUV + float2( g_fTime/32, 0.0f) );
	
	Output.RGBColor += cloud;			


    

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSceneVS( 1, true, true );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture2Light
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSceneVS( 2, true, true );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSceneVS( 3, true, true );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneNoTexture
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSceneVS( 1, false, false );
        PixelShader  = compile ps_2_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)
    }
}
