//-----------------------------------------------------------------------------------------------
//
// Debug.h : Defines the OutputMessage namespace
//-----------------------------------------------------------------------------------------------

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "stdafx.h"
#include <tchar.h>


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
namespace OutputMessage
{
	extern char output[1024];
	
	void Format(char* buffer, const char* text, va_list ArgList);
	
	WCHAR * ToWchar(char *inpt, WCHAR *strout);
	char* Tochars (WCHAR* inpt,char* text);
	void DebugPrint (char *format,...);
	void MessageBoxPrint (char *format,...);
	char* Format (char* format,...);
	void Log (char *format,...);
	void Message( std::string str );
	
};
