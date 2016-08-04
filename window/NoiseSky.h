#ifndef NOISE_SKY_H
#define NOISE_SKY_H

#include "mynoise.h"

//const int TEXTURE_HEIGHT = 480;

// Given two noise modules, this function renders two flat square texture maps
// and combines them to form a Windows bitmap (*.bmp) file.  Because the
// texture map is square, its width is equal to its height.  The final texture
// map can be seamless (tileable) or non-seamless.
void CreatePlanarTexture (const module::Module& lowerNoiseModule,
  const module::Module& upperNoiseModule, bool seamless, int height,
  const char* filename);

// Given two noise modules, this function renders two spherical texture maps
// and combines them to form a Windows bitmap (*.bmp) file.  The texture map's
// width is double its height.
void CreateSphericalTexture (const module::Module& lowerNoiseModule,
  const module::Module& upperNoiseModule, int height, const char* filename);

// Given two noise maps, this function renders two texture maps and combines
// them to form a Windows bitmap (*.bmp) file.
void RenderTexture (const utils::NoiseMap& lowerNoiseMap,
  const utils::NoiseMap& upperNoiseMap, const char* filename);

utils::Image* CreateSphericalImage (const module::Module& lowerNoiseModule, 
									const module::Module& upperNoiseModule, 
									int height);

utils::Image* RenderImage(const utils::NoiseMap& lowerNoiseMap, 
						  const utils::NoiseMap& upperNoiseMap);

void noise_sky();
void create_clouds( int octave_count );
void noise_sky(int cloud_octaves);


#endif