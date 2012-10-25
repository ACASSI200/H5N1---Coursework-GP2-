#pragma once

//Header files
#include "IComponent.h"
#include "Vertex.h"

#include <D3D10.h>
#include <D3DX10.h>

#include <vector>

using namespace std;

//Geometry class, this holds a vertex buffer and index buffer
class CGeometryComponent:public CBaseComponent
{
public:
	CGeometryComponent();
	~CGeometryComponent();

	void init();

	void addVertex(Vertex vert);
	void addIndex(int index);

	void createVertexBuffer();
	void createIndexBuffer();

	void bindBuffers();

	int getNumberOfIndices()
	{
		return m_Indices.size();
	};

	int getNumberOfVertices()
	{
		return m_Vertices.size();
	};
private:
	//The index and vertex buffer
	ID3D10Buffer *m_pVertexBuffer;
	ID3D10Buffer *m_pIndexBuffer;
	//Vector of the vertices, basically a growable array
	vector<Vertex> m_Vertices;
	//Vector of the indices
	vector<int> m_Indices;
};