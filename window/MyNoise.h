#ifndef MY_NOISE_H
#define MY_NOISE_H

#include "../noise/src/noise.h"
#include "../noiseutils/noiseutils.h"

using namespace noise;

const int TEXTURE_HEIGHT = 480;

// Creates the color gradients for the texture.
void CreateTextureColor (utils::RendererImage& renderer);
void CreateWoodColor(utils::RendererImage& renderer);
void CreateSlimeColor(utils::RendererImage& renderer);
void CreateJadeColor (utils::RendererImage& renderer);
void CreateGraniteColor (utils::RendererImage& renderer);

// Given a noise module, this function renders a flat square texture map and
// writes it to a Windows bitmap (*.bmp) file.  Because the texture map is
// square, its width is equal to its height.  The texture map can be seamless
// (tileable) or non-seamless.
void CreatePlanarTexture (const module::Module& noiseModule, bool seamless,
  int height, const char* filename, int num);

// Given a noise module, this function renders a spherical texture map and
// writes it to a Windows bitmap (*.bmp) file.  The texture map's width is
// double its height.
void CreateSphericalTexture (const module::Module& noiseModule, int height,
  const char* filename, int num);

// Given a noise map, this function renders a texture map and writes it to a
// Windows bitmap (*.bmp) file.
void RenderTexture (const utils::NoiseMap& noiseMap, const char* filename, int num);
void RenderWoodTexture (const utils::NoiseMap& noiseMap, const char* filename);
void RenderSlimeTexture (const utils::NoiseMap& noiseMap, const char* filename);
void RenderJadeTexture (const utils::NoiseMap& noiseMap, const char* filename);
void RenderGraniteTexture (const utils::NoiseMap& noiseMap, const char* filename);

void texture_granite();
void texture_jade();
void texture_wood();
void create_slime();

void create_perlin( int ocatves );

#endif