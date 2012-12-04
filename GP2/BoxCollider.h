#pragma once

#include "BaseCollider.h"

//Box collider
class CBoxCollider:public CBaseCollider
{
public:
	CBoxCollider();
	~CBoxCollider();

	//init
	void init();

	//get width
	float getWidth()
	{
		return m_fWidth;
	};

	//get height
	float getHeight()
	{
		return m_fHeight;
	};

	//get length
	float getLength()
	{
		return m_fLength;
	};

	//set the size of the box
	void setExtents(float width,float height,float length)
	{
		m_fWidth=width;
		m_fHeight=height;
		m_fLength=length;
	};
private:
	float m_fWidth,m_fHeight,m_fLength;
};