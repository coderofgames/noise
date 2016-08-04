//-----------------------------------------------------------------------------------------------
//
// ResourceManager.cpp : Defines the ResourceManager class
//-----------------------------------------------------------------------------------------------

#include "StdAfx.h"
#include "ResourceManager.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(void) : Singleton <ResourceManager>(*this), CareTaker()
{
	assert (Singleton <ResourceManager>::GetInstance());

	CareTaker::SetName ("ResourceManager");
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager(void)
{
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Int32 ResourceManager::addResourcePool( POOL location )
{
	for( Int32 i = 0; i < (Int32)m_resourcePools.size(); i++ )
	{
		if( m_resourcePools[i]->GetPool() == location )
			return -1;
	}

	ResourcePool *pool = new ResourcePool();
	pool->SetPool( location );

	m_resourcePools.push_back(pool);

	return (Int32)m_resourcePools.size();
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------