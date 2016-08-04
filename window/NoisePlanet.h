#ifndef NOISE_PLANET_H
#define NOISE_PLANET_H

#include "mynoise.h"


struct LATLONG
{
	float latitude;
	float longitude;
};

using namespace noise;

namespace noise
{

  namespace utils
  {

		class NoiseMapBuilderCubeFace : public NoiseMapBuilder
		{
		public:

			NoiseMapBuilderCubeFace();

			virtual void Build ();

			void SetDir( int dir ){ m_dir = dir; }


		private:

			int m_dir;	// one of 6 directions

		};


		class NoiseMapBuilderCubeFaceTexelAligned : public NoiseMapBuilder
		{
		public:

			NoiseMapBuilderCubeFaceTexelAligned();

			virtual void Build ();

			void SetDir( int dir ){ m_dir = dir; }


		private:

			int m_dir;	// one of 6 directions

		};




		class WriterD3DTexture
		{
		public:

			WriterD3DTexture(){}


			void WriteDestFile ();

		   /// Returns the name of the file to write.
			///
			/// @returns The name of the file to write.
			std::string GetDestFilename () const
			{
				return m_destFilename;
			}

			/// Sets the name of the file to write.
			///
			/// @param filename The name of the file to write.
			///
			/// Call this method before calling the WriteDestFile() method.
			void SetDestFilename (const std::string& filename)
			{
				m_destFilename = filename;
			}

			/// Sets the image object that is written to the file.
			///
			/// @param sourceImage The image object to write.
			///
			/// This object only stores a pointer to an image object, so make sure
			/// this object exists before calling the WriteDestFile() method.
			void SetSourceImage (Image& sourceImage)
			{
				m_pSourceImage = &sourceImage;
			}

			/// Name of the file to write.
			std::string m_destFilename;

			/// A pointer to the image object that will be written to the file.
			Image* m_pSourceImage;

		};



  };
};

void create_noise_planet(int mode, int width = 512, int height = 512);


#endif