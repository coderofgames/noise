#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H


// note@ this class is temporary
class EffectManager	
	: public CareTaker, Singleton <EffectManager>
{
public:
	EffectManager();
	~EffectManager();

	int addEffect( char* name, LPDIRECT3DDEVICE9 pd3dDevice );
	LPD3DXEFFECT retrieveEffect( int i );

	LPD3DXEFFECT operator[](int i){return m_effects[i]->GetEffect();}

	void Cleanup();

private:

	std::vector< Effect* > m_effects;
};

#endif