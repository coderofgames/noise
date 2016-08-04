//-----------------------------------------------------------------------------------------------
//
// ResourceItem.h : Defines the ResourceItem class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "InstanceCounter.h"


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class ResourceItem : public InstanceCounter <ResourceItem>
{
public:
	ResourceItem(void);
	~ResourceItem(void);

	char* GetName(void);
	void SetName(char* Name);

	Int32 GetID(void);
	Int32 GetNumResourceItems(void);

private:

	char					name[256];

	NO_COPY( ResourceItem )
};




					