#include "CapsuleCollider.h"

CCapsuleCollider::CCapsuleCollider()
{
	m_ColliderType=Capsule;
	setStart(0.0f,0.0f,0.0f);
	setEnd(0.0f,1.0f,0.0f);
	m_fRadius=1.0f;
}

CCapsuleCollider::~CCapsuleCollider()
{
}

void CCapsuleCollider::init()
{
	if (!m_pPhysicsShape){
		hkVector4 start( m_fStartX,m_fStartY, m_fStartZ);
		hkVector4 end( m_fEndX,m_fEndY, m_fEndZ);
		// creates a capsule with a axis between "start" and "end", and the specified "radius"
		m_pPhysicsShape=new hkpCapsuleShape( start, end, m_fRadius ); 
	}
}