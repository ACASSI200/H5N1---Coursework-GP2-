#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <stdio.h>

void CCameraComponent::MyOutputFunction(const char *str, ...)
{

  char buf[2048];

  va_list ptr;
  va_start(ptr,str);
  vsprintf(buf,str,ptr);

  OutputDebugStringA(buf);
}

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

void CCameraComponent::moveDirection(float speed, CTimer m_Timer){
	D3DXVECTOR3 direction;
	D3DXVECTOR3 newDirection;
	direction = m_vecLookAt-this->getParent()->getTransform()->getPosition();
	D3DXVec3Normalize(&direction, &direction);
	float PI = 3.14159265;

	/*right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	lookat = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXVec3Normalize(&direction,&(lookat - right));
	D3DXMatrixRotationAxis(&m_pitch, &right, 45.0f);
	D3DXMatrixRotationAxis(&m_pitch, &lookat, 10.0f);
	D3DXVec3TransformCoord(&m_vecLookAt,&m_vecLookAt,&m_pitch);
	D3DXVec3Normalize(&lookat, &lookat);
	D3DXVec3Normalize(&right, &right);
	*/


	//direction.x =  speed * cos(m_Timer.getElapsedTime());
	//direction.z = speed * sin(m_Timer.getElapsedTime());
	//m_vecLookAt.z = direction.z;
	direction.x *= sin(90*180/PI);
	direction.z *= cos(90*180/PI);
	m_vecLookAt.x += direction.x;
	m_vecLookAt.z += direction.z;
	MyOutputFunction("xPos - %d",direction.x);
	MyOutputFunction(", zPos - %d \n",direction.z);
	this->getParent()->getTransform()->translate(direction.x, 0.0f, direction.z);
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