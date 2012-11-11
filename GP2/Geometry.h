#pragma once

//Header files
#include "Vertex.h"

#include <D3D10.h>
#include <D3DX10.h>

#include <vector>

using namespace std;

//Geometry class, this holds a vertex buffer and index buffer
class CGeometry
{
public:
	CGeometry(ID3D10Device *pDevice);
	~CGeometry();

	void createBuffers();

	void addVertex(Vertex vert);
	void addIndex(int index);


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
	void createVertexBuffer();
	void createIndexBuffer();

private:
	//The index and vertex buffer
	ID3D10Buffer *m_pVertexBuffer;
	ID3D10Buffer *m_pIndexBuffer;
	//Vector of the vertices, basically a growable array
	vector<Vertex> m_Vertices;
	//Vector of the indices
	vector<int> m_Indices;

	ID3D10Device * m_pD3D10Device;
};