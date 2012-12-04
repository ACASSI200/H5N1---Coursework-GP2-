#pragma once

//Headers
#include "IComponent.h"
//Physics Header
#include "Physics.h"

//Collider Type
enum ColliderType
{
	Base=0,
	Sphere,
	Box,
	Capsule,
	Cylinder,
	Mesh
};

//Base collider
class CBaseCollider:public CBaseComponent
{
public:
	//Constructor
	CBaseCollider();
	//We going to inherit from the class so we
	//mark this as virtual
	virtual ~CBaseCollider();

	//Get the collider type
	ColliderType getColliderType()
	{
		return m_ColliderType;
	};

	//Is the shape create?
	bool shapeCreated()
	{
		//if this has a value return true else false
		return (m_pPhysicsShape?true:false);
	};

	//Grab the actual Havok Shape
	hkpConvexShape * physicsShape()
	{
		return m_pPhysicsShape;
	};
protected:
	//Collider type
	ColliderType m_ColliderType;
	//The Havok Shape
	hkpConvexShape * m_pPhysicsShape;
};