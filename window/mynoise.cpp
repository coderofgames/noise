#include "stdafx.h"
#include "noisesky.h"
#include "mynoise.h"


void CreateTextureColor (utils::RendererImage& renderer)
{
	// Create a dark-stained wood palette (oak?)
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.00, utils::Color (189, 94, 4, 255));
	renderer.AddGradientPoint ( 0.50, utils::Color (144, 48, 6, 255));
	renderer.AddGradientPoint ( 1.00, utils::Color ( 60, 10, 8, 255));
}

void CreateWoodColor(utils::RendererImage& renderer)
{
	// Create a dark-stained wood palette (oak?)
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.00, utils::Color (189, 94, 4, 255));
	renderer.AddGradientPoint ( 0.50, utils::Color (144, 48, 6, 255));
	renderer.AddGradientPoint ( 1.00, utils::Color ( 60, 10, 8, 255));
}
void CreateSlimeColor(utils::RendererImage& renderer)
{
	// Create a green slime palette.  A dirt brown color is used for very low
	// values while green is used for the rest of the values.
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (160,  64,  42, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color ( 64, 192,  64, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color (128, 255, 128, 255));
}
void CreateJadeColor (utils::RendererImage& renderer)
{
	// Create a nice jade palette.
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.000, utils::Color ( 24, 146, 102, 255));
	renderer.AddGradientPoint ( 0.000, utils::Color ( 78, 154, 115, 255));
	renderer.AddGradientPoint ( 0.250, utils::Color (128, 204, 165, 255));
	renderer.AddGradientPoint ( 0.375, utils::Color ( 78, 154, 115, 255));
	renderer.AddGradientPoint ( 1.000, utils::Color ( 29, 135, 102, 255));
}
void CreateGraniteColor (utils::RendererImage& renderer)
{
	// Create a gray granite palette.  Black and pink appear at either ends of
	// the palette; those colors provide the charactistic black and pink flecks
	// in granite.
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0,   0, 255));
	renderer.AddGradientPoint (-0.9375, utils::Color (  0,   0,   0, 255));
	renderer.AddGradientPoint (-0.8750, utils::Color (216, 216, 242, 255));
	renderer.AddGradientPoint ( 0.0000, utils::Color (191, 191, 191, 255));
	renderer.AddGradientPoint ( 0.5000, utils::Color (210, 116, 125, 255));
	renderer.AddGradientPoint ( 0.7500, utils::Color (210, 113,  98, 255));
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 176, 192, 255));
}


void CreatePlanarTexture (const module::Module& noiseModule, bool seamless,
  int height, const char* filename, int num)
{
  // Map the output values from the noise module onto a plane.  This will
  // create a two-dimensional noise map which can be rendered as a flat
  // texture map.
	//utils::N
  utils::NoiseMapBuilderPlane plane;
  utils::NoiseMap noiseMap;
  plane.SetBounds (-1.0, 1.0, -1.0, 1.0);
  plane.SetDestSize (height, height);
  plane.SetSourceModule (noiseModule);
  plane.SetDestNoiseMap (noiseMap);
  plane.EnableSeamless (seamless);
  plane.Build ();

  RenderTexture (noiseMap, filename, num);

}

void CreateSphericalTexture (const module::Module& noiseModule, int height,
  const char* filename, int num)
{
  // Map the output values from the noise module onto a sphere.  This will
  // create a two-dimensional noise map which can be rendered as a spherical
  // texture map.
  utils::NoiseMapBuilderSphere sphere;
  utils::NoiseMap noiseMap;
  sphere.SetBounds (-90.0, 90.0, -180.0, 180.0); // degrees
  sphere.SetDestSize (height * 2, height);
  sphere.SetSourceModule (noiseModule);
  sphere.SetDestNoiseMap (noiseMap);
  sphere.Build ();

  RenderTexture (noiseMap, filename, num);
}

