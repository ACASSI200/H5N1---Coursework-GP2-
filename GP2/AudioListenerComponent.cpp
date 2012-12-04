#include "AudioListenerComponent.h"

#include "TransformComponent.h"
#include "GameObject.h"
#include "AudioSystem.h"

CAudioListenerComponent::CAudioListenerComponent()
{
	//set some inital values
	m_vecForward=D3DXVECTOR3(0.0f,0.0f,1.0f);
	m_vecVelocity=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vecLastPosition=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_strName="AudioListenerComponent";
}

CAudioListenerComponent::~CAudioListenerComponent()
{

}

void CAudioListenerComponent::update(float elapsedTime)
{
	//grab the transform
	CTransformComponent *pTransform=getParent()->getTransform();

	//values to be sent to FMOD
	FMOD_VECTOR pos;
	FMOD_VECTOR vel;
	FMOD_VECTOR forward;
	FMOD_VECTOR up;

	//define our up, should alwys be 0,1,0 ?
	up.x=0.0f;
	up.y=1.0f;
	up.z=0.0f;

	//get the current position of the transform
	pos.x=pTransform->getPosition().x;
	pos.y=pTransform->getPosition().y;
	pos.z=pTransform->getPosition().z;
	
	//calculate our direction
	m_vecForward=pTransform->getPosition()-m_vecLastPosition;
	//normalize so that it is a direction
	D3DXVec3Normalize(&m_vecForward,&m_vecForward);
	forward.x=m_vecForward.x;
	forward.y=m_vecForward.y;
	forward.z=m_vecForward.z;

	//get our velocity, we also need to scale it for FMOD
	m_vecVelocity=(pTransform->getPosition()-m_vecLastPosition)*1000/elapsedTime;
	vel.x=m_vecVelocity.x;
	vel.y=m_vecVelocity.y;
	vel.z=m_vecVelocity.z;

	//Update our listener properties
	CAudioSystem::getInstance().updateListener(pos,vel,forward,up);
	
	//get the last position of the listener
	m_vecLastPosition=pTransform->getPosition();
}