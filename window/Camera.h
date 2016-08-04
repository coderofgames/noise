#pragma once

#include "Frame.h"
#include "Frustrum.h"



class Camera : public ReferenceFrame
{
public:

	Camera(){
		fNear = 0.1;
		fFar = 290.0f;
		fFov = 90.0f;
	}

	Mat44& GetViewMatrix(){ return m_matView; }
	Mat44& GetProjMatrix(){return m_matProj; }
	ViewFrustrum& GetFrustrum(){ return m_frustrum; }

	void SetupCubemapProjectionMatrix()
	{
		// 90 degrees is PI/2 and aspect is 1, the other params are 
		// not important but should contain valid values
		SetupProjectionMatrix( D3DX_PI/2, 0.01, 100, 1 );
	}

	void SetupProjectionMatrix(float fov, float fnear, float ffar, float fAspect )
	{
		//fFov = D3DX_PI/4;
		fNear			= fnear;
		fFar			= ffar;
		fCameraAspect	= fAspect;//;
//		D3DXMatrixPerspectiveFovLH( &m_matProj, fov, fCameraAspect, fNear, fFar );
		m_matProj		= PerspectiveMatrix44( fov, fCameraAspect, fNear, fFar );
	}

	void UpdateViewMatrix()
	{
		this->OrthoNormalizeRotation();
		this->UpdateHeirarchy();

		m_vEye =	Vec3(finalTransformationMatrix._41,finalTransformationMatrix._42,finalTransformationMatrix._43);
		m_vLook =	Vec3(finalTransformationMatrix._31,finalTransformationMatrix._32,finalTransformationMatrix._33);
		m_vUp =		Vec3(finalTransformationMatrix._21,finalTransformationMatrix._22,finalTransformationMatrix._23);
		m_vRight =	Vec3(finalTransformationMatrix._11,finalTransformationMatrix._12,finalTransformationMatrix._13);

		//D3DXMatrixInverse( &m_matView, 0, &finalTransformationMatrix);

		InvertFull( m_matView, finalTransformationMatrix );
		m_frustrum.Create( m_matView, m_matProj );
	}


	Mat44 GetCubeMapViewMatrix( int face ) 
	{
		Vec3 vEyePt   = Vec3( 0.0f, 0.0f, 0.0f );
		Vec3 vLookDir;
		Vec3 vUpDir;

		switch( face ) {
			case 0:
				vLookDir = Vec3( 1.0f, 0.0f, 0.0f );
				vUpDir   = Vec3( 0.0f, 1.0f, 0.0f );
				break;
			case 1:
				vLookDir = Vec3(-1.0f, 0.0f, 0.0f );
				vUpDir   = Vec3( 0.0f, 1.0f, 0.0f );
				break;
			case 2:
				vLookDir = Vec3( 0.0f, 1.0f, 0.0f );
				vUpDir   = Vec3( 0.0f, 0.0f,-1.0f );
				break;
			case 3:
				vLookDir = Vec3( 0.0f,-1.0f, 0.0f );
				vUpDir   = Vec3( 0.0f, 0.0f, 1.0f );
				break;
			case 4:
				vLookDir = Vec3( 0.0f, 0.0f, 1.0f );
				vUpDir   = Vec3( 0.0f, 1.0f, 0.0f );
				break;
			case 5:
				vLookDir = Vec3( 0.0f, 0.0f,-1.0f );
				vUpDir   = Vec3( 0.0f, 1.0f, 0.0f );
				break;
		}

		// Set the view transform for this cubemap surface
		
		//D3DXMatrixLookAtLH( &m_matView, &vEyePt, &vLookDir, &vUpDir );
	
		m_matView = LookAtMatrix44( vEyePt, vUpDir, vLookDir );

		m_frustrum.Create( m_matView, m_matProj );

		return m_matView;
	}


	ViewFrustrum		m_frustrum;

	Vec3			m_vEye;
	Vec3			m_vLook;
	Vec3			m_vUp;
	Vec3			m_vRight;

	Mat44		m_matView;    
	Mat44		m_matProj;
	float		fFov;
	float		fCameraAspect;	
	float		fNear;
	float		fFar;

};
