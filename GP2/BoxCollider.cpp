#include "BoxCollider.h"

CBoxCollider::CBoxCollider()
{
	//set up some initial values
	m_fHeight=1.0f;
	m_fWidth=1.0f;
	m_fLength=1.0f;
	m_ColliderType=Box;
}

CBoxCollider::~CBoxCollider()
{
}

//init the box
void CBoxCollider::init()
{
	if (!m_pPhysicsShape){
		hkVector4 halfExtent( m_fWidth/2.0f, m_fHeight/2.0f, m_fLength/2.0f);
		m_pPhysicsShape= new hkpBoxShape( halfExtent, 0.0f );
	}
}