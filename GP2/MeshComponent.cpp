#pragma once

#include "MeshComponent.h"

CMeshComponent::CMeshComponent()
{
	m_strName="MeshComponent";
}

CMeshComponent::~CMeshComponent()
{
	vector<CGeometry*>::iterator iter=m_Subsets.begin();
	while(iter!=m_Subsets.end())
	{
		if ((*iter))
		{
			delete (*iter);
			(*iter)=NULL;
			iter=m_Subsets.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void CMeshComponent::addSubset(CGeometry *pGeom)
{
	m_Subsets.push_back(pGeom);
}

CGeometry * CMeshComponent::getSubset(int index)
{
	if (index>m_Subsets.size())
		return NULL;

	return m_Subsets[index];
}

int CMeshComponent::getTotalNumberOfSubsets()
{
	return m_Subsets.size();
}

void CMeshComponent::init()
{
	for(vector<CGeometry*>::iterator iter=m_Subsets.begin();iter!=m_Subsets.end();iter++)
	{
		(*iter)->createBuffers();
	}
}