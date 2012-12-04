#include "SphereCollider.h"

//Constructor
CSphereCollider::CSphereCollider()
{
	//Sphere
	m_ColliderType=Sphere;
	//Radius
	m_fRadius=1.0f;
}

//deconstructor
CSphereCollider::~CSphereCollider()
{

}

//init method
void CSphereCollider::init()
{
	//create our sphere shape
	if (!m_pPhysicsShape)
		m_pPhysicsShape=new hkpSphereShape( m_fRadius); 
}