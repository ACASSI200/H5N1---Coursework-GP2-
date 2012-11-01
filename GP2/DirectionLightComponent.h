#pragma once

#include "LightComponent.h"

class CDirectionalLightComponent:public CLightComponent
{
public:
	CDirectionalLightComponent();
	~CDirectionalLightComponent();

	void setDirection(D3DXVECTOR3& lightDir);
	D3DXVECTOR3& getLightDirection();
private:
	D3DXVECTOR3 m_LightDirection;

};