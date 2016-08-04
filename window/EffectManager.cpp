#include "stdafx.h"


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
EffectManager::EffectManager() : Singleton<EffectManager>(*this), CareTaker ()
{
	assert (Singleton <EffectManager>::GetInstance());
	
	CareTaker::SetName ("EffectManager");
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
EffectManager::~EffectManager()
{
	Cleanup();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int EffectManager::addEffect( char* name, LPDIRECT3DDEVICE9 pd3dDevice )
{
	int i = 0;
	for( i = 0; i < m_effects.size(); i++ )
	{
		if( !strcmp( m_effects[i]->GetName(), name ))
			return i;
	}

	Effect* effect = new Effect();
	effect->LoadEffect( name, pd3dDevice );
	m_effects.push_back(effect);

	return m_effects.size()-1;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
LPD3DXEFFECT EffectManager::retrieveEffect( int i )
{
	return m_effects[i]->GetEffect();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void EffectManager::Cleanup()
{
	for( int i = 0; i < m_effects.size(); i++ )
	{
		delete m_effects[i];

		m_effects[i] = NULL;

	}

	m_effects.clear();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------