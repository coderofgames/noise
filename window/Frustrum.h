#ifndef FRUSTRUM_H
#define FRUSTRUM_H

#define	NEAR_PLANE		0
#define	FAR_PLANE		1
#define	LEFT_PLANE		2
#define	RIGHT_PLANE		3
#define	TOP_PLANE		4
#define	BOTTOM_PLANE    5

/*
*	Viewing frustrum class
*/
class ViewFrustrum
{
public:
	
	
	//	Constructor/Destructor

	ViewFrustrum(){}
	~ViewFrustrum(){}

	enum CORNER_INDEXES
	{
		NEAR_BOTTOM_LEFT = 0,
		NEAR_BOTTOM_RIGHT,
		NEAR_TOP_LEFT,
		NEAR_TOP_RIGHT,
		FAR_BOTTOM_LEFT,
		FAR_BOTTOM_RIGHT,
		FAR_TOP_LEFT,
		FAR_TOP_RIGHT
	};
	
	//	Create a frustrum given the view an projection matrices
	
	void Create( D3DXMATRIX &matView, D3DXMATRIX &matProj )
	{
		D3DXMATRIX mat;

		// Transforming the view frustum corners in world space by the view transform
		// then by the projection transform converts the frustum to a 'unit' cuboid with
		// x-range {-1 to +1}, y-range {-1 to +1) and z-range {0 to +1}.	
		// Thus;

		//  [frustum_corner_cuboid_space] = 
		//			[frustum_corner_world_space]*[matView]*[matProjection]
		//	
		// Therefore to obtain the viewing frustum corners from the 'unit' cuboid
		// multiply the coners of the 'unit' cuboid by the inverse of 
		// [matView]*[matProjection].

		// @ thanks to John Kawala for the above derivation

	    //MatMult( mat, matView, matProj );
		mat = matView * matProj;
	    D3DXMatrixInverse( &mat, 0, &mat );
		//InvertFull( mat, mat );
		


	    vFrustumCorner[NEAR_BOTTOM_LEFT]	= D3DXVECTOR3(-1.0f, -1.0f,  0.0f); // near-bottom-left
	    vFrustumCorner[NEAR_BOTTOM_RIGHT]	= D3DXVECTOR3( 1.0f, -1.0f,  0.0f); // near-bottom-right
	    vFrustumCorner[NEAR_TOP_LEFT]		= D3DXVECTOR3(-1.0f,  1.0f,  0.0f); // near-top-left
	    vFrustumCorner[NEAR_TOP_RIGHT]		= D3DXVECTOR3( 1.0f,  1.0f,  0.0f); // near-top-right
	    vFrustumCorner[FAR_BOTTOM_LEFT]		= D3DXVECTOR3(-1.0f, -1.0f,  1.0f); // far_bottom-left
	    vFrustumCorner[FAR_BOTTOM_RIGHT]	= D3DXVECTOR3( 1.0f, -1.0f,  1.0f); // far-bottom-right
	    vFrustumCorner[FAR_TOP_LEFT]		= D3DXVECTOR3(-1.0f,  1.0f,  1.0f); // far-top-left
	    vFrustumCorner[FAR_TOP_RIGHT]		= D3DXVECTOR3( 1.0f,  1.0f,  1.0f); // far-top-right

	    for( INT i = 0; i < 8; i++ )
	        D3DXVec3TransformCoord( &vFrustumCorner[i], 
			&vFrustumCorner[i], &mat );

	    D3DXPlaneFromPoints( &planes[NEAR_PLANE], 
			(D3DXVECTOR3*)&vFrustumCorner[0], 
	        (D3DXVECTOR3*)&vFrustumCorner[1], 
			(D3DXVECTOR3*)&vFrustumCorner[2] ); // Near
	    D3DXPlaneFromPoints( &planes[FAR_PLANE], 
			(D3DXVECTOR3*)&vFrustumCorner[6], 
	        (D3DXVECTOR3*)&vFrustumCorner[7], 
			(D3DXVECTOR3*)&vFrustumCorner[5] ); // Far
	    D3DXPlaneFromPoints( &planes[LEFT_PLANE], 
			(D3DXVECTOR3*)&vFrustumCorner[2], 
	        (D3DXVECTOR3*)&vFrustumCorner[6], 
			(D3DXVECTOR3*)&vFrustumCorner[4] ); // Left
	    D3DXPlaneFromPoints( &planes[RIGHT_PLANE],
			(D3DXVECTOR3*)&vFrustumCorner[7], 
	        (D3DXVECTOR3*)&vFrustumCorner[3], 
			(D3DXVECTOR3*)&vFrustumCorner[5] ); // Right
	    D3DXPlaneFromPoints( &planes[TOP_PLANE], 
			(D3DXVECTOR3*)&vFrustumCorner[2], 
	        (D3DXVECTOR3*)&vFrustumCorner[3],
			(D3DXVECTOR3*)&vFrustumCorner[6] ); // Top
	    D3DXPlaneFromPoints( &planes[BOTTOM_PLANE], 
			(D3DXVECTOR3*)&vFrustumCorner[1], 
	        (D3DXVECTOR3*)&vFrustumCorner[0],
			(D3DXVECTOR3*)&vFrustumCorner[4] ); // Bottom
	}

	bool IsPointInside( D3DXVECTOR3 *point )
	{
		float distance;
		for(int i=0; i<6; ++i)
		{
			distance = point->x* planes[i].a + point->y*planes[i].b + point->z*planes[i].c + planes[i].d;
			
			if( distance < -0.0001 )
				return true;
		}
				
		return false;
	}


	bool IsSphereInside( D3DXVECTOR3 *pos,  float radius )
	{
		float distance;
		for( int i = 0; i < 6; i++ )
		{
			distance = pos->x* planes[i].a + pos->y*planes[i].b + pos->z*planes[i].c + planes[i].d;

			if( distance < -radius )
				return false;
		}

		return true;
	}


	Ray*	CalculateFrustrumRays( )
	{
		rays[0].orig = vFrustumCorner[NEAR_BOTTOM_LEFT];
		rays[0].dir = vFrustumCorner[FAR_BOTTOM_LEFT] - vFrustumCorner[NEAR_BOTTOM_LEFT];

		rays[1].orig = vFrustumCorner[NEAR_BOTTOM_RIGHT];
		rays[1].dir = vFrustumCorner[FAR_BOTTOM_RIGHT] - vFrustumCorner[NEAR_BOTTOM_RIGHT];

		rays[2].orig = vFrustumCorner[NEAR_TOP_LEFT];
		rays[2].dir = vFrustumCorner[FAR_TOP_LEFT] - vFrustumCorner[NEAR_TOP_LEFT];

		rays[3].orig = vFrustumCorner[NEAR_TOP_RIGHT];
		rays[3].dir = vFrustumCorner[FAR_TOP_RIGHT] - vFrustumCorner[NEAR_TOP_RIGHT];


		for( int i = 0; i < 4; i++ )
			D3DXVec3Normalize( &rays[i].dir, &rays[i].dir );

		return &rays[0];
	}

	Ray& GetRay( int val ){ return  rays[val]; }

	D3DXPLANE planes[6];
	D3DXVECTOR3 vFrustumCorner[8];
	Ray rays[4];
};

#endif