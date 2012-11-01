#include "DirectionLightComponent.h"

CDirectionalLightComponent::CDirectionalLightComponent()
{
	m_LightDirection=D3DXVECTOR3(0.0f,0.0f,-1.0f);
}

CDirectionalLightComponent::~CDirectionalLightComponent()
{
}

void CDirectionalLightComponent::setDirection(D3DXVECTOR3& lightDir)
{
	m_LightDirection=lightDir;
}

D3DXVECTOR3& CDirectionalLightComponent::getLightDirection()
{
	return m_LightDirection;
}