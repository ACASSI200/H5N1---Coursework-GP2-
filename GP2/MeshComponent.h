#pragma once

#include "IComponent.h"
#include "Geometry.h"

#include <vector>

using namespace std;

class CMeshComponent:public CBaseComponent
{
public:
	CMeshComponent();
	~CMeshComponent();

	void addSubset(CGeometry *pGeom);
	CGeometry * getSubset(int index);
	int getTotalNumberOfSubsets();

	void init();

private:
	vector<CGeometry*> m_Subsets;

};