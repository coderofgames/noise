//-----------------------------------------------------------------------------------------------
//
// ResourcePool.h : Defines the ResourcePool class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "ResourceItem.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
enum POOL
{
	POOL_DEFAULT,
	POOL_SYSTEM,
	POOL_MANAGED,
	POOL_SCRATCH
};
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class ResourcePool
{
public:
	ResourcePool(void);
	~ResourcePool(void);

	Int32	AddResourceItem(ResourceItem* item);
	Int32	GetResourceCount(void);
	void	SetPool(POOL pool);
	POOL	GetPool(void);

private:

	POOL							m_pool;
	std::vector< ResourceItem* >	items;

	NO_COPY( ResourcePool )
};


