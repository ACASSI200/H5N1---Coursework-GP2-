#include "Mouse.h"

#include <Windows.h>

CMouse::CMouse()
{
	m_fMouseAbsX=0.0f;
	m_fMouseAbsY=0.0f;
	m_fMouseDeltaX=0.0f;
	m_fMouseDeltaY=0.0f;
	m_bMouseMovedY=false;
	m_bMouseMovedX=false;
	ZeroMemory(&m_bMouseButtons,sizeof(m_bMouseButtons));
}

CMouse::~CMouse()
{
}

void CMouse::setMouseButtonsDown(bool mouse1,bool mouse2,bool mouse3)
{
	m_bMouseButtons[0]=mouse1;
	m_bMouseButtons[1]=mouse2;
	m_bMouseButtons[2]=mouse3;
}

void CMouse::setMouseMove(float x,float y)
{
	m_fMouseDeltaX=x;
	m_fMouseDeltaY=y;
	m_fMouseAbsX+=m_fMouseDeltaX;
	m_fMouseAbsY+=m_fMouseDeltaY;
	m_bMouseMovedY=true;
	m_bMouseMovedX=true;

}

bool CMouse::getMouseDown(int index)
{
	return m_bMouseButtons[index];
}

bool CMouse::getMouseUp(int index)
{
	return m_bMouseButtons[index];
}

float CMouse::getRelativeMouseX()
{
	float mouseX=m_fMouseDeltaX;
	if (m_bMouseMovedX)
	{
		m_bMouseMovedX=false;
		m_fMouseDeltaX=0.0f;
	}
	return mouseX;
}

float CMouse::getRelativeMouseY()
{
	float mouseY=m_fMouseDeltaY;
	if (m_bMouseMovedY)
	{
		m_bMouseMovedY=false;
		m_fMouseDeltaY=0.0f;
	}
	return mouseY;
}

float CMouse::getAbsoluteMouseX()
{
	return m_fMouseAbsX;
}

float CMouse::getAbsoluteMouseY()
{
	return m_fMouseAbsY;
}