void   RenderTexture (const utils::NoiseMap& noiseMap, const char* filename, int num)
{
	if( num == 0 )RenderWoodTexture (noiseMap, filename);
	if( num == 1 )RenderSlimeTexture (noiseMap, filename);
	if( num == 2 )RenderJadeTexture (noiseMap, filename);
	if( num == 3 ) RenderGraniteTexture (noiseMap, filename);
}

void RenderWoodTexture (const utils::NoiseMap& noiseMap, const char* filename)
{
  // Create the color gradients for the texture.
  utils::RendererImage textureRenderer;
  CreateWoodColor (textureRenderer);

  // Set up us the texture renderer and pass the noise map to it.
  utils::Image destTexture;
  textureRenderer.SetSourceNoiseMap (noiseMap);
  textureRenderer.SetDestImage (destTexture);
  textureRenderer.EnableLight (false);

  // Render the texture.
  textureRenderer.Render ();

  // Write the texture as a Windows bitmap file (*.bmp).
  utils::WriterBMP textureWriter;
  textureWriter.SetSourceImage (destTexture);
  textureWriter.SetDestFilename (filename);
  textureWriter.WriteDestFile ();
}
void RenderSlimeTexture (const utils::NoiseMap& noiseMap, const char* filename)
{
  // Create the color gradients for the texture.
  utils::RendererImage textureRenderer;
  CreateSlimeColor (textureRenderer);

  // Set up us the texture renderer and pass the noise map to it.
  
  utils::Image destTexture;
  textureRenderer.SetSourceNoiseMap (noiseMap);
  textureRenderer.SetDestImage (destTexture);
  textureRenderer.EnableLight (false);

  // Render the texture.
  textureRenderer.Render ();

  // Write the texture as a Windows bitmap file (*.bmp).
  utils::WriterBMP textureWriter;
  textureWriter.SetSourceImage (destTexture);
  textureWriter.SetDestFilename (filename);
  textureWriter.WriteDestFile ();
}

void RenderJadeTexture (const utils::NoiseMap& noiseMap, const char* filename)
{
  // Create the color gradients for the texture.
  utils::RendererImage textureRenderer;
  CreateJadeColor (textureRenderer);

  // Set up us the texture renderer and pass the noise map to it.
  utils::Image destTexture;
  textureRenderer.SetSourceNoiseMap (noiseMap);
  textureRenderer.SetDestImage (destTexture);
  textureRenderer.EnableLight (false);

  // Render the texture.
  textureRenderer.Render ();

  // Write the texture as a Windows bitmap file (*.bmp).
  utils::WriterBMP textureWriter;
  textureWriter.SetSourceImage (destTexture);
  textureWriter.SetDestFilename (filename);
  textureWriter.WriteDestFile ();
}

void RenderGraniteTexture (const utils::NoiseMap& noiseMap, const char* filename)
{
  // Create the color gradients for the texture.
  utils::RendererImage textureRenderer;
  CreateGraniteColor (textureRenderer);

  // Set up us the texture renderer and pass the noise map to it.
  utils::Image destTexture;
  textureRenderer.SetSourceNoiseMap (noiseMap);
  textureRenderer.SetDestImage (destTexture);
  textureRenderer.EnableLight (true);
  textureRenderer.SetLightAzimuth (135.0);
  textureRenderer.SetLightElev (60.0);
  textureRenderer.SetLightContrast (2.0);
  textureRenderer.SetLightColor (utils::Color (255, 255, 255, 0));

  // Render the texture.
  textureRenderer.Render ();

  // Write the texture as a Windows bitmap file (*.bmp).
  utils::WriterBMP textureWriter;
  textureWriter.SetSourceImage (destTexture);
  textureWriter.SetDestFilename (filename);
  textureWriter.WriteDestFile ();
}

