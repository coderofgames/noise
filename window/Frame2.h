#ifndef FRAME_H
#define FRAME_H

#define USEMATH_H
#ifdef USEMATH_H
	#include "3DMath.h"
#define Identity( a ) D3DXMatrixIdentity( &a )
/*#else
	#define Mat44 D3DXMATRIX
	#define Vec3 D3DXVECTOR3 
*/
#endif

//-----------------------------------------------------------------------------
// ReferenceFrame Structure
//-----------------------------------------------------------------------------
struct ReferenceFrame 
#ifdef _USED3D
	: public D3DXFRAME
#endif
{
public:

#ifndef _USED3D
	Mat44 TransformationMatrix; 
	char* Name;
	ReferenceFrame *pFrameSibling;
	ReferenceFrame *pFrameFirstChild;
#endif
	Mat44 finalTransformationMatrix; 

	Mat44	m_matRot;
	Vec3 m_trans;
	Vec3 m_scale;
	Vec3 m_rotation;
	Mat44 m_matTrans;
	ReferenceFrame *m_pRoot; 

	ReferenceFrame()
	{
		memset((void*)this, 0, sizeof(ReferenceFrame));

		Identity( finalTransformationMatrix );
		Identity( TransformationMatrix );
		Identity( m_matRot );
		//D3DXMatrixIdentity( &finalTransformationMatrix );
		//D3DXMatrixIdentity( &TransformationMatrix );
		//D3DXMatrixIdentity( &m_matRot );
		m_scale = Vec3(1.0f,1.0f,1.0f);
		//m_pRoot = NULL;
	}

	void SetName( char * name )
	{
		 Name= name;
	}

	void AddSibling( ReferenceFrame * sib )
	{
		if( pFrameSibling == NULL )
		{
#ifdef _USED3D
			pFrameSibling = (D3DXFRAME *)sib;
#else
			pFrameSibling = sib;
#endif
		}
		else
		{
			ReferenceFrame* temp = (ReferenceFrame *)pFrameSibling;

			while( temp->pFrameSibling )
				temp = (ReferenceFrame *)temp->pFrameSibling;
#ifdef _USED3D			
			temp->pFrameSibling = (D3DXFRAME *)sib;
#else
			temp->pFrameSibling = sib;
#endif
		}	
	}

	void AddSub( ReferenceFrame* sub )
	{
		if( !pFrameFirstChild )
		{
#ifdef _USED3D
			pFrameFirstChild = (D3DXFRAME *)sub;
#else
			pFrameFirstChild = sub;
#endif
			sub->m_pRoot = this;
		}
		else
		{
			ReferenceFrame* pfc = (ReferenceFrame *)pFrameFirstChild;
			pfc->AddSibling( sub );
			sub->m_pRoot = this;
		}
	}


	void UpdateHeirarchy()
	{
		// is this the root?
		// we cannot update a heirarchy from a sub-ReferenceFrame properly!
		if( !m_pRoot ) 
		{
			OrthoNormalizeRotation();
			UpdateLocalMatrix();

			// the m_matCombined is the m_matLocal	
			finalTransformationMatrix = *(Mat44*)&TransformationMatrix;
		}

		ReferenceFrame* temp = (ReferenceFrame *)pFrameFirstChild;

		while( temp )
		{
			temp->OrthoNormalizeRotation();
			temp->UpdateLocalMatrix();

			// compute the combined transformation matrix and then transform the heirarchy
			//D3DXMatrixMultiply( &temp->finalTransformationMatrix, &temp->TransformationMatrix, &finalTransformationMatrix );
			MatMult( temp->finalTransformationMatrix, temp->TransformationMatrix, finalTransformationMatrix );
			temp->UpdateHeirarchy();

			temp = (ReferenceFrame *)temp->pFrameSibling;
		}
	}

	void OrthoNormalizeRotation(){
		Vec3 right, up, at;

		right =  Vec3( m_matRot._11, m_matRot._12, m_matRot._13 );
		up =	 Vec3( m_matRot._21, m_matRot._22, m_matRot._23 );
		at =	 Vec3( m_matRot._31, m_matRot._32, m_matRot._33 );
		
		
		//D3DXVec3Normalize( &at, &at );
		at.Normalize();
		//D3DXVec3Cross( &right, &up, &at );
		up.CrossProduct( right, at );
		//D3DXVec3Normalize( &right, &right );
		right.Normalize();
		//D3DXVec3Cross( &up, &at, &right );
		at.CrossProduct(up, right);
		//D3DXVec3Normalize( &up, &up );
		up.Normalize();

		//D3DXMatrixIdentity( &m_matRot );
		Identity( m_matRot );

		SetRight( right );
		SetUp( up );
		SetAt( at );
	}

	void UpdateLocalMatrix()
	{
		Mat44 matScale;
		ScaleMatrix44( matScale, m_scale.x, m_scale.y, m_scale.z );
		//D3DXMatrixScaling( &matScale, m_scale.x, m_scale.y, m_scale.z );

		m_matTrans = TranslateMatrix44( m_trans.x, m_trans.y, m_trans.z );
		//D3DXMatrixTranslation( &m_matTrans, m_trans.x, m_trans.y, m_trans.z );

		//D3DXMatrixIdentity( &TransformationMatrix );
		Identity( TransformationMatrix );

		TransformationMatrix = matScale * m_matRot * m_matTrans ;
	}

	//-------------------------------------------------------------------------
	// Returns the ReferenceFrame's translation.
	//-------------------------------------------------------------------------
	Vec3 GetTranslation()
	{
		return Vec3( finalTransformationMatrix._41, 
							finalTransformationMatrix._42, 
							finalTransformationMatrix._43 );
	}

	void SetTranslation( Vec3 trans )
	{
		m_trans = trans;
	}

	void AddTranslation( Vec3 trans )
	{
		m_trans += trans;
	}

	Vec3 GetAt()
	{
		return Vec3( m_matRot._31, 
							m_matRot._32, 
							m_matRot._33 );
	}

	void SetAt(Vec3 &v)
	{
		m_matRot._31 = v.x; 
		m_matRot._32 = v.y; 
		m_matRot._33 = v.z;
	}

	Vec3 GetRight()
	{
		return Vec3( m_matRot._11, 
							m_matRot._12, 
							m_matRot._13 );
	}

	void SetRight(Vec3 &v)
	{
		m_matRot._11 = v.x; 
		m_matRot._12 = v.y; 
		m_matRot._13 = v.z;
	}

	Vec3 GetUp()
	{
		return Vec3( m_matRot._21, 
							m_matRot._22, 
							m_matRot._23 );
	}

	void SetUp(Vec3 &v)
	{
		m_matRot._21 = v.x; 
		m_matRot._22 = v.y; 
		m_matRot._23 = v.z;
	}

	void SetRotation( float x, float y, float z )
	{
		m_rotation = Vec3(x,y,z);
		Mat44 rotationX, rotationY;
		//D3DXMatrixRotationX( &rotationX, x );
		//D3DXMatrixRotationY( &rotationY, y );
		//D3DXMatrixRotationZ( &m_matRot, z );
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationX );
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationY );
		RotationMatrixX( rotationX, m_rotation.x );
		RotationMatrixY( rotationY, m_rotation.y );
		RotationMatrixZ( m_matRot, m_rotation.z );
		m_matRot = m_matRot * rotationX;
		m_matRot = m_matRot * rotationY;
	}

	void SetRotation( Vec3 rotation )
	{
		m_rotation = rotation;
		Mat44 rotationX, rotationY;
		/*D3DXMatrixRotationX( &rotationX, rotation.x );
		D3DXMatrixRotationY( &rotationY, rotation.y );
		D3DXMatrixRotationZ( &m_matRot, rotation.z );
		D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationX );
		D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationY );*/

		RotationMatrixX( rotationX, m_rotation.x );
		RotationMatrixY( rotationY, m_rotation.y );
		RotationMatrixZ( m_matRot, m_rotation.z );
		m_matRot = m_matRot * rotationX;
		m_matRot = m_matRot * rotationY;
	}

	void AddRotation( float x, float y, float z )
	{
		m_rotation += Vec3(x,y,z);
		Mat44 rotationX, rotationY;
		//D3DXMatrixRotationX( &rotationX, m_rotation.x );
		//D3DXMatrixRotationY( &rotationY, m_rotation.y );
		//D3DXMatrixRotationZ( &m_matRot, m_rotation.z );

		RotationMatrixX( rotationX, m_rotation.x );
		RotationMatrixY( rotationY, m_rotation.y );
		RotationMatrixZ( m_matRot, m_rotation.z );
		m_matRot = m_matRot * rotationX;
		m_matRot = m_matRot * rotationY;
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationX );
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationY );
	}

	void AddRotation( Vec3 rotation )
	{
		m_rotation += rotation;
		Mat44 rotationX, rotationY;
		//D3DXMatrixRotationX( &rotationX, m_rotation.x );
		//D3DXMatrixRotationY( &rotationY, m_rotation.y );
		//D3DXMatrixRotationZ( &m_matRot, m_rotation.z );
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationX );
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotationY );

		RotationMatrixX( rotationX, m_rotation.x );
		RotationMatrixY( rotationY, m_rotation.y );
		RotationMatrixZ( m_matRot, m_rotation.z );
		m_matRot = m_matRot * rotationX;
		m_matRot = m_matRot * rotationY;
	}

	void Pitch( float angle )
	{
		Mat44 rotation;
		Vec3 right = Vec3( m_matRot._11, m_matRot._12, m_matRot._13 );

		RotationMatrixAxis(rotation, right, angle );
		//D3DXMatrixRotationAxis( &rotation, &right, angle );
		
		// rotate
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotation );	
		m_matRot *= rotation;
	}

	void Yaw( float angle )
	{
		Mat44 rotation;
		Vec3 up = Vec3( m_matRot._21, m_matRot._22, m_matRot._23 );

		RotationMatrixAxis(rotation, up, angle );
		//D3DXMatrixRotationAxis( &rotation, &up, angle );
		
		// rotate
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotation );
		m_matRot *= rotation;
	}
	void Roll( float angle )
	{
		Mat44 rotation;
		Vec3 at = Vec3( m_matRot._31, m_matRot._32, m_matRot._33 );

		RotationMatrixAxis(rotation, at, angle );
		//D3DXMatrixRotationAxis( &rotation, &at, angle );
		
		// rotate
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotation );	
		m_matRot *= rotation;
	}

	void PitchWorld( float angle ) {
		Mat44 rotation;
		Vec3 right = Vec3( 1.0f, 0.0f, 0.0f );

		RotationMatrixAxis(rotation, right, angle );
		//D3DXMatrixRotationAxis( &rotation, &right, angle );
		
		// rotate
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotation );
		m_matRot *= rotation;
	}

	void YawWorld( float angle ) {

		Mat44 rotation;
		Vec3 up = Vec3( 0.0f, 1.0f, 0.0f );

		RotationMatrixAxis(rotation, up, angle );
		//D3DXMatrixRotationAxis( &rotation, &up, angle );
		
		// rotate
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotation );	
		m_matRot *= rotation;
	}

	void RollWorld( float angle ) {
		Mat44 rotation;
		Vec3 at = Vec3( 0.0f, 0.0f, 1.0f );

		//D3DXMatrixRotationAxis( &rotation, &at, angle );

		RotationMatrixAxis(rotation, at, angle );
		
		// rotate
		//D3DXMatrixMultiply( &m_matRot, &m_matRot, &rotation );
		m_matRot *= rotation;
	}

	void PointAt( Vec3 *lookat ){
		Vec3 dir = *lookat-m_trans;
		//D3DXVec3Normalize( &dir, &dir );
		dir.Normalize();
		Vec3 right;
		Vec3 YAXIS(0.0f,1.0f,0.0f);
		right = YAXIS.CrossProduct(right,dir);
		//D3DXVec3Cross( &right, &YAXIS, &dir );
		right.Normalize();
		//D3DXVec3Normalize( &right, &right );
		SetRight(right);
		SetUp(YAXIS);
		SetAt( dir) ;
	}


	Mat44 *GetCombinedMatrix(){ return &finalTransformationMatrix; }
	Mat44 *GetRotationMatrix(){ return &m_matRot; }
	void AssignOrientation( Mat44 &mat ){finalTransformationMatrix = mat;}
	void AssignOrientation( ReferenceFrame* newFrame )
	{
		this->finalTransformationMatrix = newFrame->finalTransformationMatrix;
	}

	Mat44 *GetTranslationMatrix(){return &m_matTrans;}


};

#endif