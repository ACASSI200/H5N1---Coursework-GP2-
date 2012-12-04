#pragma once

#include "LightComponent.h"

class CDirectionalLightComponent:public CLightComponent
{
public:
	CDirectionalLightComponent()
	{
		CBaseComponent::CBaseComponent();
		setDirection(0.0f,0.0f,-1.0f);
		m_strName="DirectionLightComponent";
		m_LightType=Direction;
	};

	~CDirectionalLightComponent()
	{
	};

	void setDirection(float x,float y,float z)
	{
		m_vecDirection=D3DXVECTOR3(x,y,z);
	};

	D3DXVECTOR3 getDirection()
	{
		return m_vecDirection;
	};

private:
	D3DXVECTOR3 m_vecDirection;
};