void texture_granite()
{
  // Primary granite texture.  This generates the "roughness" of the texture
  // when lit by a light source.
  module::Billow primaryGranite;
  primaryGranite.SetSeed (0);
  primaryGranite.SetFrequency (8.0);
  primaryGranite.SetPersistence (0.625);
  primaryGranite.SetLacunarity (2.18359375);
  primaryGranite.SetOctaveCount (6);
  primaryGranite.SetNoiseQuality (QUALITY_STD);

  // Use Voronoi polygons to produce the small grains for the granite texture.
  module::Voronoi baseGrains;
  baseGrains.SetSeed (1);
  baseGrains.SetFrequency (16.0);
  baseGrains.EnableDistance (true);

  // Scale the small grain values so that they may be added to the base
  // granite texture.  Voronoi polygons normally generate pits, so apply a
  // negative scaling factor to produce bumps instead.
  module::ScaleBias scaledGrains;
  scaledGrains.SetSourceModule (0, baseGrains);
  scaledGrains.SetScale (-0.5);
  scaledGrains.SetBias (0.0);

  // Combine the primary granite texture with the small grain texture.
  module::Add combinedGranite;
  combinedGranite.SetSourceModule (0, primaryGranite);
  combinedGranite.SetSourceModule (1, scaledGrains);

  // Finally, perturb the granite texture to add realism.
  module::Turbulence finalGranite;
  finalGranite.SetSourceModule (0, combinedGranite);
  finalGranite.SetSeed (2);
  finalGranite.SetFrequency (4.0);
  finalGranite.SetPower (1.0 / 8.0);
  finalGranite.SetRoughness (6);

  // Given the granite noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalGranite, false, TEXTURE_HEIGHT,
    "Resources/noise/granite_plane.bmp",3);
  CreatePlanarTexture (finalGranite, true, TEXTURE_HEIGHT,
    "Resources/noise/granite_seamless.bmp",3);
  CreateSphericalTexture (finalGranite, TEXTURE_HEIGHT,
    "Resources/noise/granite_sphere.bmp",3);

}
void texture_jade()
{
  // Primary jade texture.  The ridges from the ridged-multifractal module
  // produces the veins.
  module::RidgedMulti primaryJade;
  primaryJade.SetSeed (0);
  primaryJade.SetFrequency (2.0);
  primaryJade.SetLacunarity (2.20703125);
  primaryJade.SetOctaveCount (6);
  primaryJade.SetNoiseQuality (QUALITY_STD);

  // Base of the secondary jade texture.  The base texture uses concentric
  // cylinders aligned on the z axis, which will eventually be perturbed.
  module::Cylinders baseSecondaryJade;
  baseSecondaryJade.SetFrequency (2.0);

  // Rotate the base secondary jade texture so that the cylinders are not
  // aligned with any axis.  This produces more variation in the secondary
  // jade texture since the texture is parallel to the y-axis.
  module::RotatePoint rotatedBaseSecondaryJade;
  rotatedBaseSecondaryJade.SetSourceModule (0, baseSecondaryJade);
  rotatedBaseSecondaryJade.SetAngles (90.0, 25.0, 5.0);

  // Slightly perturb the secondary jade texture for more realism.
  module::Turbulence perturbedBaseSecondaryJade;
  perturbedBaseSecondaryJade.SetSourceModule (0, rotatedBaseSecondaryJade);
  perturbedBaseSecondaryJade.SetSeed (1);
  perturbedBaseSecondaryJade.SetFrequency (4.0);
  perturbedBaseSecondaryJade.SetPower (1.0 / 4.0);
  perturbedBaseSecondaryJade.SetRoughness (4);

  // Scale the secondary jade texture so it contributes a small part to the
  // final jade texture.
  module::ScaleBias secondaryJade;
  secondaryJade.SetSourceModule (0, perturbedBaseSecondaryJade);
  secondaryJade.SetScale (0.25);
  secondaryJade.SetBias (0.0);

  // Add the two jade textures together.  These two textures were produced
  // using different combinations of coherent noise, so the final texture will
  // have a lot of variation.
  module::Add combinedJade;
  combinedJade.SetSourceModule (0, primaryJade);
  combinedJade.SetSourceModule (1, secondaryJade);

  // Finally, perturb the combined jade textures to produce the final jade
  // texture.  A low roughness produces nice veins.
  module::Turbulence finalJade;
  finalJade.SetSourceModule (0, combinedJade);
  finalJade.SetSeed (2);
  finalJade.SetFrequency (4.0);
  finalJade.SetPower (1.0 / 16.0);
  finalJade.SetRoughness (2);

  // Given the jade noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalJade, false, TEXTURE_HEIGHT,
    "jadeplane.bmp",2);
  CreatePlanarTexture (finalJade, true, TEXTURE_HEIGHT,
    "jadeseamless.bmp",2);
  CreateSphericalTexture (finalJade, TEXTURE_HEIGHT,
    "jadesphere.bmp",2);
}


