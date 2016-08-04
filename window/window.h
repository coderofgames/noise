//-----------------------------------------------------------------------------------------------
//
// window.h : Defines the window class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "resource.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class Window
{
public:

	
	HINSTANCE hInst;								// current instance
	TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
	HWND hwnd;
	BOOL m_bLButtonDown;
	BOOL m_bRButtonDown;
	BOOL m_bKeys[256];

	Window(void){}

	LRESULT				WndProc(HWND, UINT, WPARAM, LPARAM);
	WORD				MyRegisterClass(HINSTANCE hInstance);
	BOOL				InitInstance(HINSTANCE, Int32);


	NO_COPY( Window )
};


INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);