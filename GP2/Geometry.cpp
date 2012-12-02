#include "Geometry.h"

CGeometry::CGeometry()
{
	CBaseComponent::CBaseComponent();
	m_pVertexBuffer=NULL;
	m_pIndexBuffer=NULL;
	m_pInstanceBuffer=NULL;
	m_pVertexLayout=NULL;
}

CGeometry::~CGeometry()
{
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer=NULL;
	}
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer=NULL;
	}
	if (m_pInstanceBuffer)
	{
		m_pInstanceBuffer->Release();
		m_pInstanceBuffer=NULL;
	}
}

void CGeometry::init()
{
	//this will actually called after we have set all variables for the
	//index and the like, we can now safetly create the vertex layout
}