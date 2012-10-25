#pragma once

//http://stackoverflow.com/questions/1008019/c-singleton-design-pattern

#define MAX_NO_JOYPADS 4

class CKeyboard;
class CMouse;
class CJoypad;

class CInput
{
public:
	~CInput();
	CKeyboard * getKeyboard()
	{
		return m_pKeyboard;
	};

	CMouse * getMouse()
	{
		return m_pMouse;
	};

	CJoypad * getJoypad(int playerIndex)
	{
		return m_pJoypads[playerIndex];
	};

	void init();

	static CInput& getInstance()
	{
		static CInput instance;
		return instance;
	};
private:
	CInput();


	CInput(CInput const&){};
	void operator=(CInput const&){}; 

private:
	CKeyboard * m_pKeyboard;
	CMouse * m_pMouse;
	CJoypad **m_pJoypads;

};