#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

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

void CCameraComponent::moveDirection(float speed){
	D3DXVECTOR3 direction;


	D3DXMATRIX yawMatrix;
	direction = m_vecLookAt-this->getParent()->getTransform()->getPosition();
	D3DXVec3Normalize(&direction, &direction);

	direction.x += speed * cos(direction.x);
	direction.z += speed * sin(direction.z);
	m_vecLookAt.x = direction.x;
	m_vecLookAt.z = direction.z;
	this->getParent()->getTransform()->translate(direction.x,direction.y, direction.z);
	//D3DXMatrixRotationAxis(&pitchMatrix, &direction, );
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
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::update(float elapsedTime)
{
	CTransformComponent *pTransform=m_pParent->getTransform();

	D3DXMatrixLookAtLH(&m_matView,&pTransform->getPosition(),&m_vecLookAt,&m_vecUp);
	D3DXMatrixPerspectiveFovLH(&m_matProjection,m_fAspectRatio,m_fFOV,m_fNearClip,m_fFarClip);


}