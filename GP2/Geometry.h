#pragma once

#include "IComponent.h"

#include <D3D10.h>
#include <D3DX10.h>

class CGeometry:public CBaseComponent
{
public:
	CGeometry();
	~CGeometry();

	const string& getName()
	{
		return "Geometry";
	};
	void init();
private:
	ID3D10Buffer *m_pVertexBuffer;
	ID3D10Buffer *m_pIndexBuffer;
	ID3D10Buffer *m_pInstanceBuffer;
	ID3D10InputLayout *m_pVertexLayout;
};