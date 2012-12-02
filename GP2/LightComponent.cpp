#pragma once

#include "LightComponent.h"

CLightComponent::CLightComponent()
{
	setDiffuseColour(0.8f,0.8f,0.8f,1.0f);
	setSpecularColour(1.0f,1.0f,1.0f,1.0f);
	m_LightType=None;
	m_strName="LightComponent";
}

CLightComponent::~CLightComponent()
{

}