void texture_wood()
{
 // Base wood texture.  The base texture uses concentric cylinders aligned
  // on the z axis, like a log.
  module::Cylinders baseWood;
  baseWood.SetFrequency (16.0);

  // Perlin noise to use for the wood grain.
  module::Perlin woodGrainNoise;
  woodGrainNoise.SetSeed (0);
  woodGrainNoise.SetFrequency (48.0);
  woodGrainNoise.SetPersistence (0.5);
  woodGrainNoise.SetLacunarity (2.20703125);
  woodGrainNoise.SetOctaveCount (3);
  woodGrainNoise.SetNoiseQuality (QUALITY_STD);

  // Stretch the Perlin noise in the same direction as the center of the
  // log.  This produces a nice wood-grain texture.
  module::ScalePoint scaledBaseWoodGrain;
  scaledBaseWoodGrain.SetSourceModule (0, woodGrainNoise);
  scaledBaseWoodGrain.SetYScale (0.25);

  // Scale the wood-grain values so that they may be added to the base wood
  // texture.
  module::ScaleBias woodGrain;
  woodGrain.SetSourceModule (0, scaledBaseWoodGrain);
  woodGrain.SetScale (0.25);
  woodGrain.SetBias (0.125);

  // Add the wood grain texture to the base wood texture.
  module::Add combinedWood;
  combinedWood.SetSourceModule (0, baseWood);
  combinedWood.SetSourceModule (1, woodGrain);

  // Slightly perturb the wood texture for more realism.
  module::Turbulence perturbedWood;
  perturbedWood.SetSourceModule (0, combinedWood);
  perturbedWood.SetSeed (1);
  perturbedWood.SetFrequency (4.0);
  perturbedWood.SetPower (1.0 / 256.0);
  perturbedWood.SetRoughness (4);

  // Cut the wood texture a small distance from the center of the "log".
  module::TranslatePoint translatedWood;
  translatedWood.SetSourceModule (0, perturbedWood);
  translatedWood.SetZTranslation (1.48);

  // Cut the wood texture on an angle to produce a more interesting wood
  // texture.
  module::RotatePoint rotatedWood;
  rotatedWood.SetSourceModule (0, translatedWood);
  rotatedWood.SetAngles (84.0, 0.0, 0.0);

  // Finally, perturb the wood texture to produce the final texture.
  module::Turbulence finalWood;
  finalWood.SetSourceModule (0, rotatedWood);
  finalWood.SetSeed (2);
  finalWood.SetFrequency (2.0);
  finalWood.SetPower (1.0 / 64.0);
  finalWood.SetRoughness (4);

  // Given the wood noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalWood, false, TEXTURE_HEIGHT,
    "WoodPlane.bmp",0);
  CreatePlanarTexture (finalWood, true, TEXTURE_HEIGHT,
    "WoodSeamless.bmp",0);
  CreateSphericalTexture (finalWood, TEXTURE_HEIGHT,
    "WoodSphere.bmp",0);
}


