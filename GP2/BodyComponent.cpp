#include "BodyComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "BaseCollider.h"

//Constructor
CBodyComponent::CBodyComponent()
{
	//Name
	m_strName="BodyComponent";
	m_pRigidBody=NULL;
	//Are we fixed
	m_bFixed=false;
	//Mass
	m_fMass=1.0f;
}

//Deconstructor
CBodyComponent::~CBodyComponent()
{
	//Do we have a rigid body, if so clean up
	if (m_pRigidBody)
	{
		m_pRigidBody->removeReference();
	}
}

//Update method, this will be used to update the game objects position and rotation
void CBodyComponent::update(float elapsedTime)
{
	CBaseComponent::update(elapsedTime);

	//Get the transform component
	CTransformComponent * transform=getParent()->getTransform();
	//If we have a rigid body
	if (m_pRigidBody)
	{
		//get the current position and rotation of the rigid body
		hkVector4 pos=m_pRigidBody->getPosition();
		hkQuaternion rot=m_pRigidBody->getRotation();

		//Set the position and rotation of the transform 
		transform->setPosition(pos.getComponent(0),pos.getComponent(1),pos.getComponent(2));
		transform->setRotation(rot.m_vec.getComponent(0),rot.m_vec.getComponent(1),
			rot.m_vec.getComponent(2),rot.m_vec.getComponent(3));
	}
}

//init method
void CBodyComponent::init()
{
	//grab the collider. If we have one, then initialise
	CBaseCollider *pCollider=(CBaseCollider*)getParent()->getComponent("ColliderComponent");
	if (!pCollider->shapeCreated())
		pCollider->init();

	//Create the rigid body info
	hkpRigidBodyCinfo rigidBodyInfo;
	if (m_bFixed)
		rigidBodyInfo.m_motionType=hkpMotion::MOTION_FIXED;

	//associate the shape with the rigid body
	rigidBodyInfo.m_shape=pCollider->physicsShape();
	//Calculate the mass using the shapr and the mass of the object
	hkpInertiaTensorComputer::setShapeVolumeMassProperties(pCollider->physicsShape(), m_fMass, rigidBodyInfo);

	//Create the rigid body
	m_pRigidBody=new hkpRigidBody(rigidBodyInfo);

	//Assign user data
	m_pRigidBody->setUserData((hkUlong)getParent());

	//Grab the transform
	CTransformComponent *pTransform=getParent()->getTransform();

	//pos and rotate the rigid body to match our object
	hkVector4 pos;
	pos.set(pTransform->getPosition().x,pTransform->getPosition().y,pTransform->getPosition().z);
	hkQuaternion rot;
	rot.set(pTransform->getRotation().x,pTransform->getRotation().y,pTransform->getRotation().z,pTransform->getRotation().w);
	m_pRigidBody->setPosition(pos);
	m_pRigidBody->setRotation(rot);
}