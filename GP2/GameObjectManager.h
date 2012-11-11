#pragma once

//vector and map to hold Components
#include <vector>
#include <map>

using namespace std;
//forward decleration of Game Object
class CGameObject;
class CCameraComponent;
class CDirectionalLightComponent;

//Game Object Manager, used to hold all the game objects
class CGameObjectManager
{
public:
	//Constructor
	CGameObjectManager();
	~CGameObjectManager();

	//add game object
	void addGameObject(CGameObject * pObject);

	//get the beginning of vector for iteration
	vector<CGameObject*>::iterator getBegining();
	//get the end of the vectir for iteration
	vector<CGameObject*>::iterator getEnd();

	//find game object by name
	CGameObject * findGameObject(const string& name);

	//clear
	void clear();

	//init
	void init();
	//update
	void update(float elapsedTime);
	//rende
	void render();

	CCameraComponent * getMainCamera()
	{
		return m_pMainCamera;
	};

	void setMainCamera(CCameraComponent *pCamera)
	{
		m_pMainCamera=pCamera;
	};

	CDirectionalLightComponent * getMainLight()
	{
		return m_pMainLight;
	};

	void setMainLight(CDirectionalLightComponent * lightDir)
	{
		m_pMainLight=lightDir;
	};
private:
	//vector of game objects
	vector<CGameObject*> m_GameObjects;
	vector<CGameObject*>::iterator m_GameObjectIter;
	map<string,CGameObject*> m_GameObjectsMap;

	CCameraComponent *m_pMainCamera;
	CDirectionalLightComponent *m_pMainLight;
	vector<CCameraComponent*> m_Cameras;

};