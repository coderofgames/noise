//-----------------------------------------------------------------------------------------------
//
// Singleton.h : Defines the Singleton class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "Exceptions.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
template <class T>
class Singleton
{
public:

	Singleton (T& other)
	{
		if (ms_pInstance)
		{
			throw CApplicationException ("Singleton Error");
		}
		else
		{
			ms_pInstance = &other;
		}
	}

	~Singleton(void)
	{
		ms_pInstance = NULL; 
	}

	static inline T *GetInstance() { return ms_pInstance; }

private:

	static T *ms_pInstance;
};

template <class T> T* Singleton<T>::ms_pInstance = 0;

