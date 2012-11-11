#include "LightComponent.h"

CLightComponent::CLightComponent()
{
	m_DiffuseColour=D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_SpecularColour=D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_strName="LightComponent";
}

CLightComponent::~CLightComponent()
{
}

void CLightComponent::setDiffuseColour(D3DXCOLOR& colour)
{
	m_DiffuseColour=colour;
}

void CLightComponent::setSpecularColour(D3DXCOLOR& colour)
{
	m_SpecularColour=colour;
}

D3DXCOLOR& CLightComponent::getDiffuseColour()
{
	return m_DiffuseColour;
}

D3DXCOLOR& CLightComponent::getSpecularColour()
{
	return m_SpecularColour;
}