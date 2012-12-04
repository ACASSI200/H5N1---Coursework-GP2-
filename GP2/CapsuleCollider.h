#pragma once

#include "BaseCollider.h"
#include "Physics.h"

//Capsule
class CCapsuleCollider:public CBaseCollider
{
public:
	CCapsuleCollider();
	~CCapsuleCollider();

	//get and set radius
	float getRadius()
	{
		return m_fRadius;
	};

	void setRadius(float radius)
	{
		m_fRadius=radius;
	};

	//set the start point of the cylinder, axis
	void setStart(float x,float y,float z)
	{
		m_fStartX=x;
		m_fStartY=y;
		m_fStartZ=z;
	};

	//get start values
	float getStartX(){return m_fStartX;}
	float getStartY(){return m_fStartY;}
	float getStartZ(){return m_fStartZ;}

	//set the end point, axis
	void setEnd(float x,float y,float z)
	{
		m_fEndX=x;
		m_fEndY=y;
		m_fEndZ=z;
	};

	//get end values
	float getEndX(){return m_fEndX;}
	float getEndY(){return m_fEndY;}
	float getEndZ(){return m_fEndZ;}

	//init
	void init();
private:
	float m_fRadius;
	float m_fStartX,m_fStartY,m_fStartZ;
	float m_fEndX,m_fEndY,m_fEndZ;
};