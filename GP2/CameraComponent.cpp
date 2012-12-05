#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <stdio.h>


void CCameraComponent::movePosition(float speed){
	D3DXVECTOR3 direction;
	direction = m_vecLookAt-this->getParent()->getTransform()->getPosition();
	D3DXVec3Normalize(&direction, &direction);

	direction *= speed;
	m_vecLookAt += direction;
	this->getParent()->getTransform()->translate(direction.x, direction.y, direction.z);

	//rotation
	//m_vecLookAt.x += direction.x;
	//m_vecLookAt.x += direction.x;
	//this->getParent()->getTransform()->translate(direction.x, 0.0f, direction.z);
}

CCameraComponent::CCameraComponent()
{
	m_vecLookAt=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vecUp=D3DXVECTOR3(0.0f,1.0f,0.0f);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProjection);

	m_fAspectRatio=640.0f/480.0f;
	m_fFOV=D3DX_PI/4.0f;
	m_fNearClip=1.0f;
	m_fFarClip=1000.0f;

	m_strName="CameraComponent";
	m_fPitch=0.0f;
	m_fYaw=0.0f;
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::update(float elapsedTime)
{
	
	CTransformComponent *pTransform=m_pParent->getTransform();

	float limit = D3DX_PI / 2.0f - 0.01f;
    m_fPitch = __max(-limit, m_fPitch);
    m_fPitch = __min(+limit, m_fPitch);

	// Keep longitude in same range by wrapping.
    if (m_fYaw >  D3DX_PI)
    {
        m_fYaw -= D3DX_PI * 2.0f;
    }
    else if (m_fYaw < -D3DX_PI)
    {
        m_fYaw += D3DX_PI * 2.0f;
    }
	
	float r=cosf(m_fPitch);
	m_vecLookAt.y=sinf(m_fPitch);
	m_vecLookAt.z = r * cosf(m_fYaw);   
    m_vecLookAt.x = r * sinf(m_fYaw); 
	
	m_vecLookAt+=m_vecLookAt+pTransform->getPosition();

	D3DXMatrixLookAtLH(&m_matView,&pTransform->getPosition(),&m_vecLookAt,&m_vecUp);
	D3DXMatrixPerspectiveFovLH(&m_matProjection,m_fAspectRatio,m_fFOV,m_fNearClip,m_fFarClip);
}