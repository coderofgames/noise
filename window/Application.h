//-----------------------------------------------------------------------------------------------
//
// Application.h : Defines the Application class
//-----------------------------------------------------------------------------------------------

#pragma once

class Window;
class Timer;


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class Application 
	: public CareTaker, Singleton <Application>
{
public:

	Application(void);
	~Application(void);

	Int32 Cleanup(void);

	Int32 Initialize(Window *wind);
	Int32 Update(void);
	Int32 Render(void);
	
	void UpdateInput();

	Quad *m_quads[16];
	LPD3DXSPRITE p_sprites[16];
	Renderer *renderer;
	Window *window;
	Camera *cam;
	POINT mouseCurr;
	POINT mousePrev;
	float motion_dx;
	float motion_dy;

	D3DXMATRIX matRot;
	D3DXMATRIX matRotationX;
	D3DXMATRIX matRotationY;

	D3DXMATRIX matTrans;

	bool m_bWireFrame;
	bool m_bUseEffect;

	LPD3DXMESH m_pMesh;
	int cubemap_indxs[6];
	int cbEffect;
	int cmap;

	Timer timer;

	int cloud0;
	int cloud1;
	int cloud2;
	int cloud3;
};

DECLARE_EXCEPTION( CApplicationException )

// an external linkage to the app singleton
extern Application *app;