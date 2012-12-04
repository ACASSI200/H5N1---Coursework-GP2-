#pragma once

#include "BaseCollider.h"
#include "Physics.h"

//Mesh Collider
class CMeshCollider:public CBaseCollider
{
public:
	CMeshCollider();
	~CMeshCollider();

	//set vertices
	void setVertices(float *pVerts, int numVerts)
	{
		//vertex array
		m_Verts=new float[3*numVerts];
		//number of verts
		m_NumVerts=numVerts;
	};

	//init method
	void init();
private:
	float * m_Verts;
	int m_NumVerts;
};