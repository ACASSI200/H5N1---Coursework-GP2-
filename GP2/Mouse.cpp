#include "Mouse.h"

#include <Windows.h>

CMouse::CMouse()
{
	m_fMouseAbsX=0.0f;
	m_fMouseAbsY=0.0f;
	m_fMouseDeltaX=0.0f;
	m_fMouseDeltaY=0.0f;
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
	return m_fMouseDeltaX;
}

float CMouse::getRelativeMouseY()
{
	return m_fMouseDeltaY;
}

float CMouse::getAbsoluteMouseX()
{
	return m_fMouseAbsX;
}

float CMouse::getAbsoluteMouseY()
{
	return m_fMouseAbsY;
}