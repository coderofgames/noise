#ifndef _GEOMETRY_H
#define _GEOMETRY_H



const Vec3 ZERO_VECTOR = Vec3( 0.0f,0.0f,0.0f );

class Point3D
{
public:
	float x, y, z;
};

#ifndef __PLANE
#define __PLANE

//-----------------------------------------------------------------------------
// Plane
//-----------------------------------------------------------------------------
class Plane
#ifdef _USED3D
: public D3DXPLANE
#endif 
{
public:
	
	//-----------------------------------------------------------------------------
	// Constructors/Destructors
	//-----------------------------------------------------------------------------

	Plane();

	Plane( const Vec3& n );

	Plane( const Vec3& p, const Vec3& n ); 

	Plane( const Vec3& p1, const Vec3& p2, const Vec3& p3 );

#ifdef _USED3D
	
	Plane( const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3 );

#endif

	~Plane();


	//-----------------------------------------------------------------------------
	// Inline Methods
	//-----------------------------------------------------------------------------

	inline float PlaneEquation( const Vec3 &ptest, const Vec3 &pplane );
	
	inline Vec3& RayIntersection( Vec3& orig, Vec3& rayDir);

	inline float DotWithNormal( Vec3 &norm );

	void FromPoints( const Vec3& p1, const Vec3& p2, const Vec3& p3 );

#ifdef _USED3D

	inline float DotWithNormal( D3DXVECTOR3 norm );

#endif



#ifndef _USED3D

	float a, b, c, d;

#endif

};
#endif // __PLANE



#ifndef __POLYPLANE
#define __POLYPLANE

//-----------------------------------------------------------------------------
// PolyPlane 
//-----------------------------------------------------------------------------
class PolyPlane : public Plane
{
public:

	// construction/destruction

	PolyPlane(){}
	
	~PolyPlane(){}


	// functions

	inline bool InFrontOfPlane( const Vec3& point );
	
	inline bool BehindPlane( const Vec3& point );
	
	inline bool PointOnPlane(const Vec3& point);

	inline void MatrixFromPlane( Mat44 &mat, Vec3 &p2 );

	void ConstructFromMatrix( Mat44 &mat );

	inline void PlaneAndPointToOrigin( Vec3 &pt_pln, Vec3& N );

	inline float SignedDistance( const Vec3 &point );

#ifdef _USED3D

	inline float SignedDistance( const D3DXVECTOR3 &point );

#endif
};


#endif // __POLYPLANE



#ifndef _LINE
#define _LINE
//-----------------------------------------------------------------------------
// Line 
//-----------------------------------------------------------------------------
class Line
{
public:
	
	Vec3 p1;
	Vec3 p2;
};
#endif

#define SetNorms( p, v )				p.norm.x = v.x;			\
										p.norm.y = v.y;			\
										p.norm.z = v.z;			\

#define SwapPositions( p, v )			p.pos.x = v.pos.x;		\
										p.pos.y = v.pos.y;		\
										p.pos.z = v.pos.z		\

#define SwapNorms( p, v )				p.norm.x = v.norm.x;    \
										p.norm.y = v.norm.y;    \
										p.norm.z = v.norm.z;    \

#define SwapTexCoord1( p, v )			p.t1.x = v.t1.x;		\
										p.t1.y = v.t1.y;		\

//p.pos.z = v.pos.z;

#define Swap( p,v)						SwapPositions( p,v );   \
										/*SwapNorms( p,v );*/   \
										SwapTexCoord1( p,v );	\




struct Edge
{
	Vertex verts[2];
	bool used_in_max;
};

// This is a triangle struct
struct Face
{
	Vertex verts[3];
	Vec3 norm;
	Edge edges[3];
};



class Triangle
{
public:

	Vec3 *points[3];
//	Edge *Edges[3];
	Vec3 norm;
};


#endif