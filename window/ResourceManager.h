//-----------------------------------------------------------------------------------------------
//
// ResourceManager.h : Defines the ResourceManager class
//-----------------------------------------------------------------------------------------------

#pragma once

#include "Singleton.h"
#include "ResourcePool.h"
#include "CareTaker.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class ResourceManager : public Singleton <ResourceManager>, public CareTaker
{
public:
	ResourceManager(void);
	~ResourceManager(void);

	Int32 addResourcePool( POOL location );

private:
	std::vector< ResourcePool* >	m_resourcePools;

	// there are only 4 resource pools
	ResourcePool		m_pDefaultPool;
	ResourcePool		m_pSystemMemPool;
	ResourcePool		m_pManagedPool;
	ResourcePool		m_pScratchPool;
};


DECLARE_EXCEPTION( CResourceManagerException )