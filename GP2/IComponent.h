#pragma once

#include <string>
using namespace std;

#include <d3d10.h>

//forward decleration of GameObject, we can
//do this to use instance of the class without
//inculding the header file
class CGameObject;

//Interface class
class IComponent
{
public:
	//Empty deconstructor, we need to do this or
	//we will have a memory leak
	virtual ~IComponent(){};

	//Get name pure vitrual function
	virtual const string& getName()=0;

	//Update
	virtual void update(float elapsedTime)=0;

	//Render
	virtual void render()=0;

	//init
	virtual void init()=0;

	//Get Parent
	virtual CGameObject * getParent()=0;

	//Set parent
	virtual void setParent(CGameObject *pParent)=0;

	//enable/disable
	virtual void enable()=0;
	virtual void disable()=0;

	//set the rendering device
	virtual void SetRenderingDevice(ID3D10Device *pDevice)=0;

};

//BaseComponent which implements the Interface
class CBaseComponent:public IComponent
{
public:
	//Constructor
	CBaseComponent()
	{
		m_pParent=NULL;
		m_bIsEnabled=true;
		m_strName="BaseComponent";
	};

	//deconstructor
	virtual ~CBaseComponent(){};

	//get the name
	const string& getName()
	{
		return m_strName;
	};

	//Virtual update, this can be overidden in classes that
	//inherit from this class
	virtual void update(float elapsedTime)
	{
	};

	//render
	virtual void render()
	{
	};

	//init
	virtual void init()
	{
	};

	//Get the Game Object that this is attached too
	CGameObject * getParent()
	{
		return m_pParent;
	};

	//Set the parent
	void setParent(CGameObject *pParent)
	{
		m_pParent=pParent;
	};

	//enable
	void enable()
	{
		m_bIsEnabled=true;
	};
	
	//disable
	void disable()
	{
		m_bIsEnabled=false;
	};

	//Set rendering device
	void SetRenderingDevice(ID3D10Device *pDevice)
	{
		m_pD3D10Device=pDevice;
	};
	//protected so that we can access in child classes
protected:
	//Parent
	CGameObject *m_pParent;
	//Enabled flag
	bool m_bIsEnabled;
	//Name
	string m_strName;
	//Rendering device
	ID3D10Device *m_pD3D10Device;
};