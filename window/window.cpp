//-----------------------------------------------------------------------------------------------
//
// window.cpp : Defines the entry poInt32 for the application.
//-----------------------------------------------------------------------------------------------

#include "stdafx.h"


// singleton pattern
Application *app;
Window win;

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int	nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	memset(&msg,0,sizeof(msg));

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, win.szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINDOW, win.szWindowClass, MAX_LOADSTRING);
	win.MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!win.InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	LOG("*******************************************************");

	try
	{
		app = new Application();
	}
	catch( CApplicationException e )
	{
		ERROR( "Startup Application Error: %s", e.Text() );
		goto E_END;
	}

	if( app->Initialize(&win) == 0 )
	{
		goto E_END;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOW));

	// Main message loop:
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			app->Update();
		}
    }

E_END:
	delete app;

	return (Int32) msg.wParam;
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return win.WndProc(hWnd,message,wParam,lParam);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
WORD Window::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= GlobalWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOW));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINDOW);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	return RegisterClassEx(&wcex);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
BOOL Window::InitInstance(HINSTANCE hInstance, Int32 nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1024, 768, NULL, NULL, hInstance, NULL);

   if (!hwnd)
   {
      return FALSE;
   }

   memset( (void*)m_bKeys, 0, sizeof(BYTE)*256 );

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   return TRUE;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Int32 wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(win.hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
    case WM_LBUTTONDOWN:
		m_bLButtonDown = true;
	break;
	case WM_LBUTTONUP:
		m_bLButtonDown = false;
		break;
	case WM_RBUTTONDOWN:
		m_bRButtonDown = true;
		break;
	case WM_RBUTTONUP:
		m_bRButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_KEYDOWN:
		wmId    = LOWORD(wParam);
		m_bKeys[ wmId ] = TRUE;
		break;
	case WM_KEYUP:
		wmId    = LOWORD(wParam);
		m_bKeys[ wmId ] = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
