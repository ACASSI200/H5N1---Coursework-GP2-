#include "Input.h"
#include "Keyboard.h"
#include "Mouse.h"

CInput::CInput()
{
	
}

CInput::~CInput()
{
	if (m_pKeyboard)
	{
		delete m_pKeyboard;
		m_pKeyboard=0;
	}
	if (m_pMouse)
	{
		delete m_pMouse;
		m_pMouse=0;
	}
}

void CInput::init()
{
	m_pKeyboard=new CKeyboard();
	m_pMouse=new CMouse();
}