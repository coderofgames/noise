//-----------------------------------------------------------------------------------------------
//
// CareTaker.h : Defines the CareTaker class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "InstanceCounter.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class CareTaker : public InstanceCounter <CareTaker>
{
public:

	CareTaker (void);
	~CareTaker(void);
	
	void SetName (char* the_name);
	char *GetName(void);

private:

	bool						unclean;
	char*						name;

	NO_COPY( CareTaker )
};

DECLARE_EXCEPTION(CCareTakerException)