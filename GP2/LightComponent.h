#pragma once

#include "IComponent.h"

#include <d3d10.h>
#include <d3dx10.h>

enum LightType
{
	None=0,
	Direction,
	Point,
	Spot,
};

class CLightComponent:public CBaseComponent
{
public:
	CLightComponent();
	virtual ~CLightComponent();

	void setDiffuseColour(float r,float g,float b,float a)
	{
		m_DiffuseColour=D3DXCOLOR(r,g,b,a);
	};
	void setSpecularColour(float r,float g,float b,float a)
	{
		m_SpecularColour=D3DXCOLOR(r,g,b,a);
	};

	void setLightType(LightType lightType)
	{
		m_LightType=lightType;
	};

	LightType getLightType()
	{
		return m_LightType;
	};

protected:
	D3DXCOLOR m_DiffuseColour;
	D3DXCOLOR m_SpecularColour;
	LightType m_LightType;
};