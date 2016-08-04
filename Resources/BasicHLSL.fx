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


float4x4 matWorldViewProj;	
float4x4 matWorld;	
float4 vecLightDir;
float4 vecEye;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
	Texture = <g_MeshTexture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    /*MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;*/
};

sampler GlossTextureSampler = 
sampler_state
{
	Texture = <g_MeshSpecular>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    /*MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;*/
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


// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUT_COOK
{
    float4 Pos  : POSITION;
    float3 Normal : TEXCOORD0;
    float4 Light : TEXCOORD1;
    float3 View : TEXCOORD2;
    float3 Half : TEXCOORD3;
	float2 TextureUV  : TEXCOORD4;
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

    
	Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV );



	float4 cloud = tex2D( CloudTextureSampler0, In.TextureUV );
		//tex2D( CloudTextureSampler1, In.TextureUV + float2( g_fTime/128, 0.0f) )
		//*tex2D( CloudTextureSampler2, In.TextureUV + float2( g_fTime/64, 0.0f) )
		//*tex2D( CloudTextureSampler3, In.TextureUV + float2( g_fTime/32, 0.0f) );
	
	//Output.RGBColor += 0.005*cloud;			


    

    return Output;
}



// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------
VS_OUTPUT_COOK VS(float4 Pos : POSITION, float3 Normal : NORMAL,float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_COOK Out = (VS_OUTPUT_COOK)0;      
    Out.Pos = mul(Pos, matWorldViewProj);	// transform Position    
    Out.Normal = mul(Normal, matWorld);

    float3 PosWorld = normalize(mul(Pos, matWorld));
    
    float3 Light = normalize(vecLightDir - PosWorld);
    float LightRange = 0.1;    
    Out.Light.xyz = Light;
    Out.Light.w = saturate(1 - dot(Light * LightRange, Light * LightRange));			// 1 - Attenuation    
    
    Out.View = normalize(vecEye - PosWorld);
    Out.Half = normalize(Out.Light.xyz + Out.View);

	Out.TextureUV = vTexCoord0; 
    
   return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(  float3 Normal : TEXCOORD0,
			float4 Light : TEXCOORD1,
			float3 View : TEXCOORD2,
			float3 Half : TEXCOORD3,
			float2 TextureUV : TEXCOORD4) : COLOR
{
	const float PI = 3.1415926535;
	const float m = 0.2f;										// roughness
	const float RI = 0.01f;										// Fresnel reflection index	
	
	const float Si = 0.3f;                                      // specular intensity
	const float Di = 0.9f;                                      // diffuse intenstiy
	const float Ai = 0.8f;										// ambient intensity
	const float dw = 7.0f;									    // unit solid angle
	
    // copper-like color RGB(184, 115, 51)
	float4 C = tex2D(MeshTextureSampler, TextureUV );//{1/256.0f * 184.0f, 1/256.0f * 115.0f, 1/256 * 51.0f, 1.0f}; 	
	
	float3 N = normalize(Normal);
	float3 L = normalize(Light.xyz);
	float3 V = normalize(View);
	float3 H = normalize(Half);
	
	float NH = saturate(dot(N, H));
	float VH = saturate(dot(V, H));
	float NV = saturate(dot(N, V));
	float NL = saturate(dot(L, N));
		
	//------------------------------
	// Compute Beckmann's distribution function
	//					  -[(1 - N.H^2) / m^2 * (N.H)^2]
	// D =     1       e^		
	//      m^2 N.H^4        
	// ß is the angle between N and H
	// m is the root-mean-square slope of the microfacets
	//------------------------------
	float NH2 = NH * NH;
	float m2 = m * m;
	float D = (1 / m2 * NH2 * NH2) * (exp(-((1 - NH2) / (m2 * NH2))));
	
	
	//------------------------------
	// Compute Fresnel term (Schlick's approximation) 
	// F = IR + (1-IR)*(1 - (N.V))^5
	// // IR + (1 - N.V)^5 * (1 - IR)
	//------------------------------
	float F = RI  +  (1 - RI) * pow((1 - NV), 5.0f);
	
	//------------------------------
	// Compute self shadowing term 
	// G = min(1, 2 * N.H * N.L, 2 * N.H * N.V)
	//                 V.H           V.H
	//------------------------------
//	float G = min(1.0f, min((2 * NH * NL) / VH, (2 * NH * NV) / VH));

	// optimized:
	float G = (2 * NH * min(NL, NV))/ VH;

	//------------------------------
	// Compute final Cook-Torrance specular term
	// Rs = (D * F * G) * 1 / PI * N.V * N.L
	//------------------------------
	float S = (F * D * G) / (PI * NL * NV);
	
	//------------------------------
	// Original: Ir = Ai * Ac + Ii(N.L)dw * (Si * Sc + Di * Dc)
	// My formula: Ir = Ia * C + N.L * sat(lidw * (Di * Dc * C + Si * Sc))
	// modified by David Nash to mask the ambient term with NL 
	//------------------------------
	float4 col = 2*NL *Ai * C + (NL * saturate(dw * ((Di * NL * C) + (Si * S)))) * Light.w* tex2D(GlossTextureSampler, TextureUV );

	return col ;

}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 1, true, true );
        PixelShader  = compile ps_3_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)

		CullMode = CCW;
	}
}

technique RenderSceneWithTexture2Light
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 2, true, true );
        PixelShader  = compile ps_3_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)

		CullMode = CCW;
	}
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 3, true, true );
        PixelShader  = compile ps_3_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)

		CullMode = CCW;
	}
}

technique RenderSceneNoTexture
{
    pass P0
    {          
        VertexShader = compile vs_3_0 RenderSceneVS( 1, false, false );
        PixelShader  = compile ps_3_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)

		CullMode = CCW;
	}
}


// -------------------------------------------------------------
// 
// -------------------------------------------------------------
technique RenderSceneCookTorrence
{
    pass P0
    {
        // compile shaders
        VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 PS();
    }
}

