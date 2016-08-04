#ifndef EFFECT_H
#define EFFECT_H

class Effect : public ResourceItem, public InstanceCounter<Effect>
{
public:

	Effect();
	~Effect();

	LPD3DXEFFECT GetEffect(){ return m_pEffect; }
	void LoadEffect( char* name, LPDIRECT3DDEVICE9 pd3dDevice );

	LPD3DXEFFECT m_pEffect;
};





void InitEffect( char* filename, LPDIRECT3DDEVICE9 device, LPD3DXEFFECT *effect );





#endif