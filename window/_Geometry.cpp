
#include "stdafx.h"


//-----------------------------------------------------------------------------
// Constructors/Destructors
//-----------------------------------------------------------------------------
Plane::Plane()
{
	a = b = c = d = 0.0f;
}

//-----------------------------------------------------------------------------
// construct a plane from a normal
//-----------------------------------------------------------------------------
Plane::Plane( const Vec3& n )
{
	a = n.x; 
	b = n.y;
	c = n.z;
	d = 0.0f;
}

//-----------------------------------------------------------------------------
// construct a plane from a point and a normal
//-----------------------------------------------------------------------------
Plane::Plane( const Vec3& p, const Vec3& n )
{
	a = n.x; 
	b = n.y;
	c = n.z;
	d = -(a*p.x + b*p.y + c*p.z);   // temporary d value
}

//-----------------------------------------------------------------------------
// construct a plane from a clockwise ordering of points
//-----------------------------------------------------------------------------
Plane::Plane( const Vec3& p1, const Vec3& p2, const Vec3& p3 )
{
	Vec3 n1 = p2 - p1;
	Vec3 n2 = p3 - p1;

	Vec3 n3;
	Cross(n3,n2,n1);
	n3.Normalize();

	a = n3.x;
	b = n3.y;
	c = n3.z;
	d = -( p1.x*a + p1.y*b + p1.z*c ); // temporary d value, unless used by polyplane
}

#ifdef _USED3D
//-----------------------------------------------------------------------------
// construct a plane from a clockwise ordering of points
//-----------------------------------------------------------------------------
Plane::Plane( const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3 )
{
	Vec3 n1 = *(Vec3*)&(p2 - p1);
	Vec3 n2 = *(Vec3*)&(p3 - p1);

	Vec3 n3;
	Cross(n3,n2,n1);
	n3.Normalize();

	a = n3.x;
	b = n3.y;
	c = n3.z;
	d = -( p1.x*a + p1.y*b + p1.z*c ); // temporary d value, unless used by polyplane
}
#endif

//-----------------------------------------------------------------------------
// Destructors
//-----------------------------------------------------------------------------
Plane::~Plane()
{
}

//-----------------------------------------------------------------------------
//  Assuming the plane normal exists, a point on the plane is passed in, and
//  the point to test is evaluated against the plane equation
//	negative distances are behind the plane in
//	opposite direction to plane normal
//-----------------------------------------------------------------------------
inline float Plane::PlaneEquation( const Vec3 &ptest, const Vec3 &pplane )
{
	d = -( pplane.x*a + pplane.y*b + pplane.z*c );

	return a * ptest.x +
		   b * ptest.y +
		   c * ptest.z + d;;
}

//-----------------------------------------------------------------------------
// Find a ray intersection with the plane 
//-----------------------------------------------------------------------------
inline Vec3& Plane::RayIntersection( Vec3& orig, Vec3& rayDir) 
{
	rayDir.Normalize();

	float dot = a * rayDir.x +
				b * rayDir.y +
				c * rayDir.z;

	if(dot == 0.0f)     // change to <= and use Eps
		return orig;	//error line parallel to plane

	return orig;
//	return orig - rayDir * ( SignedDistance(orig) / dot );
}

//-----------------------------------------------------------------------------
// Dot product with a normal vector
//-----------------------------------------------------------------------------
inline float Plane::DotWithNormal( Vec3 &norm )
{
	return a * norm.x +
		   b * norm.y +
		   c * norm.z;
}

//-----------------------------------------------------------------------------
// Sets this planes values from points
//-----------------------------------------------------------------------------
void Plane::FromPoints( const Vec3& p1, const Vec3& p2, const Vec3& p3 )
{
	Vec3 n1 = p2 - p1;
	Vec3 n2 = p3 - p1;

	Vec3 n3;
	Cross(n3,n2,n1);
	n3.Normalize();

	a = n3.x;
	b = n3.y;
	c = n3.z;
	d = -( p1.x*a + p1.y*b + p1.z*c ); // temporary d value, unless used by polyplane
}

#ifdef _USED3D
//-----------------------------------------------------------------------------
// Dot with Normal
//-----------------------------------------------------------------------------
inline float Plane::DotWithNormal( D3DXVECTOR3 norm )
{
	return a * norm.x +
		   b * norm.y +
		   c * norm.z;
}
#endif


#ifdef _USED3D
//-----------------------------------------------------------------------------
// Calculate Signed Distance
//-----------------------------------------------------------------------------
inline float PolyPlane::SignedDistance( const D3DXVECTOR3 &point )
{
	return a * point.x +
		   b * point.y +
		   c * point.z + d;
}
#endif

//-----------------------------------------------------------------------------
//	negative distances are behind the plane in
//	opposite direction to plane normal
//-----------------------------------------------------------------------------
inline float PolyPlane::SignedDistance( const Vec3 &point )
{
	return a * point.x +
		   b * point.y +
		   c * point.z + d;
}

//-----------------------------------------------------------------------------
// Inline Methods
//-----------------------------------------------------------------------------
inline bool PolyPlane::InFrontOfPlane( const Vec3& point ) 
{
	return IsPositive( SignedDistance(point) );
}

//-----------------------------------------------------------------------------
// Inline Methods
//-----------------------------------------------------------------------------
inline bool PolyPlane::BehindPlane( const Vec3& point ) 
{
	return IsNegative( SignedDistance(point) );
}

//-----------------------------------------------------------------------------
// Inline Methods
//-----------------------------------------------------------------------------
inline bool PolyPlane::PointOnPlane(const Vec3& point) 
{
	return SignedDistance(point) == 0.0f; // should use eps
}

//-----------------------------------------------------------------------------
// Construct a Matrix from a plane
// note: this function is experimental
//-----------------------------------------------------------------------------
inline void PolyPlane::MatrixFromPlane( Mat44 &mat, Vec3 &p2 )
{
	Vec3 P;
	Vec3 N = Vec3( a, b, c );

	// find signed distance of zerovector
	float di = SignedDistance( ZERO_VECTOR );

	( di > epsilon ? 
	  P = N * (-d) :     // zero vector is in front of plane in same direction as normal.
	  P = N * (d) );     // zero vector is in opposite direction of normal

	Vec3 R = Vec3( p2 - P );
	Vec3 U;
	Cross( U, R, N );

	mat.Set( R, U, N, P );
}

//-----------------------------------------------------------------------------
// Construct a plane using a matrix
//-----------------------------------------------------------------------------
void PolyPlane::ConstructFromMatrix( Mat44 &mat )
{
	Vec3 N( mat._31 - mat._41, 
			   mat._32 - mat._42, 
			   mat._33 - mat._43 );

	N.Normalize();

	Vec3 P( mat._41, mat._42, mat._43 );

	a = N.x;
	b = N.y;
	c = N.z;
	d = -N.Dot(P);
}

