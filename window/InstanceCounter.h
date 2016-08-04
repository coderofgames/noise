//-----------------------------------------------------------------------------------------------
//
// InstanceCounter.h : Defines the InstanceTracker and InstanceCounter classes 
//-----------------------------------------------------------------------------------------------

#pragma once

#include "Semaphore.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class InstanceTracker : public Semaphore
{
public:
	InstanceTracker(void):Semaphore(){}
	~InstanceTracker(void){}
};
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
template <class T> class InstanceCounter
{
public:
	InstanceCounter<T>(void)
	{
		num = count.Incr();
	}
	~InstanceCounter<T>(void)
	{
		count.Decr();
	}

	Int32 GetRefID(void)
	{
		return num;
	}

	Int32 GetNumRefs(void)
	{
		return count.GetValue();
	}

	Int32 num;
	static Semaphore count;
};

template <class T> Semaphore InstanceCounter<T>::count;