//-----------------------------------------------------------------------------------------------
//
// Semaphore.h : Defines the Semaphore class
//-----------------------------------------------------------------------------------------------


#pragma once

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class Semaphore
{
public:

	Semaphore(void)
	{
		var = 0;
	}

	~Semaphore(void)
	{
	}

	Int32 Incr(void)
	{
		return var++;
	}

	void Decr(void)
	{
		var--;
	}

	Int32 GetValue(void)
	{
		return var;
	}
	
private:

	Int32 var;

};
