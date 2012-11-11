#pragma once

#include <D3D10.h>
#include <D3DX10.h>

#include "IComponent.h"

class CLightComponent:public CBaseComponent
{
public:
	CLightComponent();
	virtual ~CLightComponent();

	void setDiffuseColour(D3DXCOLOR& colour);
	void setSpecularColour(D3DXCOLOR& colour);

	D3DXCOLOR& getDiffuseColour();
	D3DXCOLOR& getSpecularColour();

private:
	D3DXCOLOR m_DiffuseColour;
	D3DXCOLOR m_SpecularColour;

};
