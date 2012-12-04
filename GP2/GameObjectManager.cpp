#include "GameObjectManager.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "Physics.h"
#include "BodyComponent.h"
#include "BaseCollider.h"

CGameObjectManager::CGameObjectManager()
{
}

CGameObjectManager::~CGameObjectManager()
{
	//call clear
	clear();
}

void CGameObjectManager::removeGameObject(CGameObject *pObject)
{

}

//add game object
void CGameObjectManager::addGameObject(CGameObject * pObject)
{
	//add to the vector for iteration
	m_GameObjects.push_back(pObject);
	//also add to map for quick lookups
	m_GameObjectsMap[pObject->getName()]=pObject;

	CCameraComponent * pCamera=static_cast<CCameraComponent*>(pObject->getComponent("CameraComponent"));
	if (pCamera)
	{
		m_pMainCamera=pCamera;
		m_Cameras.push_back(pCamera);
	}
}

//get beginning of vector
vector<CGameObject*>::iterator CGameObjectManager::getBegining()
{
	return m_GameObjects.begin();
}

//get the end of vector
vector<CGameObject*>::iterator CGameObjectManager::getEnd()
{
	return m_GameObjects.end();
}

//find the game object by name
CGameObject * CGameObjectManager::findGameObject(const string& name)
{
	return m_GameObjectsMap[name];
}

void CGameObjectManager::clear()
{
	//clear out the vector
	m_GameObjectIter=m_GameObjects.begin();
	while(m_GameObjectIter!=m_GameObjects.end())
	{
		//delete the element
		if ((*m_GameObjectIter))
		{
			delete (*m_GameObjectIter);
			m_GameObjectIter=m_GameObjects.erase(m_GameObjectIter);
		}
		else
		{
			m_GameObjectIter++;
		}
	}

	m_GameObjectsMap.clear();
}

//init, cycle through all the game objects
void CGameObjectManager::init()
{
	for(m_GameObjectIter=m_GameObjects.begin();m_GameObjectIter!=m_GameObjects.end();m_GameObjectIter++)
	{
		(*m_GameObjectIter)->init();
		//any physic components?
		CBodyComponent *pBody=(CBodyComponent*)(*m_GameObjectIter)->getComponent("BodyComponent");
		if (pBody)
			CPhysics::getInstance().getPhysicsWorld()->addEntity(pBody->getRigidBody());
	}
}

//update
void CGameObjectManager::update(float elapsedTime)
{
	for(m_GameObjectIter=m_GameObjects.begin();m_GameObjectIter!=m_GameObjects.end();m_GameObjectIter++)
	{
		(*m_GameObjectIter)->update(elapsedTime);
	}
}

//render
void CGameObjectManager::render()
{
	for(m_GameObjectIter=m_GameObjects.begin();m_GameObjectIter!=m_GameObjects.end();m_GameObjectIter++)
	{
		(*m_GameObjectIter)->render();
	}
}