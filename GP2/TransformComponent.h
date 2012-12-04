#pragma once

#include "IComponent.h"
#include "GameObject.h"
#include "BodyComponent.h"

#include <D3D10.h>
#include <D3DX10.h>

//Transform component, this represents a transformation
//Scale, Rotation, Rotation combine to World Matrix
class CTransformComponent:public CBaseComponent
{
public:
	//Constructor
	CTransformComponent()
	{
		//Set to default values
		m_vecPosition=D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_vecRotation=D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_vecScale=D3DXVECTOR3(1.0f,1.0f,1.0f);
		D3DXMatrixIdentity(&m_matTranslate);
		D3DXMatrixIdentity(&m_matRotation);
		D3DXMatrixIdentity(&m_matScale);
		D3DXMatrixIdentity(&m_matWorld);
		D3DXQuaternionIdentity(&m_quatRotation);
		m_strName="TransformComponent";
	};
	
	~CTransformComponent(){};

	//Update method
	void update(float elapsedTime)
	{
		//Get the transform component
		CBodyComponent *pBody=(CBodyComponent*)getParent()->getComponent("BodyComponent");
		//If we have a rigid body
		if (pBody)
		{
			//get the current position and rotation of the rigid body
			hkVector4 pos=pBody->getRigidBody()->getPosition();
			hkQuaternion rot=pBody->getRigidBody()->getRotation();

			m_vecPosition.x=pos.getComponent(0);
			m_vecPosition.y=pos.getComponent(1);
			m_vecPosition.z=pos.getComponent(2);

			m_quatRotation.x=rot.m_vec.getComponent(0);
			m_quatRotation.y=rot.m_vec.getComponent(1);
			m_quatRotation.z=rot.m_vec.getComponent(2);
			m_quatRotation.w=rot.m_vec.getComponent(3);
		}
		//Build a rotation matrix, we can't use quaternions directly
		D3DXMatrixRotationQuaternion(&m_matRotation,&m_quatRotation);

		//Create translation matrix
		D3DXMatrixTranslation(&m_matTranslate,m_vecPosition.x,m_vecPosition.y,m_vecPosition.z);
		//Create scale matrix
		D3DXMatrixScaling(&m_matScale,m_vecScale.x,m_vecScale.y,m_vecScale.z);

		//Multiply the matrices together
		D3DXMatrixMultiply(&m_matWorld,&m_matScale,&m_matRotation);
		D3DXMatrixMultiply(&m_matWorld,&m_matWorld,&m_matTranslate);
	};

	//set position
	void setPosition(float x,float y,float z)
	{
		m_vecPosition=D3DXVECTOR3(x,y,z);

		CBodyComponent *pBody=(CBodyComponent*)getParent()->getComponent("BodyComponent");
		if (pBody)
		{
			pBody->getRigidBody()->setPosition(hkVector4(x,y,z));
		}
	};

	//set rotation
	void setRotation(float x,float y,float z)
	{
		m_vecRotation=D3DXVECTOR3(x,y,z);
		//We are using Quaternion for rotation, no gimble lock
		D3DXQuaternionRotationYawPitchRoll(&m_quatRotation,m_vecRotation.y,m_vecRotation.x,m_vecRotation.z);

		CBodyComponent *pBody=(CBodyComponent*)getParent()->getComponent("BodyComponent");
		if (pBody)
		{
			pBody->getRigidBody()->setRotation(hkQuaternion(m_quatRotation.x,m_quatRotation.y,m_quatRotation.z,m_quatRotation.w));
		}		
	};

	void setRotation(float x,float y,float z,float w)
	{
		m_quatRotation=D3DXQUATERNION(x,y,z,w);
		CBodyComponent *pBody=(CBodyComponent*)getParent()->getComponent("BodyComponent");
		if (pBody)
		{
			pBody->getRigidBody()->setRotation(hkQuaternion(m_quatRotation.x,m_quatRotation.y,m_quatRotation.z,m_quatRotation.w));
		}
		//D3DXQuaternionRotationYawPitchRoll(&m_quatRotation,x,y,z);
		//get euler rotation
	};

	//set scale
	void setScale(float x,float y,float z)
	{
		m_vecScale=D3DXVECTOR3(x,y,z);
	};

	D3DXQUATERNION& getRotation()
	{
		return m_quatRotation;
	};

	D3DXVECTOR3& getPosition()
	{
		return m_vecPosition;
	};

	//get world
	D3DXMATRIX& getWorld()
	{
		return m_matWorld;
	};

	//rotate
	void rotate(float x,float y,float z)
	{
		m_vecRotation.x+=x;
		m_vecRotation.y+=y;
		m_vecRotation.z+=z;
		setRotation(m_vecRotation.x,m_vecRotation.y,m_vecRotation.z);
	};

	//translate
	void translate(float x,float y,float z)
	{
		m_vecPosition.x+=x;
		m_vecPosition.y+=y;
		m_vecPosition.z+=z;
		setPosition(m_vecPosition.x,m_vecPosition.y,m_vecPosition.z);
	};

	//scale
	void scale(float x,float y, float z)
	{
		m_vecScale.x+=x;
		m_vecScale.y+=y;
		m_vecScale.z+=z;
	};
private:
	//vectors
	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecRotation;
	D3DXVECTOR3 m_vecScale;

	//matrices
	D3DXMATRIX m_matTranslate;
	D3DXMATRIX m_matRotation;
	D3DXMATRIX m_matScale;

	//quaternion
	D3DXQUATERNION m_quatRotation;

	D3DXMATRIX m_matWorld;
};