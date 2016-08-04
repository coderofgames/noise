#include "stdafx.h"

cSphereRectPatch::cSphereRectPatch()
{

}

cSphereRectPatch::~cSphereRectPatch()
{
	delete m_points;
}

Array2D< Vector3<float> >* cSphereRectPatch::GetPoints()
{
	return m_points;
}

void cSphereRectPatch::CreatePatch(int dir, int xdim, int ydim)
{
	
	xDim = xdim;
	yDim = ydim;

	m_points = new Array2D< Vector3<float> > ( xDim+1, yDim+1 );

	/*
		Vector3<float> **arr;
		arr = new Vector3<float>*[xDim+1];
		for( int i = 0; i < xDim+1; i++ )
		{
			*arr = new Vector3<float>[yDim+1];
		}
	*/

	//
	// for this noise map we need a 3D camera with a 90 degree projection matrix
	// for the cubemap. 
	// we then get a cubemap view matrix for the current direction, m_dir
	// and then extract the coordinates of the view frustrums near planes.
	// The grid representing the texture is then mapped to the near plane in
	// vector space, and rays are cast and intersected with a unit sphere
	// to give the volume noise coordinates.
	// this technique is preferable to a spherical projection because
	// the planets 6 faces can be displayed as separate heightmaps using optimal
	// display algorithms, whilst a spherical projection has a problem at the poles.
	//
	//
	// for this noise map we need a 3D camera with a 90 degree projection matrix
	// for the cubemap. 
	// we then get a cubemap view matrix for the current direction, m_dir
	// and then extract the coordinates of the view frustrums near planes.
	// The grid representing the texture is then mapped to the near plane in
	// vector space, and rays are cast and intersected with a unit sphere
	// to give the volume noise coordinates.
	// this technique is preferable to a spherical projection because
	// the planets 6 faces can be displayed as separate heightmaps using optimal
	// display algorithms, whilst a spherical projection has a problem at the poles.
	//
	Camera *cam = new Camera();
	cam->SetupCubemapProjectionMatrix();
	cam->GetCubeMapViewMatrix( dir );
	ViewFrustrum frustrum = cam->GetFrustrum();
 
	// extract the corners of the near plane from the frustrum
	Vector3<float> v_near_bottom_left = *(Vector3<float> *)&frustrum.vFrustumCorner[frustrum.NEAR_BOTTOM_LEFT];
	Vector3<float> v_near_bottom_right = *(Vector3<float> *)&frustrum.vFrustumCorner[frustrum.NEAR_BOTTOM_RIGHT];
	Vector3<float> v_near_top_right = *(Vector3<float> *)&frustrum.vFrustumCorner[frustrum.NEAR_TOP_RIGHT];
	Vector3<float> v_near_top_left = *(Vector3<float> *)&frustrum.vFrustumCorner[frustrum.NEAR_TOP_LEFT];

	// compute the distances between the near planes corners for 
	// the near planes horizontal and vertical extents
	Vector3<float> width_diff( v_near_bottom_left - v_near_bottom_right );
	Vector3<float> height_diff( v_near_bottom_left - v_near_top_left );

	float width_extent = width_diff.length();
	float height_extent = height_diff.length();

	// the near plane grid shall be computed using vector math,
	// so the direction is ...
	Vector3<float> w_dir(width_diff);
	Vector3<float> h_dir(height_diff);

	// this has to be normalized
	w_dir.normalize();
	h_dir.normalize();

	// the delta is the amount we add for each grid point
	float w_delta = width_extent / (float)(xDim);
	float h_delta = height_extent / (float)(yDim);

	// start the incrementing vector at the top right corner,
	// since this is the top left in texture space ...
	Vector3<float> curr_v(v_near_top_right);

	int x,y;
	for( x  = 0; x <= yDim; x++ )
	{
		y = 0;
		for( y = 0; y <= xDim; y++ )
		{
			Ray3<float> currRay;
			currRay.origin = Point3<float>(0.0f,0.0f,0.0f);
			// ...compute a texel correct ray direction
			// the dir is (curr_v - origin), but origin is 0
			currRay.direction = curr_v;

			Spheref sp1;
			sp1.center = Point3<float>(0.0f,0.0f,0.0f);
			sp1.radius = 1.f;
			float dist = 0.0f;
			

			if( testIntersection<float>(currRay,sp1,dist) )
			{
				Vector3<float> tmp( currRay.direction * dist );
				//(*m_points).AddElem( Vector3<float>(currRay.direction * dist), x, y );
				(*m_points)(x,y).x = tmp.x;
				(*m_points)(x,y).y = tmp.y;
				(*m_points)(x,y).z = tmp.z;

			}

			// increase the width pointer
			curr_v += (w_delta * w_dir);
		}

		// scan back to the left edge of the near plane
		curr_v -= y * w_delta * w_dir;

		// move down one height increment
		curr_v += h_delta * h_dir;
	}

	delete cam;
}