void create_slime()
{
  // Large slime bubble texture.
  module::Billow largeSlime;
  largeSlime.SetSeed (0);
  largeSlime.SetFrequency (4.0);
  largeSlime.SetLacunarity (2.12109375);
  largeSlime.SetOctaveCount (1);
  largeSlime.SetNoiseQuality (QUALITY_BEST);

  // Base of the small slime bubble texture.  This texture will eventually
  // appear inside cracks in the large slime bubble texture.
  module::Billow smallSlimeBase;
  smallSlimeBase.SetSeed (1);
  smallSlimeBase.SetFrequency (24.0);
  smallSlimeBase.SetLacunarity (2.14453125);
  smallSlimeBase.SetOctaveCount (1);
  smallSlimeBase.SetNoiseQuality (QUALITY_BEST);

  // Scale and lower the small slime bubble values.
  module::ScaleBias smallSlime;
  smallSlime.SetSourceModule (0, smallSlimeBase);
  smallSlime.SetScale (0.5);
  smallSlime.SetBias (-0.5);

  // Create a map that specifies where the large and small slime bubble
  // textures will appear in the final texture map.
  module::RidgedMulti slimeMap;
  slimeMap.SetSeed (0);
  slimeMap.SetFrequency (2.0);
  slimeMap.SetLacunarity (2.20703125);
  slimeMap.SetOctaveCount (3);
  slimeMap.SetNoiseQuality (QUALITY_STD);

  // Choose between the large or small slime bubble textures depending on the
  // corresponding value from the slime map.  Choose the small slime bubble
  // texture if the slime map value is within a narrow range of values,
  // otherwise choose the large slime bubble texture.  The edge falloff is
  // non-zero so that there is a smooth transition between the two textures.
  module::Select slimeChooser;
  slimeChooser.SetSourceModule (0, largeSlime);
  slimeChooser.SetSourceModule (1, smallSlime);
  slimeChooser.SetControlModule (slimeMap);
  slimeChooser.SetBounds (-0.375, 0.375);
  slimeChooser.SetEdgeFalloff (0.5);

  // Finally, perturb the slime texture to add realism.
  module::Turbulence finalSlime;
  finalSlime.SetSourceModule (0, slimeChooser);
  finalSlime.SetSeed (2);
  finalSlime.SetFrequency (8.0);
  finalSlime.SetPower (1.0 / 32.0);
  finalSlime.SetRoughness (2);

  // Given the slime noise module, create a non-seamless texture map, a
  // seamless texture map, and a spherical texture map.
  CreatePlanarTexture (finalSlime, false, TEXTURE_HEIGHT,
    "SlimePlane.bmp",1);
  CreatePlanarTexture (finalSlime, true, TEXTURE_HEIGHT,
    "SlimeSeamless.bmp",1);
  CreateSphericalTexture (finalSlime, TEXTURE_HEIGHT,
    "SlimeSphere.bmp",1);
}


void create_perlin( int octaves )
{
  module::Perlin myModule;
  myModule.SetOctaveCount(octaves);

  utils::NoiseMap heightMap;
  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (myModule);
  heightMapBuilder.SetDestNoiseMap (heightMap);
  heightMapBuilder.SetDestSize (512, 512);
  heightMapBuilder.SetBounds (6.0, 10.0, 1.0, 5.0);
  heightMapBuilder.Build ();

  utils::RendererImage renderer;
  utils::Image image;
  renderer.SetSourceNoiseMap (heightMap);
  renderer.SetDestImage (image);
  renderer.ClearGradient ();
  renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
  renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
  renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
  renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
  renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
  renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
  renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
  renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow
  renderer.EnableLight ();
  renderer.SetLightContrast (3.0); // Triple the contrast
  renderer.SetLightBrightness (2.0); // Double the brightness
  renderer.Render ();

  utils::WriterBMP writer;
  writer.SetSourceImage (image);
  char destFileName[256];
  sprintf( destFileName, "resources/perlin_map%d.bmp", octaves );
  writer.SetDestFilename (destFileName);
  writer.WriteDestFile ();
}