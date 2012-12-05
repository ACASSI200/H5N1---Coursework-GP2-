#pragma once

//Component
#include "IComponent.h"
//Physics
#include "Physics.h"

//Body Componet, wraps up rigid body
class CBodyComponent:public CBaseComponent
{
public:
	CBodyComponent();
	~CBodyComponent();

	//Update
	void update(float elapsedTime);
	//Init
	void init();

	//Get the havok rigid body
	hkpRigidBody* getRigidBody()
	{
		return m_pRigidBody;
	};

	//Is the body fixed, not effected by gravity and other objects
	void setFixed(bool fixed)
	{
		m_bFixed=fixed;
	};

	//Add torque, rotational force
	void applyTorque(float x,float y,float z,float deltaTime)
	{
		m_pRigidBody->applyTorque(deltaTime,hkVector4(x,y,z));
	};

	//Apply Angular Impulse
	void applyAngularImpulse(float x,float y,float z)
	{
		m_pRigidBody->applyAngularImpulse(hkVector4(x,y,z));
	};

	//Add impulse
	void addImpulse(float x,float y,float z)
	{
		m_pRigidBody->applyLinearImpulse(hkVector4(x,y,z));
	};

	//Add force
	void addForce(float x,float y,float z,float deltaTime)
	{
		m_pRigidBody->applyForce(deltaTime,hkVector4(x,y,z));
	};

	//Add force at point
	void addForceAtPoint(float x,float y,float z,float deltaTime, float pointX,float pointY,float pointZ)
	{
		m_pRigidBody->applyForce(deltaTime,hkVector4(x,y,z),hkVector4(pointX,pointY,pointZ));
	};
private:
	hkpRigidBody* m_pRigidBody;
	float m_fMass;
	bool m_bFixed;
};