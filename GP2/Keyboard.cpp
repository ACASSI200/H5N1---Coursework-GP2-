#include "Keyboard.h"
#include <Windows.h>

CKeyboard::CKeyboard()
{
	ZeroMemory(m_Keys,sizeof(bool)*MAX_NO_OF_KEYS);
}

CKeyboard::~CKeyboard()
{
}

bool CKeyboard::isKeyDown(int keycode)
{
	return m_Keys[keycode];
}

bool CKeyboard::isKeyUp(int keycode)
{
	return m_Keys[keycode];
}

bool CKeyboard::keyPressed(int keycode)
{
	return m_Keys[keycode];
}

void CKeyboard::setKeyDown(int keycode)
{
	m_Keys[keycode]=true;
}

void CKeyboard::setKeyUp(int keycode)
{
	m_Keys[keycode]=false;
}