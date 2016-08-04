//-----------------------------------------------------------------------------------------------
//
// NumericTools.h : Defines the UTIL namespace
//-----------------------------------------------------------------------------------------------

#pragma once

#include <float.h>

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
namespace UTIL
{

template <class T> bool IsPositive( T& val )
{
	return val >= 0;
}

template <class T> bool IsNegative( T& val )
{
	return val < 0;
}

template <class T> T& Min( T& a, T& b )
{
	return a < b ? a : b;
}

template <class T> T& Max( T& a, T& b )
{
	return a < b ? b : a;
}

template <class T> T& Abs( T& a )
{
	return sqrt(a*a);
}

template <class T> T& Pow( T& a, Int32 b )
{
	T res = a;
	for( int i = 1; i < b; i++ )
	{
		res *= a;
	}
	return res;
}


template <class T> void Swap( T& a, T& b )
{
	T c = a; 
	a = b; 
	b = c;
}

template <class T> T& Mod( T& a, T& b )
{
	return (a%b);
}

template <class T> T& Clamp( T& a, T& b, T& c )
{
	if( a <= b )
		return b;

	if( a >= c )
		return c;

	return a;
}

}; // namespace UTIL

// more to come ...