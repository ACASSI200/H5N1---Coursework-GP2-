#include "BaseCollider.h"

CBaseCollider::CBaseCollider()
{
	//The name of the compoent
	m_strName="ColliderComponent";
	//Set it to the base component
	m_ColliderType=Base;
	//set it to NULL
	m_pPhysicsShape=NULL;
}

CBaseCollider::~CBaseCollider()
{
	//check to see if we have a shape, if we do remove the reference
	if (m_pPhysicsShape)
	{
		m_pPhysicsShape->removeReference();
	}
}