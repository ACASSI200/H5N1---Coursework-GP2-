#pragma once

#include "MeshCollider.h"

CMeshCollider::CMeshCollider()
{
	m_NumVerts=0;
	m_ColliderType=Mesh;
	m_Verts=NULL;
}

CMeshCollider::~CMeshCollider()
{
	if (m_Verts)
	{
		delete [] m_Verts;
		m_Verts=NULL;
	}
}

void CMeshCollider::init()
{	
	if (!m_pPhysicsShape){
		//Vertices
		hkStridedVertices stridedVerts;
		//number of verts
		stridedVerts.m_numVertices = m_NumVerts;
		//the size of the single vertex
		stridedVerts.m_striding = 3*sizeof(float);
		//The actual vertices
		stridedVerts.m_vertices = m_Verts;

		// Creates a convex vertices shape from vertices.
		m_pPhysicsShape = new hkpConvexVerticesShape(stridedVerts);
	}
}