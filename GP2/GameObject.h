#pragma once

//Forwared decleration of components
class IComponent;
class CTransformComponent;

#include <map>
#include <vector>
#include <string>

using namespace std;

//Game Object class
class CGameObject
{
public:
	//Constructor
	CGameObject();
	//Deconstructor
	~CGameObject();

	//Get transform component, every game object will have one
	CTransformComponent * getTransform()
	{
		return m_pTransform;
	};

	//Get component by name
	IComponent* getComponent(const string &name)
	{
		return m_ComponentMap[name];
	};

	//add component
	void addComponent(IComponent * pComponent);

	//init
	void init();
	//render
	void render();
	//update
	void update(float elapsedTime);

	//set name
	void setName(const string& name)
	{
		m_strName=name;
	};

	//get name
	const string& getName()
	{
		return m_strName;
	};
private:
	//transform component
	CTransformComponent * m_pTransform;
	//map of components
	map<string,IComponent*> m_ComponentMap;
	//vector of components
	vector<IComponent*> m_Components;
	//name
	string m_strName;
};